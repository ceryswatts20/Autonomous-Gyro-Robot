const int LATERAL_POT = A0;
const int SPIN_POT = A1;

int prev_lat_val, prev_spin_val, lat_pot, spin_pot;
void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read potentiometer values
  lat_pot = analogRead(LATERAL_POT);
  spin_pot = analogRead(SPIN_POT);
  
  if (lat_pot != prev_lat_pot) {
    // Save new value
    prev_lat_pot = lat_pot;
    Serial.print("Lateral Potentiometer = " + String(lat_pot));
  }
  if (spin_pot != prev_spin_pot) {
    // Save new value
    prev_spin_pot = spin_pot;
    Serial.print("Dome Spin Potentiometer = " + String(spin_pot));
  }                
}
