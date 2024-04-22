String receivedMessage = "";
char receivedChar = '';

void setup() {
  // Start serial communication
  Serial.begin(9600);
  // Uno
  Serial1.begin(115200);
}

void loop() {
  // If there is data waiting
  while (Serial1.available() > 0) {
    // Read 1 character
    receivedChar = Serial1.read();
    if (receivedChar == '\n') {
      Serial.println(receivedMessage);
      // Reset the received message
      receivedMessage = "";
    } 
    else {
      // Append characters to the received message
      receivedMessage += receivedChar;  
    }
  }
}
