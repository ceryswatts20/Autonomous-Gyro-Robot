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
				byte x, y;

				// D-Pad
        // Up
				if (psx.buttonJustPressed(PSB_PAD_UP)) {
          Serial.println("dup");
        }
        if (psx.buttonJustReleased(PSB_PAD_UP)) {
          Serial.println("dur");
        }
        // Down
				if (psx.buttonJustPressed(PSB_PAD_DOWN)) {
          Serial.println("ddp");
				}
        if (psx.buttonJustReleased(PSB_PAD_DOWN)) {
          Serial.println("ddr");
				}
        // Left
				if (psx.buttonJustPressed(PSB_PAD_LEFT)) {
          Serial.println("dlp");
				}
        if (psx.buttonJustReleased(PSB_PAD_LEFT)) {
          Serial.println("dlr");
				}
        // Right
        if (psx.buttonJustPressed(PSB_PAD_RIGHT)) {
          Serial.println("drp");
				}
        if (psx.buttonJustReleased(PSB_PAD_RIGHT)) {
          Serial.println("drr");
				}

				// Left stick
        // Get current position
        psx.getLeftAnalog(x, y);
        // If there has been movement
				if (x != prev_lx || y != prev_ly) {
					Serial.print("lx=");
          Serial.print(x);
          Serial.print(",ly=");
          Serial.println(y);
				}
        // Save current analogue stick position
        prev_lx = x;
        prev_ly = y;
        // Right stick
        // Get current position
        psx.getRightAnalog(x, y);
        // If there has been lateral movement
				if (x != prev_rx || y != prev_ry) {
          Serial.print("rx=");
          Serial.print(x);
          Serial.print(",ry=");
          Serial.println(y);
				}
        // Save current analogue stick positon
        prev_rx = x;
        prev_ry = y;

        // Buttons
        // Triangle
        if (psx.buttonJustPressed(PSB_TRIANGLE)) {
          Serial.println("tp");
        }
        if (psx.buttonJustReleased(PSB_TRIANGLE)) {
          Serial.println("tr");
        }
        // Square
        if (psx.buttonJustPressed(PSB_SQUARE)) {
          Serial.println("sp");
        }
        if (psx.buttonJustReleased(PSB_SQUARE)) {
          Serial.println("sr");
        }
        // X
        if (psx.buttonJustPressed(PSB_CROSS)) {
          Serial.println("xp");
        }
        if (psx.buttonJustReleased(PSB_CROSS)) {
          Serial.println("xr");
        }
        // Circle
        if (psx.buttonJustPressed(PSB_CIRCLE)) {
          Serial.println("cp");
        }
        if (psx.buttonJustReleased(PSB_CIRCLE)) {
          Serial.println("cr");
        }
        // R1
        if (psx.buttonJustPressed(PSB_R1)) {
          Serial.println("R1p");
        }
        if (psx.buttonJustReleased(PSB_R1)) {
          Serial.println("R1r");
        }
        // L1
        if (psx.buttonJustPressed(PSB_L1)) {
          Serial.println("L1p");
        }
        if (psx.buttonJustReleased(PSB_L1)) {
          Serial.println("L1r");
        }
        // R2
        if (psx.buttonJustPressed(PSB_R2)) {
          Serial.println("R2p");
        }
        if (psx.buttonJustReleased(PSB_R2)) {
          Serial.println("R2r");
        }
        // L2
        if (psx.buttonJustPressed(PSB_L2)) {
          Serial.println("L2p");
        }
        if (psx.buttonJustReleased(PSB_L2)) {
          Serial.println("L2r");
        }
        // R3
        if (psx.buttonJustPressed(PSB_R3)) {
          Serial.println("R3p");
        }
        if (psx.buttonJustReleased(PSB_R3)) {
          Serial.println("R3r");
        }
        // L3
        if (psx.buttonJustPressed(PSB_L3)) {
          Serial.println("L3p");
        }
        if (psx.buttonJustReleased(PSB_L3)) {
          Serial.println("L3r");
        }
        // Start
        if (psx.buttonJustPressed(PSB_START)) {
          Serial.println("stp");
        }
        if (psx.buttonJustReleased(PSB_START)) {
          Serial.println("str");
        }
        // Select
        if (psx.buttonJustPressed(PSB_SELECT)) {
          Serial.println("sep");
        }
        if (psx.buttonJustReleased(PSB_SELECT)) {
          Serial.println("ser");
        }
			}
		}
	}
}
