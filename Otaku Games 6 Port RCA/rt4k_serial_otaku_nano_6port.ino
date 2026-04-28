/*
* RT4K Otaku 6 Port Serial Auto-switcher v0.1a
* Copyright (C) 2026 @Donutswdad
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

uint8_t const SVS = 1;     // 0 - use "Remote" button profiles 1-6(must be set to this if going directly into a Donut Dongle SW1 or SW2 port)
                           // 1 - use SVS profiles S1_<whatever>.rt4 - S6_<whatever>.rt4.
                           //     Ex: when combined with an offset of say "offset = 300" as an example, this would translate to SVS profiles S301_<whatever>.rt4 - S306_<whatever>.rt4

uint16_t const offset = 0; // by default SVS profiles 1 - 6 will be used. set the offset to add this number so they dont conflict with another SVS device.
                           // Ex: offset = 300 would give you SVS profiles 301-305.

bool OffProfile = true; // set to "true" loads either S0_<whatever>.rt4 or Remote Button Profile 12 when all OTAKU inputs are off. This depends on the SVS setting above. You can assign it to a generic OTAKU input profile for example.
                        // "false" always leaves the last active OTAKU input profile loaded
                        // "true" by default
                              
uint16_t OTAKU1 = 0x0f; //used to store state of first grouping of OTAKU ports
uint8_t OTAKUoff = 1; //used to keep track of OTAKU off profile state, if enabled
uint8_t OTAKUoffprev = 1; //used to store previous state of OTAKUoff
uint16_t OTAKU1prev = 0x0f; //used to store previous state of first group


void setup(){
    
    Serial.begin(9600); // Set the baud rate for the RT4K Serial Connection
    while(!Serial){;}   // allow connection to establish before continuing
    Serial.println(F("\r")); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                          // sending a carriage return clears out the buffer so the first input change works correctly
    DDRD  &= ~B11111100; // Set PD2-7 as inputs (shown on nano as pins D2-D7)
    
} // end of setup()

void loop(){

readOTAKU();

delay(250);

} // end of loop()

void readOTAKU(){
    // Read OTAKU ports (bitwise NOT results in a 1 when OTAKU port is Active. AKA low voltage on LED cathode leg.
    OTAKU1 = ~(PIND & B11111100); //read pins D2-D7
    OTAKUoff = !(OTAKU1 & B11111100);

    // Has active OTAKU port changed? Group 1
    if(OTAKU1 != OTAKU1prev){
      //Detect which OTAKU port is now active and change profile accordingly
      if(OTAKU1 & B00000100){ //Pin D2
        if(SVS) sendSVS(1);
        else Serial.println(F("remote prof1\r"));
      }
      else if(OTAKU1 & B00001000){ //Pin D3
        if(SVS) sendSVS(2);
        else Serial.println(F("remote prof2\r"));
      }
      else if(OTAKU1 & B00010000){ //Pin D4
        if(SVS) sendSVS(3);
        else Serial.println(F("remote prof3\r"));
      }
      else if(OTAKU1 & B00100000){ //Pin D5
        if(SVS) sendSVS(4);
        else Serial.println(F("remote prof4\r"));
      }
      if(OTAKU1 & B01000000){ //Pin D6
        if(SVS) sendSVS(5);
        else Serial.println(F("remote prof5\r"));
      }
      else if(OTAKU1 & B10000000){ //Pin D7
        if(SVS) sendSVS(6);
        else Serial.println(F("remote prof6\r"));
      }
      OTAKU1prev = OTAKU1;
    }
    
    // If all ports are in-active, send SVS profile 0 or button profile 12. You can assign it to a generic HDMI profile for example.
    if((OTAKUoff != OTAKUoffprev) && OffProfile){
      if((OTAKU1 & B11111100) == 0){
        if(SVS) sendSVS(0);
        else Serial.println(F("remote prof12\r"));
      }
        
      OTAKUoffprev = OTAKUoff;
    }
} // end of readOTAKU()

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
