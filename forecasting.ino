const express = require('express');
const app = express();
const simpleStats = require('simple-statistics');
const PORT = 3000;

// In-memory storage for sensor data (each entry contains ppm value and a timestamp)
let airQualityData = [];

// Add new data to airQualityData
function addData(ppm) {
  const timestamp = new Date();
  airQualityData.push({ ppm, timestamp });
  if (airQualityData.length > 100) {
    // Maintain the last 100 records for prediction (Optional: you can adjust the number)
    airQualityData.shift();
  }
}

// Endpoint to receive data from Arduino sensor
app.post('/air-quality', (req, res) => {
  const { ppm } = req.body; // Get ppm value from Arduino
  addData(ppm); // Add new sensor data to our array

  res.status(200).send('Data received');
});

// Endpoint to get real-time sensor data
app.get('/real-time-data', (req, res) => {
  const latestData = airQualityData.length > 0 ? airQualityData[airQualityData.length - 1] : null;
  res.json(latestData || { message: 'No data available' });
});

// Endpoint to get a forecasted ppm value for the next time period
app.get('/forecast', (req, res) => {
  if (airQualityData.length < 2) {
    return res.status(400).json({ message: 'Not enough data for forecasting' });
  }

  // Prepare historical data for regression (timestamp to numeric)
  const timestamps = airQualityData.map(data => (data.timestamp.getTime())); // Convert time to milliseconds
  const ppmValues = airQualityData.map(data => data.ppm);

  // Apply Linear Regression to predict future value (next time period)
  const regression = simpleStats.linearRegression([timestamps, ppmValues]);

  // Predict the next ppm value (using the next time step)
  const nextTimestamp = new Date().getTime() + 1 * 60 * 1000; // Predict 1 minute ahead
  const predictedPpm = simpleStats.linearRegressionLine(regression)(nextTimestamp);

  res.json({
    forecastedPpm: predictedPpm,
    timestamp: new Date(nextTimestamp).toISOString(),
    status: predictedPpm > 500 ? "Unsafe" : "Safe", // Basic threshold for safety (can be customized)
  });
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
