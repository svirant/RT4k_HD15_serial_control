/*
* RT4K Generic HDMI Serial Auto-switcher v0.01
* Copyright (C) 2025 @Donutswdad
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

// RT4K HD15 serial remote control w/ Arduino Nano
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin (not needed for this project, you can leave disconnected)
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

uint8_t const SVS = 1;     // 0 - use "Remote" button profiles 1-5
                           // 1 - use SVS profiles S1_<whatever>.rt4 - S5_<whatever>.rt4.
                           //     Ex: when combined with an offset of say "offset = 300" as an example, this would translate to SVS profiles S301_<whatever>.rt4 - S305_<whatever>.rt4

uint16_t const offset = 0; // by default SVS profiles 1 - 5 will be used. set the offset to add this number so they dont conflict with another SVS device.
                           // Ex: offset = 300 would give you SVS profiles 301-305.

bool HDMIOffProfile = false; //set to "true" loads Remote Profile 12 when all HDMI inputs are off. You can assign it to a generic HDMI input profile for example.
                              //set to "false" always leaves the last active HDMI input profile loaded
                              //set to "false" by default
                              
uint16_t HDMI1 = 0x0f; //used to store state of first grouping of HDMI ports
uint8_t HDMIoff = 1; //used to keep track of HDMI off profile state, if enabled
uint8_t HDMIoffprev = 1; //used to store previous state of HDMIoff
uint16_t HDMI1prev = 0x0f; //used to store previous state of first group


void setup(){
    
    Serial.begin(9600); // Set the baud rate for the RT4K Serial Connection
    while(!Serial){;}   // allow connection to establish before continuing
    Serial.println(F("\r")); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                          // sending a carriage return clears out the buffer so the first input change works correctly
    DDRD  &= ~B11111100; // Set PD2-7 as inputs (shown on nano as pins D2-D7)
    
} // end of setup()

void loop(){

readHDMI();

delay(250);

} // end of loop()

void readHDMI(){
    // Read HDMI ports (bitwise NOT results in a 1 when HDMI port is Active. AKA low voltage on LED cathode leg.
    //
    // If you are wanting to use this for a different application where an Active port is High instead of Low,
    // replace this following with this instead: (removing the ~)
    //
    // HDMI1 = (PIND & B11111100);
    //
    
    // Read HDMI ports (bitwise NOT results in a 1 when HDMI port is active)
    HDMI1 = ~(PIND & B11111100); //read pins D2-D7
    HDMIoff = !(HDMI1 & B11111100);

    // Has active HDMI port changed? Group 1
    if(HDMI1 != HDMI1prev){
      //Detect which HDMI port is now active and change profile accordingly
      if(HDMI1 & B01000000){ //Pin D2
        if(SVS) sendSVS(5);
        else Serial.println(F("remote prof5\r"));
      }
      else if(HDMI1 & B00100000){ //Pin D3
        if(SVS) sendSVS(4);
        else Serial.println(F("remote prof4\r"));
      }
      else if(HDMI1 & B00010000){ //Pin D4
        if(SVS) sendSVS(3);
        else Serial.println(F("remote prof3\r"));
      }
      else if(HDMI1 & B00001000){ //Pin D5
        if(SVS) sendSVS(2);
        else Serial.println(F("remote prof2\r"));
      }
      else if(HDMI1 & B00000100){ //Pin D6
        if(SVS) sendSVS(1);
        else Serial.println(F("remote prof1\r"));
      }
      else if(HDMI1 & B10000000){ //Pin D7
        if(SVS) sendSVS(6);
        else Serial.println(F("remote prof6\r"));
      }
      HDMI1prev = HDMI1;
    }
    
    // If all ports are in-active, load profile 12. You can assign it to a generic HDMI profile for example.
    if((HDMIoff != HDMIoffprev) && HDMIOffProfile){
      if((HDMI1 & B11111100) == 0){
        if(SVS) sendSVS(0);
        else Serial.println(F("remote prof12\r"));
      }
        
      HDMIoffprev = HDMIoff;
    }
} // end of readHDMI()

void sendSVS(uint16_t num){
  Serial.print(F("\rSVS NEW INPUT="));
  if(num != 0)Serial.print(num + offset);
  else Serial.print(num);
  Serial.println(F("\r"));
  delay(1000);
  Serial.print(F("\rSVS CURRENT INPUT="));
  if(num != 0)Serial.print(num + offset);
  else Serial.print(num);
  Serial.println(F("\r"));
}