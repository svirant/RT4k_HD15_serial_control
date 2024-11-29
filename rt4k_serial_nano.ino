// RT4K HD15 serial remote control w/ Arduino Nano edition (5v tolerant)
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin

uint16_t scart1 = 0x0f; //used to store state of first grouping of scart ports
uint16_t scart2 = 0x0f; //second grouping...
uint16_t scart1prev = 0x0f; //used to store previous state of first group
uint16_t scart2prev = 0x0f; 


void setup() {

    Serial.begin(9600); // Set the baud rate for the RT4K Serial Connection
    DDRD  &= ~B11111100; // Set PD2-7 as inputs (shown on nano as pins D2-D7)
    PORTD |=  B11111100; // Enable internal pull-up resistors
    DDRB  &= ~B00011111; // Set PB0 - PB4 as inputs (shown on nano as pins D8-12)
    PORTB |=  B00011111; // Enable internal pull-up resistors

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
    
    scart1 = ~(PIND & B11111100);
    scart2 = ~(PINB & B00011111);

    // Has active scart port changed? Group 1
    if(scart1 != scart1prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart1 & B00000100) //Pin D2
        Serial.println("remote prof11\r");
      else if(scart1 & B00001000) //Pin D3
        Serial.println("remote prof10\r");
      else if(scart1 & B00010000) //Pin D4
        Serial.println("remote prof9\r");
      else if(scart1 & B00100000) //Pin D5
        Serial.println("remote prof8\r");
      else if(scart1 & B01000000) //Pin D6
        Serial.println("remote prof7\r");
      else if(scart1 & B10000000) //Pin D7
        Serial.println("remote prof6\r");

      scart1prev = scart1;
    }
    
    // Has active scart port changed? Group 2
    if(scart2 != scart2prev){
      //Detect which scart port is now active and change profile accordingly
      if(scart2 & B00000001) //Pin D8
        Serial.println("remote prof5\r");
      else if(scart2 & B00000010) //Pin D9
        Serial.println("remote prof4\r");
      else if(scart2 & B00000100) //Pin D10
        Serial.println("remote prof3\r");
      else if(scart2 & B00001000) //Pin D11
        Serial.println("remote prof2\r");
      else if(scart2 & B00010000) //Pin D12
        Serial.println("remote prof1\r");

      scart2prev = scart2;
    }  

    delay(500);
}
