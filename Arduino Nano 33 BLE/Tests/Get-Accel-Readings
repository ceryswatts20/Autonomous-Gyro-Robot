// Read IMU values
#include "Arduino_LSM9DS1.h"

// Variable for accelerometer readings
float x, y, z;

void setup() {
  // Initialise serial port
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  // Attempt to initialise IMU
  if (!IMU.begin()) {
    Serial.println("Failed to intialise IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
}

void loop() {
  // Check if new accelerometer data is available
  if (IMU.accelerationAvailable()) {
    // Get accelerometer readings
    IMU.readAcceleration(x, y, z);
    // Print the values
    Serial.println("X = " + String(x) + "m/s^2\tY = " + String(y) + "m/s^2\tZ = " + String(z) + "m/s^2");
  }
}
