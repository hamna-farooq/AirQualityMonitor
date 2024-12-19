const express = require('express');
const app = express();
const PORT = 3000;

// Middleware to parse JSON requests
app.use(express.json());

// Temporary storage for air quality data (just in memory)
let airQualityData = [];

// Endpoint to receive data from Arduino sensor
app.post('/air-quality', (req, res) => {
  const { ppm } = req.body; // Get ppm value from request

  // Basic logic to categorize air quality
  let airQualityStatus = 'Safe';
  if (ppm > 500) {
    airQualityStatus = 'Unsafe';
  }

  // Save data to temporary storage
  airQualityData.push({ ppm, airQualityStatus, timestamp: new Date() });

  console.log(`Received Data: PPM = ${ppm}, Status = ${airQualityStatus}`);
  res.status(200).send('Data received');
});

// Endpoint to fetch real-time data (latest sensor data)
app.get('/real-time-data', (req, res) => {
  const latestData = airQualityData.length > 0 ? airQualityData[airQualityData.length - 1] : null;

  if (latestData) {
    res.json(latestData);  // Send the latest air quality data in JSON format
  } else {
    res.json({ message: 'No data available yet.' });
  }
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
