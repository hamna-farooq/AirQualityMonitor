const express = require('express');
const app = express();
const PORT = 3000;

// Middleware to parse incoming JSON data
app.use(express.json());

// Temporary storage for air quality data (in memory)
let airQualityData = [];

// Receive air quality data from Arduino sensor
app.post('/air-quality', (req, res) => {
  const { ppm } = req.body; // Get ppm data from the incoming request body
  console.log('Received PPM data: ', ppm);

  // Check if the air quality is safe (threshold example: 500 ppm)
  let airQualityStatus = 'Safe';
  if (ppm > 500) {
    airQualityStatus = 'Unsafe';
  }

  // Save the air quality data (example in memory, but can save to a database)
  airQualityData.push({ ppm, airQualityStatus, timestamp: new Date() });
  
  // Send back a response to Arduino
  res.status(200).send('Data received');
});

// Endpoint to fetch the latest air quality data
app.get('/real-time-data', (req, res) => {
  // Send the most recent air quality data to the frontend (webpage)
  const latestData = airQualityData.length > 0 ? airQualityData[airQualityData.length - 1] : null;
  
  if (latestData) {
    res.json(latestData); // Respond with JSON containing ppm and air quality status
  } else {
    res.json({ message: 'No data available yet.' });
  }
});

// Start the server
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
