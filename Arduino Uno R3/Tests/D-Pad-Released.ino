#include <PsxControllerBitBang.h>

const byte PIN_PS2_ATT = 10;
const byte PIN_PS2_CMD = 11;
const byte PIN_PS2_DAT = 12;
const byte PIN_PS2_CLK = 9;

const unsigned long POLLING_INTERVAL = 1000U / 50U;

PsxControllerBitBang<PIN_PS2_ATT, PIN_PS2_CMD, PIN_PS2_DAT, PIN_PS2_CLK> psx;

bool haveController = false;

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
				byte x, y;

				// D-Pad
        // Up
				if (psx.buttonJustReleased(PSB_PAD_UP)) {
          Serial.println("dur");
        }
        // Down
				if (psx.buttonJustReleased(PSB_PAD_DOWN)) {
          Serial.println("ddr");
				}
        // Left
				if (psx.buttonJustReleased(PSB_PAD_LEFT)) {
          Serial.println("dlr");
				}
        // Right
        if (psx.buttonJustReleased(PSB_PAD_RIGHT)) {
          Serial.println("drr");
				}
      }
    }
  }
}
