#include <PsxControllerBitBang.h>

const byte PIN_PS2_ATT = 10;
const byte PIN_PS2_CMD = 11;
const byte PIN_PS2_DAT = 12;
const byte PIN_PS2_CLK = 9;

const unsigned long POLLING_INTERVAL = 1000U / 50U;

PsxControllerBitBang<PIN_PS2_ATT, PIN_PS2_CMD, PIN_PS2_DAT, PIN_PS2_CLK> psx;

bool haveController = false;
// Initialise analogue stick start positions
byte prev_lx = 128;
byte prev_ly = 128;
byte prev_rx = 128;
byte prev_ry = 128;

void setup () {
  // Initialise UART for sending data
  Serial.begin(115200);
  Serial.println("Connection started");
}

void loop () {
	static unsigned long last = 0;
	
	if (millis () - last >= POLLING_INTERVAL) {
		last = millis ();
		
		if (!haveController) {
			if (psx.begin ()) {
				Serial.println("Controller found!");
				if (!psx.enterConfigMode ()) {
					Serial.println("Cannot enter config mode");
				} else {
					// Try to enable analog sticks
					if (!psx.enableAnalogSticks ()) {
						Serial.println("Cannot enable analog sticks");
					}
									
					if (!psx.exitConfigMode ()) {
						Serial.println("Cannot exit config mode");
					}
				}
				
				haveController = true;
			}
		} 
    else {
      // Poll controller
			if (!psx.read ()) {
				Serial.println("Controller lost :(");
				haveController = false;
			} 
      else {
        // Triangle
        if (psx.buttonJustReleased(PSB_TRIANGLE)) {
          Serial.println("tp");
        }
        // Square
        if (psx.buttonJustReleased(PSB_SQUARE)) {
          Serial.println("sp");
        }
        // X
        if (psx.buttonJustReleased(PSB_CROSS)) {
          Serial.println("xp");
        }
        // Circle
        if (psx.buttonJustReleased(PSB_CIRCLE)) {
          Serial.println("cp");
        }
        // R1
        if (psx.buttonJustReleased(PSB_R1)) {
          Serial.println("R1p");
        }
        // L1
        if (psx.buttonJustReleased(PSB_L1)) {
          Serial.println("L1p");
        }
        // R2
        if (psx.buttonJustReleased(PSB_R2)) {
          Serial.println("R2p");
        }
        // L2
        if (psx.buttonJustReleased(PSB_L2)) {
          Serial.println("L2p");
        }
        // R3
        if (psx.buttonJustReleased(PSB_R3)) {
          Serial.println("R3p");
        }
        // L3
        if (psx.buttonJustReleased(PSB_L3)) {
          Serial.println("L3p");
        }
        // Start
        if (psx.buttonJustReleased(PSB_START)) {
          Serial.println("stp");
        }
        // Select
        if (psx.buttonJustReleased(PSB_SELECT)) {
          Serial.println("sep");
        }
			}
		}
	}
}
