String receivedMessage = "";
char receivedChar = '';

void setup() {
  // Start serial communication
  Serial.begin(9600);
  // Nano
  Serial2.begin(115200);
}

void loop() {
  // If there is data waiting
  while (Serial2.available() > 0) {
    // Read 1 character
    receivedChar = Serial2.read();
    if (receivedChar == '\n') {
      Serial.println(receivedMessage);
      // Reset the variable
      receivedMessage = "";
    }
    else {
      // Append the character to the rest
      receivedMessage += receivedChar;
    }
  }
}
