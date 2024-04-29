#include <Servo.h>

// Define pins
// Head tilt servo
const int HEAD_TILT = 6;
// Head pan servo
const int HEAD_PAN = 11;
// Head spin servo
const int HEAD_SPIN = 7;

// Head tilt servo
Servo headTilt;
// Head pan servo
Servo headPan;
// Head spin servo
Servo headSpin;

String receivedMessage, command, data;
int comma, angle;
double x, y, inverse_x, inverse_y, x_percent, y_percent;
char receivedChar;

const int DEADZONE_MAX = 150;
const int DEADZONE_MIN = 100;
const int H_TILT_START = 115;
const int H_PAN_START = 90;

void setup() {
  // Attach servos to their pins
  headTilt.attach(HEAD_TILT);
  headPan.attach(HEAD_PAN);
  headSpin.attach(HEAD_SPIN);
  // Stop servo
  headSpin.write(95);
  // Initialize servo positions
  headTilt.write(H_TILT_START);
  headPan.write(H_PAN_START);

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
}
