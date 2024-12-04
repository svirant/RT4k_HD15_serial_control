// RT4K HD15 serial remote control w/ Arduino Nano edition (5v tolerant)
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 4
String einput; // used to store state of Extron input
String preveinput; // used to keep track of previous input
SoftwareSerial extronSerial = SoftwareSerial(rxPin,txPin); // setup an additional serial port for listening to the Extron


void setup()  {

    pinMode(rxPin,INPUT); // set pin modes for RX and TX
    pinMode(txPin,OUTPUT);
        
    Serial.begin(9600); // set the baud rate for the RT4K Serial Connection
    extronSerial.begin(9600); // set the baud rate for the Extron Connection
    extronSerial.setTimeout(50); // sets the timeout for reading / saving reads into a string
    
}

void loop() {

    // listen to the Extron Serial Port for input changes
    if(extronSerial.available() > 0){
      einput = extronSerial.readString().substring(0,3); //read in and store only the first 3 chars for every Extron serial status received

      // filter out unstable Extron connections and other status messages to prevent the same profile change being sent multiple times
      // use remaining results to see which input is now active and change profile accordingly
      if(preveinput != einput && einput.substring(0,2) == "In" && einput != "In0"){
        if(einput == "In1"){
          Serial.println("remote prof1\r");
        }
        else if(einput == "In2"){
          Serial.println("remote prof2\r");
        }
        else if(einput == "In3"){
          Serial.println("remote prof3\r");
        }
        else if(einput == "In4"){
          Serial.println("remote prof4\r");
        }
        else if(einput == "In5"){
          Serial.println("remote prof5\r");
        }
        else if(einput == "In6"){
          Serial.println("remote prof6\r");
        }
        
          preveinput = einput;
      }
    }

    
   delay(250);
}    
