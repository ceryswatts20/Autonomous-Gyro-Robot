// Libraries
// Dual Motor Driver
#include <CytronMotorDriver.h>
// Servos
#include <Servo.h>
// PID Controllers
#include <PID_v1.h>
// BLE connection
#include <ArduinoBLE.h>

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

// Define pins
// Head tilt servo
const int HEAD_TILT = 6;
// Head pan servo
const int HEAD_PAN = 11;
// Head spin servo
const int HEAD_SPIN = 7;

// Cytron motor driver
// Motor 1 PWM
const int PWM1 = 3;
// Motor 1 Dir
const int DIR1 = 4;
// Motor 2 PWM
const int PWM2 = 9;
// Motor 2 Dir
const int DIR2 = 10;

// BTS7960 motor driver
// Directions pins
const int R_EN = 34;
const int L_EN = 33;
// Speed pins
const int R_PWM = 32;
const int L_PWM = 31;

const int LATERAL_POT = A0;
/*
// Define PID parameters
double xSetpoint, ySetpoint;
double xAxisInput, yAxisInput;
double xAxisOutput, yAxisOutput;
double xKp = 1, xKi = 0, xKd = 0;
double yKp = 1, yKi = 0, yKd = 0;
*/

// Create objects
// Drive motor
CytronMD lateralMotor(PWM_DIR, PWM2, DIR2);
//  motor
CytronMD driveMotor(PWM_DIR, PWM1, DIR1);
// Spin motor

// Head tilt servo
Servo headTilt;
// Head pan servo
Servo headPan;
// Head spin servo
Servo headSpin;
/*
// X axis PID Controller
PID xAxisPID(&xAxisInput, &xAxisOutput, &xSetpoint, xKp, xKi, xKd, DIRECT);
// Y Axis PID Controller
PID yAxisPID(&yAxisInput, &yAxisOutput, &yAxisOutput, yKp, yKi, yKd, DIRECT);
*/
String receivedMessage, command, data;
int comma, speed, angle;
double x, y, inverse_x, inverse_y, x_percent, y_percent;
char receivedChar;

const int DEADZONE_MAX = 150;
const int DEADZONE_MIN = 100;
const int H_TILT_START = 115;
const int H_PAN_START = 90;
const int POT_MIN = 300;
const int POT_MAX = 580;
const int POT_CENTER = 400;

void setup() {
  // Lateral motor setup
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  
  // Attach servos to their pins
  headTilt.attach(HEAD_TILT);
  headPan.attach(HEAD_PAN);
  headSpin.attach(HEAD_SPIN);
  // Stop servo
  headSpin.write(95);
  // Initialize servo positions
  headTilt.write(H_TILT_START);
  headPan.write(H_PAN_START);

  /*
  // Set initial setpoints for PID controllers
  xSetpoint = 
  ySetpoint = 

  // Initialize PID controllers
  xAxisPID.SetMode(AUTOMATIC);
  // Motor limits - analogueWrite values
  xAxisPID.SetOutputLimits(-255, 255);
  // ???
  xAxisPID.SetSampleTime(50);
  yAxisPID.SetMode(AUTOMATIC);
  // Motor limits - analogueWrite values
  yAxisPID.SetOutputLimits(-255, 255);
  // ???
  yAxisPID.SetSampleTime(50);
  */

  // Start serial communication
  Serial.begin(9600);
  // Uno
  Serial1.begin(115200);
  // Nano
  Serial2.begin(115200);

  // Start BLE
  if (!BLE.begin()) {
    Serial.println("Bluetooth Low Energy module failed!");
    while(1);
  }

  BLE.setLocalName("Giga (Central)");
  BLE.advertise();

  Serial.println("Arduino Giga R1 WiFi (Central)");
  Serial.println(" ");
}

void loop() {
  // Enables motor to spin in both directions
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  //lateralMotor.setSpeed(255);

  //Serial.println(analogRead(LATERAL_POT));

  // If there is data waiting from the controller
  while (Serial1.available() > 0) {
    // Read 1 character
    receivedChar = Serial1.read();
    if (receivedChar == '\n') {
      // Save the received message
      command = receivedMessage;
      Serial.println(command);
      // Reset the received message
      receivedMessage = "";
    } 
    else {
      // Append characters to the received message
      receivedMessage += receivedChar;  
    }
  }
  // If there is data waiting from the gyroscope
  while (Serial2.available() > 0) {
    // Read 1 character
    receivedChar = Serial2.read();
    if (receivedChar == '\n') {
      // Save the received data
      data = receivedMessage;
      // Reset the variable
      receivedMessage = "";
    }
    else {
      // Append the character to the rest
      receivedMessage += receivedChar;
    }
  }

  // Analogue sticks
  // Left
  if (command.startsWith("lx")) {
    // Sanatize command
    command.replace("lx=", "");
    command.replace("ly=", "");
    comma = command.indexOf(',');
    x = command.substring(0, comma).toDouble();
    y = command.substring(comma+1).toDouble();
    Serial.println(command);
    // If in deadzone 101->149
    /*if (x < DEADZONE_MAX && x > DEADZONE_MIN) {
      int pot = analogRead(LATERAL_POT);
      Serial.println(pot);
      while (pot > (POT_CENTER + 5) || pot < (POT_CENTER - 5)) {
        if (pot > POT_CENTER) {
          speed = -255;
        }
        else if (pot < POT_CENTER) {
          speed = 255;
        }
        lateralMotor.setSpeed(speed);
        pot = analogRead(LATERAL_POT);
        Serial.println(pot);
      }
    }
    // If left stick pushed right
    else if (x >= DEADZONE_MAX) {
      // Scale x
      int position = map(x, DEADZONE_MAX, 255, POT_CENTER, POT_MAX);
      Serial.print("Position");
      Serial.println(position);
      int pot_reading = analogRead(LATERAL_POT);
      while (position < pot_reading && pot_reading < POT_MAX) {
        lateralMotor.setSpeed(-255);
        pot_reading = analogRead(LATERAL_POT);
        Serial.println(pot_reading);
      }
    }
    // If left stick pushed left
    else if (x <= DEADZONE_MIN) {
      // Scale x
      int position = map(x, 0, DEADZONE_MIN, POT_MIN, POT_CENTER);
      Serial.print("Position");
      Serial.println(position);
      int pot_reading = analogRead(LATERAL_POT);
      while (position > pot_reading && pot_reading > POT_MIN) {
        lateralMotor.setSpeed(255);
        pot_reading = analogRead(LATERAL_POT);
        Serial.println(pot_reading);
      }
    }
    else {
      Serial.println("Error: Left x axis");
    }*/
    // If in deadzone 101->149
    if (x < DEADZONE_MAX && x > DEADZONE_MIN) {
      int pot = analogRead(LATERAL_POT);
      lateralMotor.setSpeed(100);
      delay(250);
      int pot2 = analogRead(LATERAL_POT);
      if (pot2-pot > 0) {
        if (POT_CENTER - pot > 0) {
          while (pot > (POT_CENTER +5) || pot < (POT_CENTER-5)) {
            pot = analogRead(LATERAL_POT);
          }
          lateralMotor.setSpeed(0);
        }
        else {
          lateralMotor.setSpeed(-100);
          while (pot > (POT_CENTER +5) || pot < (POT_CENTER-5)) {
            pot = analogRead(LATERAL_POT);
          }
          lateralMotor.setSpeed(0);
        }
      }
      else {
        if (pot - POT_CENTER > 0) {
          while (pot > (POT_CENTER +5) || pot < (POT_CENTER-5)) {
            pot = analogRead(LATERAL_POT);
          }
          lateralMotor.setSpeed(0);
        }
        else {
          lateralMotor.setSpeed(100);
          while (pot > (POT_CENTER +5) || pot < (POT_CENTER-5)) {
            pot = analogRead(LATERAL_POT);
          }
          lateralMotor.setSpeed(0);
        }
      }
    }
    // If left stick pushed right
    else if (x >= DEADZONE_MAX) {
      int pot_reading = analogRead(LATERAL_POT);
      while (pot_reading < POT_MAX) {
        lateralMotor.setSpeed(255);
        pot_reading = analogRead(LATERAL_POT);
        Serial.println(pot_reading);
      }
      lateralMotor.setSpeed(0);
    }
    // If left stick pushed left
    else if (x <= DEADZONE_MIN) {
      int pot_reading = analogRead(LATERAL_POT);
      while (pot_reading > POT_MIN) {
        lateralMotor.setSpeed(-255);
        pot_reading = analogRead(LATERAL_POT);
        Serial.println(pot_reading);
      }
      lateralMotor.setSpeed(0);
    }
    else {
      Serial.println("Error: Left x axis");
    }

    // If in deadzone 101->149
    if (y < DEADZONE_MAX && y > DEADZONE_MIN) {
      // Stop motor
      driveMotor.setSpeed(0);
    }
    // If left stick pushed up
    else if (y <= DEADZONE_MIN) {
      // Scale controller input to motor speed
      inverse_y = 1 - y/DEADZONE_MIN;
      // 254 is motor forward range
      speed = (254*inverse_y) + 1;
      driveMotor.setSpeed(speed);
      Serial.println(speed);
    }
    // If left stick pushed down
    else if (y >= DEADZONE_MAX) {
      // Scale controller input to motor speed
      y_percent = (y-DEADZONE_MAX)/(255-DEADZONE_MAX);
      // -254 is motor backward range
      speed = (-254*y_percent) - 1;
      driveMotor.setSpeed(speed);
      Serial.println(speed);
    }
    else {
      Serial.println("Error: Left y axis");
    }
  }
  // Right
  if (command.startsWith("rx")) {
    command.replace("rx=", "");
    command.replace("ry=", "");
    //Serial.println(command);
    comma = command.indexOf(',');
    x = command.substring(0, comma).toDouble();
    y = command.substring(comma+1).toDouble();

    // If in deadzone 101->149
    if (x < DEADZONE_MAX && x > DEADZONE_MIN) {
      // Return to start position
      headPan.write(H_PAN_START);
    }
    // If right stick pushed right
    else if (x >= DEADZONE_MAX) {
      // Scale controller input to servo angle
      x_percent = (x-DEADZONE_MAX)/(255-DEADZONE_MAX);
      // Angle ranges * controller % + start position + 1
      angle = (180.0-H_PAN_START-1.0)*x_percent + H_PAN_START + 1;
      Serial.println(angle);
      headPan.write(angle);
    }
    // If right stick pushed left
    else if (x <= DEADZONE_MIN) {
      // Get controller input %
      x_percent = x/DEADZONE_MIN;
      //
      angle = (H_PAN_START-1)*x_percent;
      Serial.println(angle);
      headPan.write(angle);
    }
    else {
      Serial.println("Error: Right x axis");
    }

    // If in deadzone 101->149
    if (y < DEADZONE_MAX && y > DEADZONE_MIN) {
      // Return to start position
      headTilt.write(H_TILT_START);
    }
    // If right stick pushed up
    else if (y <= DEADZONE_MIN) {
     // Get controller input %
      y_percent = y/DEADZONE_MIN;
      //
      angle = (H_TILT_START-1)*y_percent;
      Serial.println(angle);
      headTilt.write(angle);
    }
    // If right stick pushed down
    else if (y >= DEADZONE_MAX) {
       // Scale controller input to servo angle
      y_percent = (y-DEADZONE_MAX)/(255-DEADZONE_MAX);
      // Angle ranges * controller % + start position + 1
      angle = (180.0-H_TILT_START-1.0)*y_percent + H_TILT_START + 1;
      Serial.println(angle);
      headTilt.write(angle);
    }
    else {
      Serial.println("Error: Right y axis");
    }
  }

  // D Pad
  // If Up is pressed
  if (command.startsWith("dup")) {
    
  }
  // If Up is released
  if (command.startsWith("dur")) {
    
    // Reset command
    command = "";
  }
  // If Down is pressed
  if (command.startsWith("ddp")) {

  }
  // If Down is released
  if (command.startsWith("ddr")) {
    // Reset command
    command = "";

  }
  // If Right is pressed
  if (command.startsWith("drp")) {

  }
  // If Right is released
  if (command.startsWith("drr")) {
    // Reset command
    command = "";

  }
  // If Left is pressed
  if (command.startsWith("dlp")) {

  }
  // If Left is released
  if (command.startsWith("dlr")) {
    // Reset command
    command = "";

  }

  // Buttons
  // If X is pressed
  if (command.startsWith("xp")) {

  }
  // If X is released
  if (command.startsWith("xr")) {
    // Reset command
    command = "";

  }
  // If Circle is pressed
  if (command.startsWith("cp")) {
    // Body spin clockwise

  }
  // If Circle is released
  if (command.startsWith("cr")) {
    // Stop body spin

    // Reset command
    command = "";

  }
  // If Triangle is pressed
  if (command.startsWith("tp")) {

  }
  // If Triangle is released
  if (command.startsWith("tr")) {
    // Reset command
    command = "";

  }
  // If Square is pressed
  if (command.startsWith("sp")) {
    // Body spin anti-clockwise

  }
  // If Square is released
  if (command.startsWith("sr")) {
    // Stop bosy spin

    // Reset command
    command = "";

  }
  // If R1 is pressed
  if (command.startsWith("R1p")) {
    // Dome spin clockwise while pressed
    headSpin.write(150);
  }
  // If R1 is released
  if (command.startsWith("R1r")) {
    // Stop dome spin
    headSpin.write(95);
    // Reset command
    command = "";
  }
  // If L1 is pressed
  if (command.startsWith("L1p")) {
    // Dome spin clockwise while pressed
    headSpin.write(30);
  }
  // If L1 is released
  if (command.startsWith("L1r")) {
    // Stop dome spin
    headSpin.write(95);
    // Reset command
    command = "";
  }
  // If R2 is pressed
  if (command.startsWith("R2p")) {

  }
  // If R2 is released
  if (command.startsWith("R2r")) {
    // Reset command
    command = "";

  }
  // If L2 is pressed
  if (command.startsWith("L2p")) {

  }
  // If L2 is released
  if (command.startsWith("L2r")) {
    // Reset command
    command = "";

  }
  // If R3 is pressed
  if (command.startsWith("R3p")) {

  }
  // If R3 is released
  if (command.startsWith("R3r")) {
    // Reset command
    command = "";

  }
  // If L3 is pressed
  if (command.startsWith("L3p")) {
    
  }
  // If L3 is released
  if (command.startsWith("L3r")) {
    
    // Reset command
    command = "";

  }
  // If SELECT is pressed
  if (command.startsWith("sep")) {

  }
  // If SELECT is released
  if (command.startsWith("ser")) {
    // Reset command
    command = "";

  }
  // If START is pressed
  if (command.startsWith("stp")) {

  }
  // If START is released
  if (command.startsWith("str")) {
    // Reset command
    command = "";

  }

}

void connectToPeripheral() {
  BLEDevice peripheral;

  Serial.println("Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);

  if (peripheral) {
    Serial.println("Peripheral device found!");
    Serial.print("Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println("");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("Connecting to peripheral device...");
  
  if (peripheral.connect()) {
    Serial.println("Connected to peripheral device!");
    Serial.println(" ");
  }
  else {
    Serial.println("Connection to pheripheral device failed");
    Serial.println(" ");
    return;
  }
  
  Serial.println("Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Peripheral device attributes discovered!");
    Serial.println(" ");
  }
  else {
    Serial.println("Peripheral device attributes discovery failed");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic distance = peripheral.characteristic(deviceServiceCharacteristicUuid);

  if (!distance) {
    Serial.println("Peripheral device does not have distance characteristic");
    peripheral.disconnect();
    return;
  } else if (!distance.canWrite()) {
    Serial.println("Peripheral does not have writable distance characteristic");
    peripheral.disconnect();
    return;
  }

  while (peripheral.connected()) {

  }
}
