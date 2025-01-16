/*
* RT4K Serial Otaku Games Scart Auto-switcher - Arduino Nano edition
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

// RT4K HD15 serial remote control w/ Arduino Nano edition (5v tolerant)
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

bool ScartOffProfile = false; //set to "true" loads Remote Profile 12 when all scart inputs are off. You can assign it to a generic HDMI input profile for example.
                              //set to "false" always leaves the last active scart input profile loaded
                              //set to "false" by default
                              
uint16_t scart1 = 0x0f; //used to store state of first grouping of scart ports
uint16_t scart2 = 0x0f; //second grouping...
int scartoff = 1; //used to keep track of scart off profile state, if enabled
int scartoffprev = 1; //used to store previous state of scartoff
uint16_t scart1prev = 0x0f; //used to store previous state of first group
uint16_t scart2prev = 0x0f; 


void setup() {

    Serial.begin(9600); // Set the baud rate for the RT4K Serial Connection
    Serial.println("\r"); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                          // sending a carriage return clears out the buffer so the first input change works correctly
    DDRD  &= ~B11111100; // Set PD2-7 as inputs (shown on nano as pins D2-D7)
    PORTD |=  B11111100; // Enable internal pull-up resistors
    DDRB  &= ~B00001111; // Set PB0 - PB4 as inputs (shown on nano as pins D8-11)
    PORTB |=  B00001111; // Enable internal pull-up resistors
    
}

void loop() {
    // Read scart ports (bitwise NOT results in a 1 when scart port is Active / Low voltage on uln2003 leg)
    //
    // If you are wanting to use this for a different application where an Active port is High instead of Low,
    // replace this following with this instead: (removing the ~)
    //
    // scart1 = (PIND & B11111100);
    // scart2 = (PINB & B00011111);
    //
    // Also comment out the PORTD, PORTB lines above to disable the internal pull-up resistors.
    
    // Read scart ports (bitwise NOT results in a 1 when scart port is active)
    scart1 = ~(PIND & B11111100); //read pins D2-D7
    scart2 = ~(PINB & B00001111); //read pins D8-D11
    scartoff = (!(scart1 & B11111100) && !(scart2 & B00001111));

    // Has active scart port changed? Group 1
    if(scart1 != scart1prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart1 & B01000000) //Pin D2
        Serial.println("remote prof10\r");
      else if(scart1 & B00100000) //Pin D3
        Serial.println("remote prof9\r");
      else if(scart1 & B00010000) //Pin D4
        Serial.println("remote prof8\r");
      else if(scart1 & B00001000) //Pin D5
        Serial.println("remote prof7\r");
      else if(scart1 & B00000100) //Pin D6
        Serial.println("remote prof6\r");
      else if(scart1 & B10000000) //Pin D7
        Serial.println("remote prof5\r");

      scart1prev = scart1;
    }
    
    // Has active scart port changed? Group 2
    if(scart2 != scart2prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart2 & B00000001) //Pin D8
        Serial.println("remote prof4\r");
      else if(scart2 & B00000010) //Pin D9
        Serial.println("remote prof3\r");
      else if(scart2 & B00000100) //Pin D10
        Serial.println("remote prof2\r");
      else if(scart2 & B00001000) //Pin D11
        Serial.println("remote prof1\r");

      scart2prev = scart2;
    }

    // If all ports are in-active, load profile 12. You can assign it to a generic HDMI profile for example.
    if((scartoff != scartoffprev) && ScartOffProfile){
      if((scart1 & B11111100) + (scart2 & B00001111) == 0)
        Serial.println("remote prof12\r");
        
      scartoffprev = scartoff;
    }


    delay(500);
}
