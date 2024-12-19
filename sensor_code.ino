// Pin Definitions
const int sensorPin = A0;   // Analog pin connected to the sensor's output
const float Vcc = 5.0;      // Arduino operating voltage (5.0V for most boards)

// Threshold for detecting safe/unsafe air quality
const float CO_THRESHOLD = 9.0; // Example threshold for CO in ppm

void setup() {
  // Initialize the Serial Monitor
  Serial.begin(9600);
  Serial.println("Electrochemical Sensor Test Starting...");
}

void loop() {
  // Read sensor's analog output
  int rawValue = analogRead(sensorPin);
  
  // Convert raw value to voltage
  float voltage = (rawValue / 1023.0) * Vcc;

  // Convert voltage to gas concentration (ppm)
  // The conversion factor depends on the specific sensor. Example: MQ-7
  // Consult your sensor's datasheet for the exact formula.
  float gasConcentrationPPM = voltage * 100;  // Example formula
  
  // Determine air quality status
  String airQualityStatus;
  if (gasConcentrationPPM > CO_THRESHOLD) {
    airQualityStatus = "Unsafe";
  } else {
    airQualityStatus = "Safe";
  }

  // Output readings and status to Serial Monitor
  Serial.print("Raw Value: ");
  Serial.print(rawValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Gas Concentration: ");
  Serial.print(gasConcentrationPPM, 2);
  Serial.print(" ppm | Status: ");
  Serial.println(airQualityStatus);

  // Delay between readings
  delay(1000);  // Wait for 1 second
}
