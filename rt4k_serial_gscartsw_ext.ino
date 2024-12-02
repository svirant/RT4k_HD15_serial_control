//
// EXPERIMENTAL - NOT CONFIRMED WORKING
//
// RT4K HD15 serial remote control w/ Arduino Pro Micro (not 5v tolerant, unless voltage divider is used)
//
// HD15 pin 15 = RX --> Arduino Pro Micro TX pin
// HD15 pin 12 = TX --> Arduino Pro Micro RX pin
//
// Connect gscartsw / gcompsw EXT pin 5 ---> Pin A2 (Arduino Pro Micro)
//                            EXT pin 6 ---> Pin A1 (Arduino Pro Micro)
//                            EXT pin 7 ---> Pin A0 (Arduino Pro Micro)
//
// You can also connect EXT pin 1 (Gnd) and EXT pin 4 (5v) to GND and RAW on the Arduino Nano for Power/Gnd.
//
// https://shmups.system11.org/viewtopic.php?p=1307320#p1307320
// gscartsw_lite EXT pinout:
// Pin 1: GND
// Pin 2: Override
// Pin 3: N/C
// Pin 4: +5V
// Pin 5: IN_BIT0
// Pin 6: IN_BIT1
// Pin 7: IN_BIT2
// Pin 8: N/C
//
// Pin 5-7 represents selected input in binary. 
//
// For the gscartsw binary representation above, it is "Assumed" that low voltage on an EXT pin is a 0 and high voltage is a 1
//

uint16_t scart1 = 0x0f; //used to store state of first grouping of scart ports
uint16_t scart1prev = 0x0f; //used to store previous state of first group

void setup() {

    Serial1.begin(9600); // Set the baud rate for the RT4K Serial Connection
    Serial.begin(9600); // Set the baud rate for the Terminal Monitor (debug)
    DDRF  &= ~B11100000; // Set pins A0-A2 as inputs, aka PF7-5
    //PORTF |=  B11100000; // Enable internal pull-up resistors
}

void loop() {

    // If you are wanting to use this for a different application where an Active pin is Low instead of High,
    // replace this following with this instead: (adding the ~)
    //
    // scart1 = ~(PINF & B11100000);
    //
    // Also comment out the PORTF line above to disable the internal pull-up resistors.
    
    scart1 = (PINF & B11100000);

    // Has active scart port changed? Group 1
    if(scart1 != scart1prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart1 & B00100000){
        Serial1.println("remote prof2\r");
        Serial.println("remote prof2\r");
      }
      else if(scart1 & B01000000){
        Serial1.println("remote prof3\r");
        Serial.println("remote prof3\r");
      }
      else if(scart1 & B01100000){
        Serial1.println("remote prof4\r");
        Serial.println("remote prof4\r");
      }
      else if(scart1 & B10000000){
        Serial1.println("remote prof5\r");
        Serial.println("remote prof5\r");
      }
      else if(scart1 & B10100000){
        Serial1.println("remote prof6\r");
        Serial.println("remote prof6\r");
      }
      else if(scart1 & B11000000){
        Serial1.println("remote prof7\r");
        Serial.println("remote prof7\r");
      }
      else if(scart1 & B11100000){
        Serial1.println("remote prof8\r");
        Serial.println("remote prof8\r");
      }
      else{
        Serial1.println("remote prof1\r");
        Serial.println("remote prof1\r");
      }      

      scart1prev = scart1;
    }
    
    delay(500);
}
