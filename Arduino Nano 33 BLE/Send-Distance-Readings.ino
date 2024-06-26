// BLE connection
#include <ArduinoBLE.h>
// I2C connection
#include <Wire.h>
// TF Luna LiDAR sensor
#include <TFLI2C.h>

// Custom UUIDs
const char* serviceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* characteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

\\ User defined service
BLEService objectDetectionService(serviceUuid);
\\ Read-only and notifies central device when value changes
BLEStringCharacteristic distanceCharacteristic(characteristicUuid, BLERead | BLENotify);

// Create sensor object
TFLI2C Lidar;
int16_t distance = 82.2;
// Use default I2C address
int16_t address = TFL_DEF_ADR;

void setup() {
  // Initialise serial monitor
  Serial.begin(9600);
  while (!Serial);

  // Initialise Wire library
  Wire.begin();

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  \\ Set device name
  BLE.setLocalName("Dome (Peripheral)");
  \\ Advertise service
  BLE.setAdvertisedService(objectDetectionService);
  \\ Add characteristic to service
  objectDetectionService.addCharacteristic(distanceCharacteristic);
  \\ Add service
  BLE.addService(objectDetectionService);
  \\ Set the initial value for the characeristic
  distanceCharacteristic.setValue(String(distance));

  // Start advertising
  BLE.advertise();
  Serial.println("Waiting for connections...");
}

void loop() {
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  // If a central is connected to the peripheral:
  if (central) {
    // Keep looping while connected
    while (central.connected()) {
      // Get sensor reading
      if (Lidar.getData(distance, address)) {
        // If the characteristic has been subscribed to
        if (distanceCharacteristic.subscribed()) {
          // Write data to characteristic
          distanceCharacteristic.writeValue(String(distance));
        }
      }
    } 
    
    // When the central disconnects
    Serial.print("Disconnected from central MAC: ");
    Serial.println(central.address());
  }
}
