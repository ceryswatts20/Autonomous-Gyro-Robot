// I2C connection
#include <Wire.h>
// TF Luna LiDAR sensor
#include <TFLI2C.h> 

// Create TF Luna sensor object
TFLI2C Lidar;

int16_t distance;
// Use default I2C address
int16_t address = TFL_DEF_ADR;

void setup() {
  // Initalise serial port
  Serial.begin(115200);
  // Initalise Wire library
  Wire.begin();           
}

void loop(){
    // Get sensor reading
    if(Lidar.getData(distance, address)) {
        Serial.println(String(tfDist) + " cm");
    }
    delay(50);
}
