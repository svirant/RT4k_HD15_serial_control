// Extron Arduino Nano edition
// RT4K HD15 serial control
//
// Features:  - supports up to 16 inputs w/ 16 selectable outputs for Matrix switchers
//            - remote or SVS profiles, or a mix of both
//            - option to auto load profile when no inputs are active
// 
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

#include <SoftwareSerial.h>

/*
////////////////////
//    OPTIONS    //
//////////////////
*/

int SVS = 0; //
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu, if using options 1 or 2
             //
             // 0 - use only "remote" profiles 1-12 for up to 12 inputs
             //     If input0 below is set to true - profile 12 is used when all ports are in-active
             //
             // 1 - use only "SVS" profiles for up to 16 inputs
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu
             //     RT4K checks the /profile/SVS subfolder for profiles and need to be named: "S<input number>_<user defined>.rt4"
             //     For example, SVS input 2 would look for a profile that is named S2_SNES…rt4
             //     If there’s more than one profile that fits the pattern, the first match is used
             //
             //  ** If input0 below is set to true - create "S0_<user defined>.rt4" for when all ports are in-active. Ex: S0_DefaultHDMI.rt4
             //
             // 2 - use "remote" profiles 1-12 and SVS profiles for 13-16
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu
             //     RT4K checks the /profile/SVS subfolder for profiles and need to be named: "S<input number>_<user defined>.rt4"
             //     For example, SVS input 2 would look for a profile that is named S2_SNES…rt4
             //     If there’s more than one profile that fits the pattern, the first match is used
             //
             //  ** If input0 below is set to true - create "S0_<user defined>.rt4" for when all ports are in-active. Ex: S0_DefaultHDMI.rt4


bool input0  = false;    // set true when all ports are in-active, load profile 12. You can assign it to a generic HDMI profile for example
                         // If your device has a 12th input, this option disables profile changes for it
                         // default is false // set false to filter out unstable Extron inputs that can result in spamming the RT4K with profile changes


uint16_t voutMatrix[17] = {1,  // MATRIX switchers // by default ALL input changes to any/all outputs result in a profile change
                                                   // disable specific outputs from triggering profile changes
                                                   //
                           1,  // output 1 (1 = enabled, 0 = disabled)
                           1,  // output 2
                           1,  // output 3
                           1,  // output 4
                           1,  // output 5
                           1,  // output 6
                           1,  // output 7
                           1,  // output 8
                           1,  // output 9
                           1,  // output 10
                           1,  // output 11
                           1,  // output 12
                           1,  // output 13
                           1,  // output 14
                           1,  // output 15
                           1,  // output 16 (1 = enabled, 0 = disabled)
                           };

#define rxPin 3 // sets Rx pin to D3 on Arduino to talk to MAX3232 TTL Serial IC
#define txPin 4 // sets Tx pin to D4 ...

//////////////////                         

                          
String ecap; // used to store Extron status messages
String einput; // used to store first 4 chars of Extron input
String eoutput; // used to store first 2 chars of Extron output
String preveinput; // used to keep track of previous input

SoftwareSerial extronSerial = SoftwareSerial(rxPin,txPin); // setup an additional serial port for listening to the Extron


void setup()  {

    pinMode(rxPin,INPUT); // set pin modes for RX and TX
    pinMode(txPin,OUTPUT);
    
    Serial.begin(9600); // set the baud rate for the RT4K Serial Connection
    Serial.print("\r"); // when the Arduino first powers on, it sends garbage bytes out the hardware serial port. this shows up in the RT4K diag screen.
                          // sending a carriage return clears out the buffer so the first input change works correctly
    extronSerial.begin(9600); // set the baud rate for the Extron Connection
    extronSerial.setTimeout(50); // sets the timeout for reading / saving reads into a string

    
} // end of setup

void loop() {


    // listen to the Extron Serial Port for changes
    // SIS Command Responses reference - Page 77 https://media.extron.com/public/download/files/userman/XP300_Matrix_B.pdf
    if(extronSerial.available() > 0){
      ecap = extronSerial.readString().substring(0,10); // read in and store only the first 10 chars for every Extron status message received
      if(ecap.substring(0,3) == "Out"){ // store only the input status, some Extron devices report output first instead of input
        einput = ecap.substring(6,4);
        eoutput = ecap.substring(3,2);
      }
      else{
        einput = ecap.substring(0,4);
        eoutput = "00";
      }
    }


      // use remaining results to see which input is now active and change profile accordingly
      if(preveinput != einput && einput.substring(0,2) == "In" && voutMatrix[eoutput.toInt()]){
        if((einput == "In0 " || einput == "In00") && input0){ // if input0 is enabled and all inputs are in-active
          if(SVS==0)Serial.println("remote prof12\r");
          else if(SVS==1 || SVS==2){
            Serial.println("SVS NEW INPUT=0\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=0\r");
          }
        }
        else if(einput == "In1 " || einput == "In01"){
          if(SVS==0 || SVS==2)Serial.println("remote prof1\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=1\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=1\r");
          }
        }
        else if(einput == "In2 " || einput == "In02"){
          if(SVS==0 || SVS==2)Serial.println("remote prof2\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=2\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=2\r");
          }
        }
        else if(einput == "In3 " || einput == "In03"){
          if(SVS==0 || SVS==2)Serial.println("remote prof3\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=3\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=3\r");
          }
        }
        else if(einput == "In4 " || einput == "In04"){
          if(SVS==0 || SVS==2)Serial.println("remote prof4\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=4\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=4\r");
          }
        }
        else if(einput == "In5 " || einput == "In05"){
          if(SVS==0 || SVS==2)Serial.println("remote prof5\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=5\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=5\r");
          }
        }
        else if(einput == "In6 " || einput == "In06"){
          if(SVS==0 || SVS==2)Serial.println("remote prof6\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=6\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=6\r");
          }
        }
        else if(einput == "In7 " || einput == "In07"){
          if(SVS==0 || SVS==2)Serial.println("remote prof7\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=7\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=7\r");
          }
        }
        else if(einput == "In8 " || einput == "In08"){
          if(SVS==0 || SVS==2)Serial.println("remote prof8\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=8\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=8\r");
          }
        }
        else if(einput == "In9 " || einput == "In09"){
          if(SVS==0 || SVS==2)Serial.println("remote prof9\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=9\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=9\r");
          }
        }
        else if(einput == "In10"){
          if(SVS==0 || SVS==2)Serial.println("remote prof10\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=10\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=10\r");
          }
        }
        else if(einput == "In11"){
          if(SVS==0 || SVS==2)Serial.println("remote prof11\r");
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=11\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=11\r");
          }
        }
        else if(einput == "In12"){
          if((SVS==0 && !input0) || SVS==2)Serial.println("remote prof12\r"); // okay to use this profile if input0 is disabled
          else if(SVS==1){
            Serial.println("SVS NEW INPUT=12\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=12\r");
          }
        }
        else if(einput == "In13"){
            Serial.println("SVS NEW INPUT=13\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=13\r");
        }
        else if(einput == "In14"){
            Serial.println("SVS NEW INPUT=14\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=14\r");
        }
        else if(einput == "In15"){
            Serial.println("SVS NEW INPUT=15\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=15\r");
        }
        else if(einput == "In16"){
            Serial.println("SVS NEW INPUT=16\r");
            delay(1000);
            Serial.println("SVS CURRENT INPUT=16\r");
        }
          
        if((einput != "In0 " && einput != "In00") || input0) // if input0 is true always save prev state. if input0 is false, only save when input is not 0
          preveinput = einput; 
           
      } 

    
   delay(250);
} // end of void loop
