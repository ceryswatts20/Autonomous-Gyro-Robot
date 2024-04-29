// Read IMU values
#include "Arduino_LSM9DS1.h"

// Variable for gyro readings
float x, y, z;

void setup() {
  // Initialise serial port
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  // Attempt to intialise IMU
  if (!IMU.begin()) {
    Serial.println("Failed to intialise IMU!");
    while (1);
  }
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
}

void loop() {
  // Check if new gyroscope data is available
  if (IMU.gyroscopeAvailable()) {
    // Get gyro readings
    IMU.readGyroscope(x, y, z);
    // Print the values
    Serial.println("X = " + String(x) + "degrees/s\tY = " + String(y) + "degrees/s\tZ = " + String(z) + "degrees/s");
  }
}
