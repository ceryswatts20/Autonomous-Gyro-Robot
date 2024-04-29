// BLE connection
#include <ArduinoBLE.h>

// Custom UUIDs
const char* serviceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* characteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

// User defined service
BLEService testService(serviceUuid);
// Read-only
BLEStringCharacteristic messageCharacteristic(characteristicUuid, BLERead);

double distance = 82.2;

void setup() {
  // Initialise serial monitor
  Serial.begin(9600);
  while (!Serial); 

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  // Set device name
  BLE.setLocalName("Dome (Peripheral)");
  // Advertise service
  BLE.setAdvertisedService(testService);
  // Add characteristic to service
  objectDetectionService.addCharacteristic(messageCharacteristic);
  // Add service
  BLE.addService(testService);
  // Set the initial value for the characeristic
  distanceCharacteristic.setValue("Hello World!");

  // Start advertising
  BLE.advertise();
  Serial.print("Peripheral device MAC: ");
  Serial.println(BLE.address());
  Serial.println("Waiting for connections...");
}

void loop() {
  // Wait for a BLE central to connect
  BLEDevice central = BLE.central();

  // If a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central MAC: ");
    // Print the central's BT address:
    Serial.println(central.address());

    // Keep looping while connected
    while (central.connected()) {} 
    
    // When the central disconnects
    Serial.print("Disconnected from central MAC: ");
    Serial.println(central.address());
  }
}
