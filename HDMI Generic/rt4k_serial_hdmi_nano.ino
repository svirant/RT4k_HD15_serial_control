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

// You will need to potentially change three things. All are highly dependent on how many inputs your switch has:
// Line 35, where you'll need to change the 2 and 8 in `for (int pin = 2; pin <=8; pin++)` to the pins that you're using. For example, if you're only using Pins D4-D8, it should be `for (int pin = 4; pin <=8; pin++)`, etc.
// Line 46, where you'll need to change for (int pin = 8; pin >=2; pin--). Same as above depending on the pins you're using.
// The switch statement in line 57, where you might need to add or remove cases depending on which pins you're using.

// Variable to store the last active pin (initialized to 0, meaning nothing was active)
int lastActivePin = 0;
int profileNum = 0;
uint8_t const SVS = 0; // 0 - use "Remote button profiles"
// 1 - use SVS profiles (S1_<blah>.rt4)
uint16_t const offset = 0; // by default SVS profiles 1 - 10 will be used. set the offset to add this number so they dont conflict with another SVS device.
                           // Ex: offset = 600 would give you SVS profiles 601-610.

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
  
  // Configure digital pins 2 through 8 as standard INPUTs. You might need to change this depending on which digital pins you use (D2 -> D12)
  for (int pin = 2; pin <= 7; pin++) {
    pinMode(pin, INPUT);
  }
}

void loop() {
  int currentActivePin = 0;

  // Scan pins starting from 8 down to 2 to check for HIGH states.
  // This order enforces priority if multiple pins happen to be HIGH simultaneously.
  // Once again, you might need to change this on which digital pins you used (D2 -> D12).
  for (int pin = 7; pin >= 2; pin--) {
    if (digitalRead(pin) == HIGH) {
      currentActivePin = pin;
      break; // Found the active pin, exit the scan loop
    }
  }

  // Check if the active pin has changed since the last cycle
  if (currentActivePin != lastActivePin) {
    
    if (currentActivePin != 0) {
      switch (currentActivePin) {

        // This is user-defined. You can tie each case statement to a profile number.
        case 2: // For example, 8 here refers to Pin D8.
          profileNum = 1; // And profileNum here refers to what remote profile / SVS profile to send. You can add to this switch statement to fit your own needs.
          break; // Maybe you soldered Input 2 to Pin D8, which would mean you'd need to swap profileNum = 1 to profileNum = 2. It's your life! Live it! Be free!
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
