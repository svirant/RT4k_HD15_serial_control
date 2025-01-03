// RT4K HD15 serial remote control w/ Arduino Pro Micro (not 5v tolerant, unless voltage divider is used)
//
// DISCLAIMER: Do not connect usb power to the Arduino (AFTER it's wired in) and the switch at the same time. If Programming the Arduino, please make sure the Switch's USB power cable is first disconnected.
//
// HD15 pin 15 = RX --> Arduino Pro Micro TX pin
// HD15 pin 12 = TX --> Arduino Pro Micro RX pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

bool ScartOffProfile = false; //set to "true" loads Remote Profile 12 when all scart inputs are off. You can assign it to a generic HDMI input profile for example.
                              //set to "false" always leaves the last active scart input profile loaded
                              //set to "false" by default
                              
uint16_t scart1 = 0x0f; //used to store state of first grouping of scart ports
uint16_t scart2 = 0x0f; //second grouping...
uint16_t scart3 = 0x0f; //third grouping...
int scartoff = 1; //used to keep track of scart off profile state
int scartoffprev = 1; //used to store previous state of scartoff
uint16_t scart1prev = 0x0f; //used to store previous state of first group
uint16_t scart2prev = 0x0f; 
uint16_t scart3prev = 0x0f;

void setup() {

    Serial1.begin(9600); // Set the baud rate for the RT4K Serial Connection
    Serial1.println("\r"); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                           // sending a carriage return clears out the buffer so the first input change works correctly
    Serial.begin(9600); // Set the baud rate for the Terminal Monitor (debug)
    DDRD  &= ~B10010011; // Set PD0,1,4,7 as inputs (shown on pro micro as pins 3,2,4,6)
    PORTD |=  B10010011; // Enable internal pull-up resistors
    DDRF  &= ~B11110000; // Set pins A0-A3 as inputs
    PORTF |=  B11110000; // Enable internal pull-up resistors
    DDRB  &= ~B00010010; // Set PB1 and PB4 as inputs (shown on pro micro as pins 15,8)
    PORTB |=  B00010010; // Enable internal pull-up resistors
}

void loop() {

    // Read scart ports (bitwise NOT results in a 1 when scart port is Active / Low voltage on uln2003 leg)
    //
    // If you are wanting to use this for a different application where an Active port is High instead of Low,
    // replace this following with this instead: (removing the ~)
    //
    // scart1 = (PIND & B10010011);
    // scart2 = (PINF & B11110000);
    // scart3 = (PINB & B00010010);
    //
    // Also comment out the PORTD, PORTF, PORTB lines above to disable the internal pull-up resistors.
    
    scart1 = ~(PIND & B10010011); //read pins 3,2,4,6 
    scart2 = ~(PINF & B11110000); //read pins A3,A2,A1,A0
    scart3 = ~(PINB & B00010010); //read pins 15,8
    scartoff = (!(scart1 & B10010011) && !(scart2 & B11110000) && !(scart3 & B00010010));

    // Has active scart port changed? Group 1
    if(scart1 != scart1prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart1 & B0000010){ //Pin 2
        Serial1.println("remote prof1\r");
        Serial.println("remote prof1\r");
      }
      else if(scart1 & B00000001){ //Pin 3
        Serial1.println("remote prof2\r");
        Serial.println("remote prof2\r");
      }
      else if(scart1 & B00010000){ //Pin 4
        Serial1.println("remote prof3\r");
        Serial.println("remote prof3\r");
      }
      else if(scart1 & B10000000){ //Pin 6
        Serial1.println("remote prof4\r");
        Serial.println("remote prof4\r");
      }

      scart1prev = scart1;
    }
    
    // Has active scart port changed? Group 2
    if(scart2 != scart2prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart2 & B00010000){ //Pin A3
        Serial1.println("remote prof10\r");
        Serial.println("remote prof10\r");
      }
      else if(scart2 & B00100000){ //Pin A2
        Serial1.println("remote prof9\r");
        Serial.println("remote prof9\r");
      }
      else if(scart2 & B01000000){ //Pin A1
        Serial1.println("remote prof8\r");
        Serial.println("remote prof8\r");
      }
      else if(scart2 & B10000000){ //Pin A0
        Serial1.println("remote prof7\r");
        Serial.println("remote prof7\r");
      }

      scart2prev = scart2;
    }  

    // Has active scart port changed? Group 3
    if(scart3 != scart3prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart3 & B00010000){ //Pin 8
        Serial1.println("remote prof5\r");
        Serial.println("remote prof5\r");
      }
      else if(scart3 & B00000010){ //Pin 15
        Serial1.println("remote prof6\r");
        Serial.println("remote prof6\r");
      }

      scart3prev = scart3;
    }


    // If all ports are in-active, load profile 12. You can assign it to a generic HDMI profile for example.
    if((scartoff != scartoffprev) && ScartOffProfile){
      if((scart1 & B10010011) + (scart2 & B11110000) + (scart3 & B00010010) == 0){
        Serial1.println("remote prof12\r");
        Serial.println("remote prof12\r");
      }
      scartoffprev = scartoff;
    }


    delay(500);
}
