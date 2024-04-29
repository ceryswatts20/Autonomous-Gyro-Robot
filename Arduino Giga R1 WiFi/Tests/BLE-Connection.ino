// BLE connection
#include <ArduinoBLE.h>

// Service UUID to look for
const char* serviceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
// Characteristic UUID to look for
const char* characteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

String distance;

void setup() {
  // Initialise serial monitor
  Serial.begin(9600);
  while (!Serial);

  // Initialise BLE
  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  // Set device name
  BLE.setLocalName("BB8 Body (Central)"); 
  // Advertise service
  BLE.advertise();
}

void loop() {
  connectToPeripheral();
}

/*
This method looks for a peripheral service, prints out it's identifing
information and connects to it.
*/
void connectToPeripheral(){
  // Create a BLE device
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  do
  {
    // Look for service
    BLE.scanForUuid(deviceServiceUuid);
    // Check if it's available
    peripheral = BLE.available();
  // If not available, repeat
  } while (!peripheral);

  // If connected
  if (peripheral) {
    // Print peripheral's info
    Serial.println("* Peripheral device found!");
    //Serial.print("* Device MAC address: ");
    //Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    //Serial.print("* Advertised service UUID: ");
    //Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    // Stop scanning
    BLE.stopScan();

    Serial.println("- Connecting to peripheral device...");
    // Attempt to connect to device
    if (peripheral.connect()) {
      Serial.println("* Connected to peripheral device!");
      Serial.println(" ");
    } 
    else {
      Serial.println("* Connection to peripheral device failed!");
      Serial.println(" ");
      // Exit method
      return;
    }

    getPeripheralData(peripheral);
  }
}

/*
This method subscribes to a BLEDevice's characteristics
*/
void getPeripheralData(BLEDevice peripheral) {
  Serial.println("- Discovering peripheral device attributes...");
  // Attempt to find all the device's characteristics
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } 
  else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    // Exit method
    return;
  }
  // Save device's characteristic at characteristicUuid
  BLEStringCharacteristic messageCharacteristic = peripheral.characteristic(characteristicUuid);

  // If the charateristic isn't there
  if (!messageCharacteristic) {
    Serial.println("* Peripheral device does not have a distance characteristic!");
    peripheral.disconnect();
    // Exit method
    return;
  }

  while (peripheral.connected()) {
    String message = "";
    // Save data into variable
    messageCharacteristic.value(message)
    Serial.println(message);
  }

  Serial.println("- Peripheral device disconnected!");
}
