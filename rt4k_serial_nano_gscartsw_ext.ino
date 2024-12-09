// RT4K HD15 serial remote control w/ Arduino Nano
//
// DISCLAIMER: Do not connect usb power to the Arduino AFTER it's wired in unless first disconnecting either EXT pin 4 (5v) or VIN on the Arduino.
//             Please Upload/Program the Arduino BEFORE connecting it to the EXT pins.
//
//
// HD15 pin 15 = RX --> Arduino Nano TX1 pin
// HD15 pin 12 = TX --> Arduino Nano RX0 pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)
//
// Connect gscartsw / gcompsw EXT pins 5,6,7 to pins D5,6,7 on the Arduino Nano
// You can also connect EXT pin 1 (Gnd) and EXT pin 4 (5v) to GND and VIN on the Arduino Nano for power/gnd.
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

#define IR_SEND_PIN 11  // Optional IR LED for RT5X compatibility. Sends IR data out Arduino pin D11
#include <IRremote.h>
IRsend irsend;
uint16_t scart1 = 0x0f; //used to store state of first grouping of scart ports
uint16_t scart1prev = 0x0f; //used to store previous state of first group

/*
////////////////////
//    OPTIONS    //
//////////////////
*/

bool RT5Xir = true;       // sends Profile 1 - 8 commands to RetroTink 5x. Must have IR LED connected as shown in README to work
                          // recommend placing the LED as close as you can (taping in front of) to the RT5X sensor for a more reliable connection
                          // OK to leave true even if nothing is connected

int SVS = 0; // 0 - use only "remote" profiles (default)
             // 1 - use SVS profiles instead. Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu, a requirement for using options 1 or 2

//////////////////   


void setup() {

    Serial.begin(9600); // Set the baud rate for the RT4K Serial Connection
    Serial.println("\r"); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                          // sending a carriage return clears out the buffer so the first input change works correctly
    DDRD  &= ~B11100000; // Set PD5-7 as inputs (shown on nano as pins D5-D7)

}

void loop() {

    scart1 = (PIND & B11100000); //read state of pins D5,D6,D7 (IN_BIT0, IN_BIT1, IN_BIT2)

    // Has active scart port changed? Group 1
    if(scart1 != scart1prev){
      //Detect which scart port is now active and change profile accordingly
      if(!(scart1 ^ B00000000)){
        if(SVS==0)Serial.println("remote prof1\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=1\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=1\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 1 
      } 
      else if(!(scart1 ^ B00100000)){
        if(SVS==0)Serial.println("remote prof2\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=2\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=2\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 2
      }
      else if(!(scart1 ^ B01000000)){
        if(SVS==0)Serial.println("remote prof3\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=3\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=3\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0xCC,2);delay(30);} // RT5X profile 3
      }
      else if(!(scart1 ^ B01100000)){
        if(SVS==0)Serial.println("remote prof4\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=4\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=4\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8E,2);delay(30);} // RT5X profile 4
      }
      else if(!(scart1 ^ B10000000)){
        if(SVS==0)Serial.println("remote prof5\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=5\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=5\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8F,2);delay(30);} // RT5X profile 5
      } 
      else if(!(scart1 ^ B10100000)){
        if(SVS==0)Serial.println("remote prof6\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=6\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=6\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0xC8,2);delay(30);} // RT5X profile 6
      }   
      else if(!(scart1 ^ B11000000)){
        if(SVS==0)Serial.println("remote prof7\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=7\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=7\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8A,2);delay(30);} // RT5X profile 7
      } 
      else if(!(scart1 ^ B11100000)){
        if(SVS==0)Serial.println("remote prof8\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=8\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=8\r");
          }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8B,2);delay(30);} // RT5X profile 8
      }
        

      scart1prev = scart1;
    }


    delay(500);
}
