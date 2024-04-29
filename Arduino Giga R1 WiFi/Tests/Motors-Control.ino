// Libraries
// Dual Motor Driver
#include <CytronMotorDriver.h>

// Cytron motor driver
// Motor 1 PWM
const int PWM1 = 3;
// Motor 1 Dir
const int DIR1 = 4;
// Motor 2 PWM
const int PWM2 = 9;
// Motor 2 Dir
const int DIR2 = 10;
// Steering pot
const int LATERAL_POT = A0;

//-----TODO--------
// BTS7960 motor driver


// Create objects
// Lateral motor
CytronMD lateralMotor(PWM_DIR, PWM2, DIR2);
//  Drive motor
CytronMD driveMotor(PWM_DIR, PWM1, DIR1);
// Spin motor

const int DEADZONE_MAX = 150;
const int DEADZONE_MIN = 100;
const int H_TILT_START = 115;
const int H_PAN_START = 90;
const int POT_MIN = 300;
const int POT_MAX = 580;
const int POT_CENTER = 400;

String receivedMessage, command;
char receivedChar;
int comma;
double x, y;

void setup() {
  //-------TODO-----
  // Spin motor setup
  
  // Start serial communication
  Serial.begin(9600);
  // Uno
  Serial1.begin(115200);
}

void loop() {

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
    if (x < DEADZONE_MAX && x > DEADZONE_MIN) {
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
}
