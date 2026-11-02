/*
* RT4K Serial Generic HDMI Auto-switcher
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* USER CONFIGURATION START */

// You will need to change the switch statement in line 80, where you might need to add or remove cases depending on which pins you're using.
// Additionally, you will need to change the two variables below. If you like S0, you can set it up as well.

const int pinLowerBound = 0; // This pin is the lower bound. If your Nano's input pins are set up to start from D2, this value should be 2.
const int pinUpperBound = 0; // This pin is the upper bound. If your Nano's input pins end at D11, this value would be 11.
// Note: if your pins are set up incorrectly, you may have some noise. Be sure to set these up exactly like you plan on having them on the Nano.
uint8_t const SVS = 0; // 0 - use "Remote button profiles"
                       // 1 - use SVS profiles (S1_<blah>.rt4)
uint16_t const offset = 0; // by default SVS profiles 1 - 10 will be used. set the offset to add this number so they dont conflict with another SVS device.
                           // Ex: offset = 600 would give you SVS profiles 601-610.

/* USER CONFIGURATION END*/

// Variable to store the last active pin (initialized to 0, meaning nothing was active)
int lastActivePin = -1;
int profileNum = 0;

void sendSVS(uint16_t num){
  Serial.print(F("\rSVS NEW INPUT="));
  Serial.print(num + offset);
  Serial.println(F("\r"));
  delay(1000);
  Serial.print(F("\rSVS CURRENT INPUT="));
  Serial.print(num + offset);
  Serial.println(F("\r"));
}

void setup() {
  // Initialize serial communication at 9600 bps for RetroTINK 4K
  Serial.begin(9600);
  // Wait for the serial port to connect
  while (!Serial) { ; }
  
  // Send an initial carriage return to clear the RetroTINK buffer 
  // and clear any garbage bytes generated during power-on
  Serial.println(F("\r"));
  
  // Configure digital pins from pinLowerBound through pinUpperBound as standard INPUTs.
  for (int pin = pinLowerBound; pin <= pinUpperBound; pin++) {
    pinMode(pin, INPUT);
  }
}

void loop() {
  int currentActivePin = 0;

  // Scan pins starting from pinUpperBound down to pinLowerBound to check for LOW states.
  // This order enforces priority if multiple pins happen to be LOW simultaneously.

  for (int pin = pinUpperBound; pin >= pinLowerBound; pin--) {
    if (digitalRead(pin) == HIGH) {
      currentActivePin = pin;
      break; // Found the active pin, exit the scan loop
    }
  }

  // Check if the active pin has changed since the last cycle
  if (currentActivePin != lastActivePin) {
    
    if (currentActivePin != 0) {
      switch (currentActivePin) {
      // This is a sample configuration and may not work with your setup. Please read the README for more instructions.
        case 2: 
          profileNum = 1;
          break;
        case 3:
          profileNum = 2; 
          break;
        case 6:
          profileNum = 3; 
          break;
        case 7:
          profileNum = 4; 
          break;
      }
      // Send the exact matched RetroTINK profile string format
      if(SVS){
        sendSVS(profileNum);
      } else {
        Serial.print(F("remote prof"));
        Serial.print(profileNum);
        Serial.println(F("\r"));
      } 
    } 

    // Save the new current state to block redundant transmissions
    lastActivePin = currentActivePin;
  }

  // Small delay to debounce inputs and prevent rapid loop spinning
  delay(100);
}