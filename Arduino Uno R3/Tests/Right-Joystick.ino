#include <PsxControllerBitBang.h>

const byte PIN_PS2_ATT = 10;
const byte PIN_PS2_CMD = 11;
const byte PIN_PS2_DAT = 12;
const byte PIN_PS2_CLK = 9;

const unsigned long POLLING_INTERVAL = 1000U / 50U;

PsxControllerBitBang<PIN_PS2_ATT, PIN_PS2_CMD, PIN_PS2_DAT, PIN_PS2_CLK> psx;

bool haveController = false;

void setup () {
  Serial.begin(115200);
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

        // Get current position
        psx.getRightAnalog(x, y);
        Serial.print(x);
        Serial.print(",");
        Serial.println(y);
      }
    }
  }
}
