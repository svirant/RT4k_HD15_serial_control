// RT4K HD15 serial remote control w/ Arduino Nano edition (5v tolerant)
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

#include <SoftwareSerial.h>
#define rxPin 3
#define txPin 4
String ecap; // used to store Extron status messages
String einput; // used to store first 4 chars of Extron input
String preveinput; // used to keep track of previous input
SoftwareSerial extronSerial = SoftwareSerial(rxPin,txPin); // setup an additional serial port for listening to the Extron


void setup()  {

    pinMode(rxPin,INPUT); // set pin modes for RX and TX
    pinMode(txPin,OUTPUT);
    
    Serial.begin(9600); // set the baud rate for the RT4K Serial Connection
    Serial.println("\r"); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                          // sending a carriage return clears out the buffer so the first input change works correctly
    extronSerial.begin(9600); // set the baud rate for the Extron Connection
    extronSerial.setTimeout(50); // sets the timeout for reading / saving reads into a string
    
}

void loop() {

    // listen to the Extron Serial Port for changes
    if(extronSerial.available() > 0){
      ecap = extronSerial.readString().substring(0,10); // read in and store only the first 10 chars for every Extron status message received
      if(ecap.substring(0,3) == "Out") // store only the input status, some Extron devices report output first instead of input
        einput = ecap.substring(6,4);
      else
        einput = ecap.substring(0,4);
      
      // filter out unstable Extron connections and other status messages to prevent the same profile change being sent multiple times
      // use remaining results to see which input is now active and change profile accordingly
      if(preveinput != einput && einput.substring(0,2) == "In" && einput != "In0 " && einput != "In00"){
        if(einput == "In1 " || einput == "In01"){
          Serial.println("remote prof1\r");
        }
        else if(einput == "In2 " || einput == "In02"){
          Serial.println("remote prof2\r");
        }
        else if(einput == "In3 " || einput == "In03"){
          Serial.println("remote prof3\r");
        }
        else if(einput == "In4 " || einput == "In04"){
          Serial.println("remote prof4\r");
        }
        else if(einput == "In5 " || einput == "In05"){
          Serial.println("remote prof5\r");
        }
        else if(einput == "In6 " || einput == "In06"){
          Serial.println("remote prof6\r");
        }
        else if(einput == "In7 " || einput == "In07"){
          Serial.println("remote prof7\r");
        }
        else if(einput == "In8 " || einput == "In08"){
          Serial.println("remote prof8\r");
        }
        else if(einput == "In9 " || einput == "In09"){
          Serial.println("remote prof9\r");
        }
        else if(einput == "In10"){
          Serial.println("remote prof10\r");
        }
        else if(einput == "In11"){
          Serial.println("remote prof11\r");
        }
        else if(einput == "In12"){
          Serial.println("remote prof12\r");
        }
        // Possible to add more than 12 if you hardcode SVS commands to load specific .rt4 profiles
        // SVS NEW INPUT=<input number> triggers an auto profile load
        // When SVS signals new input, RT4K checks the /profile/SVS subfolder for a matching profile
        // Profiles need to be named: ‘S<input number>_<user defined>.rt4’
        // For example, SVS input 2 would look for a profile that is named S2_SNES…rt4
        // If there’s more than one profile that fits the pattern, the first match is used
          
          preveinput = einput;
      }
    }

    
   delay(250);
}
