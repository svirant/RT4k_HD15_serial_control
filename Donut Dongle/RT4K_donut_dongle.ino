// RT4K Donut Dongle v0.4
//
//
//

#define PORT->Group[g_APinDescription[SCL].ulPort].PINCFG[g_APinDescription[SCL].ulPin].bit.PMUXEN = 0; // set SCL pin back to GPIO
#define PORT->Group[g_APinDescription[SDA].ulPort].PINCFG[g_APinDescription[SDA].ulPin].bit.PMUXEN = 0; // set SDA pin back to GPIO

#define IR_SEND_PIN 11  // Optional IR LED Emitter for RT5X compatibility. Sends IR data out Arduino pin D11
#define IR_RECEIVE_PIN 2 // Optional IR Receiver on pin D2

#include "TinyIRReceiver.hpp"
#include <IRremote.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h> // https://github.com/PaulStoffregen/AltSoftSerial in order to have a 3rd Serial port for 2nd Extron Switch / alt sw2
                           // Step 1 - Goto the github link above. Click the GREEN "<> Code" box and "Download ZIP"
                           // Step 2 - In Arudino IDE; goto "Sketch" -> "Include Library" -> "Add .ZIP Library"
IRsend irsend;


// Extron sw1 / alt sw1 software serial port -> MAX3232 TTL IC
#define rxPin 3 // sets Rx pin to D3 on Arduino
#define txPin 4 // sets Tx pin to D4 ...

uint16_t gscart1 = 0x0f; //used to store state of first gscart switch
uint16_t gscart1prev = 0x0f; //used to store previous state of first gscart switch
uint16_t gscart2 = 0x0f; //used to store state of second gscart switch
uint16_t gscart2prev = 0x0f; //used to store previous state of second gscart switch


/*
////////////////////
//    OPTIONS    //
//////////////////
*/


int SVS = 0; //     "Remote" profiles are profiles that are assigned to buttons 1-12 on the RT4K remote. "SVS" profiles reside under the "/profile/SVS/" directory 
             //     on the SD card.  This option allows you to choose which ones to call when a console is powered on.  Remote profiles allow you to easily change 
             //     the profile being used for a console's switch input if your setup is in flux. SVS require you to rename the file itself on the SD card which is 
             //     a little more work.  Regardless, SVS profiles will need to be used for console switch inputs over 12.
             //
             // **** Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu. A requirement for most options ****
             //
             // 0 - use "Remote" profiles 1-12 for up to 12 inputs on 1st Extron Switch and SVS 13 - 99 for everything over 12. Only SVS profiles are used on 2nd Extron Switch if connected.
             //
             //     - remote profiles 1-12 for 1st Extron or TESmart Switch (If DP0 below is set to true - remote profile 12 is used when all ports are in-active)
             //     - SVS  12 - 99  for 1st Extron or TESmart (DP0 is true)
             //     - SVS  13 - 99  for 1st Extron or TESmart (DP0 is false)
             //     - SVS 101 - 199 for 2nd Extron or TESmart
             //     - SVS 201 - 208 for 1st gScart
             //     - SVS 209 - 216 for 2nd gScart
             //
             // 1 - use only "SVS" profiles.
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu
             //     RT4K checks the /profile/SVS subfolder for profiles and need to be named: "S<input number>_<user defined>.rt4"
             //     For example, SVS input 2 would look for a profile that is named S2_SNES…rt4
             //     If there’s more than one profile that fits the pattern, the first match is used
             //
             //     - SVS   1 -  99 for 1st Extron or TESmart
             //     - SVS 101 - 199 for 2nd Extron or TESmart
             //     - SVS 201 - 208 for 1st gScart
             //     - SVS 209 - 216 for 2nd gScart
             //     - SVS   0       for DP0 option mentioned below
             //
             //  ** If DP0 below is set to true, create "S0_<user defined>.rt4" for when all ports are in-active. Ex: S0_DefaultHDMI.rt4
             //
             // 2 - use "Remote" profiles 1-12 for gscart/gcomp switches. Remote profile 1-8 for 1st gscart switch, 9-12 for inputs 1-4 on 2nd gscart switch.
             //     inputs 5-8 on the 2nd gscart switch will use SVS profiles 213 - 216
             //
             //     - remote profiles 1-8 for 1st gScart, 9 - 12 for first 4 inputs on 2nd gScart
             //     - SVS 212 -  216 for remaining inputs 5 - 8 on 2nd gScart 
             //     - SVS   1 -  99 for 1st Extron or TESmart
             //     - SVS 101 - 199 for 2nd Extron or TESmart
             //
             //  ** Best to set DP0 false if using gscart due to the current detection method (not knowing if all inputs are off) **



bool DP0  = false;        // (Default Profile 0) 
                         //
                         // ** Best to set DP0 to false if using gscart due to the current detection method (not knowing if all inputs are off) **
                         //
                         // set true to load "Remote" profile 12 (if SVS=0) when all ports are in-active on 1st Extron switch (and 2nd if connected). 
                         // You can assign it to a generic HDMI profile for example.
                         // If your device has a 12th input, SVS will be used instead. "IF" you have a 2nd Extron Switch connected, the remote profile 12
                         // will only load if "BOTH" switches have all in-active ports.
                         // 
                         // 
                         // If SVS=1, /profile/SVS/ "S0_<user defined>.rt4" will be used instead of remote profile 12
                         //
                         // If SVS=2, DP0 is ignored due to the current detection system for gscart (not knowing if all inputs are off)
                         //
                         // default is false // also recommended to set false to filter out unstable Extron inputs that can result in spamming the RT4K with profile changes 
                       


uint16_t voutMatrix[65] = {1,  // MATRIX switchers // by default ALL input changes to any/all outputs result in a profile change
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
                           1,  // output 16
                           1,  // output 17
                           1,  // output 18
                           1,  // output 19
                           1,  // output 20
                           1,  // output 21
                           1,  // output 22
                           1,  // output 23
                           1,  // output 24
                           1,  // output 25
                           1,  // output 26
                           1,  // output 27
                           1,  // output 28
                           1,  // output 29
                           1,  // output 30
                           1,  // output 31
                           1,  // output 32 (1 = enabled, 0 = disabled)
                               //
                               // ONLY USE FOR 2ND MATRIX SWITCH
                           1,  // 2ND MATRIX SWITCH output 1 (1 = enabled, 0 = disabled)
                           1,  // 2ND MATRIX SWITCH output 2
                           1,  // 2ND MATRIX SWITCH output 3
                           1,  // 2ND MATRIX SWITCH output 4
                           1,  // 2ND MATRIX SWITCH output 5
                           1,  // 2ND MATRIX SWITCH output 6
                           1,  // 2ND MATRIX SWITCH output 7
                           1,  // 2ND MATRIX SWITCH output 8
                           1,  // 2ND MATRIX SWITCH output 9
                           1,  // 2ND MATRIX SWITCH output 10
                           1,  // 2ND MATRIX SWITCH output 11
                           1,  // 2ND MATRIX SWITCH output 12
                           1,  // 2ND MATRIX SWITCH output 13
                           1,  // 2ND MATRIX SWITCH output 14
                           1,  // 2ND MATRIX SWITCH output 15
                           1,  // 2ND MATRIX SWITCH output 16
                           1,  // 2ND MATRIX SWITCH output 17
                           1,  // 2ND MATRIX SWITCH output 18
                           1,  // 2ND MATRIX SWITCH output 19
                           1,  // 2ND MATRIX SWITCH output 20
                           1,  // 2ND MATRIX SWITCH output 21
                           1,  // 2ND MATRIX SWITCH output 22
                           1,  // 2ND MATRIX SWITCH output 23
                           1,  // 2ND MATRIX SWITCH output 24
                           1,  // 2ND MATRIX SWITCH output 25
                           1,  // 2ND MATRIX SWITCH output 26
                           1,  // 2ND MATRIX SWITCH output 27
                           1,  // 2ND MATRIX SWITCH output 28
                           1,  // 2ND MATRIX SWITCH output 29
                           1,  // 2ND MATRIX SWITCH output 30
                           1,  // 2ND MATRIX SWITCH output 31
                           1,  // 2ND MATRIX SWITCH output 32 (1 = enabled, 0 = disabled)
                           };
                           


int RT5Xir = 1;      // 0 = disables IR Emitter for RetroTink 5x
                     // 1 = enabled for SW1 Extron switch or Otaku Games Scart Switch if connected
                     //     sends Profile 1 - 10 commands to RetroTink 5x. Must have IR LED emitter connected.
                     //     (DP0 - if enabled uses Profile 10 on the RT5X)
                     //
                     // 2 = enabled for gscart switch only (remote profiles 1-8 for first gscart, 9-10 for first 2 inputs on second)

int RT4Kir = 0;      // 0 = disables IR Emitter for RetroTink 4K
                     // 1 = enabled for SW1 Extron switch or Otaku Games Scart Switch if connected
                     //     sends Profile 1 - 12 commands to RetroTink 4K. Must have IR LED emitter connected.
                     //     (DP0 - if enabled uses Profile 12 on the RT4K)
                     //
                     // 2 = enabled for gscart switch only (remote profiles 1-8 for first gscart, 9-12 for first 4 inputs on second)
                          
                          
//////////////////  


byte ecapbytes[13]; // used to store first 13 captured bytes / messages for Extron sw1 / alt sw1                    
String ecap; // used to store Extron status messages for 1st Extron in String format
String einput; // used to store first 4 chars of Extron input
String previnput; // used to keep track of previous input
String eoutput; // used to store first 2 chars of Extron output

byte ecapbytes2[13]; // used to store first 13 captured bytes / messages for Extron sw2 / alt sw2
String ecap2; // used to store Extron status messages for 2nd Extron in String format
String einput2; // used to store first 4 chars of Extron input for 2nd Extron
String previnput2 = "discon"; // used to keep track of previous input
String eoutput2; // used to store first 2 chars of Extron output for 2nd Extron

SoftwareSerial extronSerial = SoftwareSerial(rxPin,txPin); // setup a software serial port for listening to Extron sw1 / alt sw1 
AltSoftSerial extronSerial2; // setup yet another serial port for listening to Extron sw2 / alt sw2. hardcoded to pins D8 / D9

// irRec() variables
int pwrtoggle = 0; // used to toggle remote power button command (on/off) when using the optional IR Receiver
int repeatcount = 0; // used to help emulate the repeat nature of directional button presses for the IR Serial Remote, irRec() function below
int extrabuttonprof = 0; // used to keep track of AUX8 button presses for addtional button profiles. Uses SVS 301 - 312
String svsbutton = "";
int nument = 0;



void setup(){

    pinMode(rxPin,INPUT); // set pin modes for RX and TX
    pinMode(txPin,OUTPUT);
    initPCIInterruptForTinyReceiver(); // for IR Receiver
    
    Serial.begin(9600); // set the baud rate for the RT4K Serial Connection
    while(!Serial){;}   // allow connection to establish before continuing
    Serial.print(F("\r")); // if the Arduino first powers on while connected to a computer, it sends some bytes out the serial port. 
                        // this can show up in the RT4K diag screen as garbage text. sending a carriage return allows the first input change to work correctly
    extronSerial.begin(9600); // set the baud rate for the Extron sw1 Connection
    extronSerial.setTimeout(150); // sets the timeout for reading / saving reads into a string
    extronSerial2.begin(9600); // set the baud rate for Extron sw2 Connection
    extronSerial2.setTimeout(150); // sets the timeout for reading / saving reads into a string for the Extron sw2 Connection
    DDRC  &= ~B00111111; // for gscart/gcomp, Set PC0-PC5 as inputs (shown on Nano as pins A0-A5) Connected to gscart1 IN_BIT0,IN_BIT1,IN_BIT2 and gscart2 IN_BIT0,IN_BIT1,IN_BIT2

} // end of setup


void loop(){

// below are a list of functions that loop over and over to read in port changes and other misc tasks. you can disable them by commenting them out

irRec(); // intercepts the remote's button presses and relays them through the Serial interface giving a much more responsive experience

readGscart1();

readGscart2();

readExtron1(); // also reads TESmart HDMI and Otaku Games Scart switch on "alt sw1" port

readExtron2(); // also reads TESmart HDMI and Otaku Games Scart switch on "alt sw2" port

all_inactive_ports_check();

//delay(250);
} /////////////////////////////////// end of void loop ////////////////////////////////////


void readExtron1(){
    
    // listens to the Extron sw1 Port for changes
    // SIS Command Responses reference - Page 77 https://media.extron.com/public/download/files/userman/XP300_Matrix_B.pdf
    if(extronSerial.available() > 0){ // if there is data available for reading, read
    extronSerial.readBytes(ecapbytes,13); // read in and store only the first 13 chars for every status message received from 1st Extron SW port
    }
    ecap = String((char *)ecapbytes); // convert bytes to String for Extron switches
    

    if(ecap.substring(0,3) == "Out"){ // store only the input and output states, some Extron devices report output first instead of input
      einput = ecap.substring(6,10);
      eoutput = ecap.substring(3,5);
    }
    else if(ecap.substring(0,1) == "F"){
      einput = ecap.substring(4,8);
      eoutput = "00";
    }
    else{                             // less complex switches only report input status, no output status
      einput = ecap.substring(0,4);
      eoutput = "00";
    }


    // for Extron devices, use remaining results to see which input is now active and change profile accordingly, cross-references voutMaxtrix
    if(einput.substring(0,2) == "In" && voutMatrix[eoutput.toInt()]){
      if(einput == "In1 " || einput == "In01"){
        if(SVS==0)Serial.println(F("remote prof1\r"));
        else{
          Serial.println(F("SVS NEW INPUT=1\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=1\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 1 
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x0B,2); // RT4K profile 1
      }
      else if(einput == "In2 " || einput == "In02"){
        if(SVS==0)Serial.println(F("remote prof2\r"));
        else{
          Serial.println(F("SVS NEW INPUT=2\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=2\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 2
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x07,2); // RT4K profile 2
      }
      else if(einput == "In3 " || einput == "In03"){
        if(SVS==0)Serial.println(F("remote prof3\r"));
        else{
          Serial.println(F("SVS NEW INPUT=3\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=3\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0xCC,2);delay(30);} // RT5X profile 3
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x03,2); // RT4K profile 3
      }
      else if(einput == "In4 " || einput == "In04"){
        if(SVS==0)Serial.println(F("remote prof4\r"));
        else{
          Serial.println(F("SVS NEW INPUT=4\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=4\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8E,2);delay(30);} // RT5X profile 4
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x0A,2); // RT4K profile 4
      }
      else if(einput == "In5 " || einput == "In05"){
        if(SVS==0)Serial.println(F("remote prof5\r"));
        else{
          Serial.println(F("SVS NEW INPUT=5\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=5\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8F,2);delay(30);} // RT5X profile 5
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x06,2); // RT4K profile 5
      }
      else if(einput == "In6 " || einput == "In06"){
        if(SVS==0)Serial.println(F("remote prof6\r"));
        else{
          Serial.println(F("SVS NEW INPUT=6\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=6\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0xC8,2);delay(30);} // RT5X profile 6
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x02,2); // RT4K profile 6
      }
      else if(einput == "In7 " || einput == "In07"){
        if(SVS==0)Serial.println(F("remote prof7\r"));
        else{
          Serial.println(F("SVS NEW INPUT=7\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=7\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8A,2);delay(30);} // RT5X profile 7
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x09,2); // RT4K profile 7
      }
      else if(einput == "In8 " || einput == "In08"){
        if(SVS==0)Serial.println(F("remote prof8\r"));
        else{
          Serial.println(F("SVS NEW INPUT=8\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=8\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8B,2);delay(30);} // RT5X profile 8
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x05,2); // RT4K profile 8
      }
      else if(einput == "In9 " || einput == "In09"){
        if(SVS==0)Serial.println(F("remote prof9\r"));
        else{
          Serial.println(F("SVS NEW INPUT=9\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=9\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0xC4,2);delay(30);} // RT5X profile 9
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x01,2); // RT4K profile 9
      }
      else if(einput == "In10"){
        if(SVS==0)Serial.println(F("remote prof10\r"));
        else{
          Serial.println(F("SVS NEW INPUT=10\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=10\r"));
        }
        if((RT5Xir == 1) && !DP0){irsend.sendNEC(0xB3,0x87,2);delay(30);} // RT5X profile 10
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x25,2); // RT4K profile 10
      }
      else if(einput == "In11"){
        if(SVS==0)Serial.println(F("remote prof11\r"));
        else{
          Serial.println(F("SVS NEW INPUT=11\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=11\r"));
        }
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x26,2); // RT4K profile 11
      }
      else if(einput == "In12"){
        if((SVS==0 && !DP0))Serial.println(F("remote prof12\r")); // okay to use this profile if DP0 is set to false
        else{
          Serial.println(F("SVS NEW INPUT=12\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=12\r"));
        }
        if((RT4Kir == 1) && !DP0)irsend.sendNEC(0x49,0x27,2); // RT4K profile 12
      }
      else if(einput != "In0 " && einput != "In00" && einput2 != "In0 " && einput2 != "In00"){ // for inputs 13-99 (SVS only)
        Serial.print(F("SVS NEW INPUT="));
        Serial.print(einput.substring(2,4));
        Serial.println(F("\r"));
        delay(1000);
        Serial.print(F("SVS CURRENT INPUT="));
        Serial.print(einput.substring(2,4));
        Serial.println(F("\r"));
      }

      previnput = einput;

    }

    // for TESmart HDMI switch on Extron sw1 Port
    if(ecapbytes[4] == 17){
      if(ecapbytes[6] == 22){
        if(SVS==0)Serial.println(F("remote prof1\r"));
        else{
          Serial.println(F("SVS NEW INPUT=1\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=1\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 1 
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x0B,2); // RT4K profile 1
      }
      else if(ecapbytes[6] == 23){
        if(SVS==0)Serial.println(F("remote prof2\r"));
        else{
          Serial.println(F("SVS NEW INPUT=2\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=2\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 2
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x07,2); // RT4K profile 2
      }
      else if(ecapbytes[6] == 24){
        if(SVS==0)Serial.println(F("remote prof3\r"));
        else{
          Serial.println(F("SVS NEW INPUT=3\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=3\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0xCC,2);delay(30);} // RT5X profile 3
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x03,2); // RT4K profile 3
      }
      else if(ecapbytes[6] == 25){
        if(SVS==0)Serial.println(F("remote prof4\r"));
        else{
          Serial.println(F("SVS NEW INPUT=4\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=4\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8E,2);delay(30);} // RT5X profile 4
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x0A,2); // RT4K profile 4
      }
      else if(ecapbytes[6] == 26){
        if(SVS==0)Serial.println(F("remote prof5\r"));
        else{
          Serial.println(F("SVS NEW INPUT=5\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=5\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8F,2);delay(30);} // RT5X profile 5
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x06,2); // RT4K profile 5
      }
      else if(ecapbytes[6] == 27){
        if(SVS==0)Serial.println(F("remote prof6\r"));
        else{
          Serial.println(F("SVS NEW INPUT=6\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=6\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0xC8,2);delay(30);} // RT5X profile 6
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x02,2); // RT4K profile 6
      }
      else if(ecapbytes[6] == 28){
        if(SVS==0)Serial.println(F("remote prof7\r"));
        else{
          Serial.println(F("SVS NEW INPUT=7\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=7\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8A,2);delay(30);} // RT5X profile 7
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x09,2); // RT4K profile 7
      }
      else if(ecapbytes[6] == 29){
        if(SVS==0)Serial.println(F("remote prof8\r"));
        else{
          Serial.println(F("SVS NEW INPUT=8\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=8\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8B,2);delay(30);} // RT5X profile 8
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x05,2); // RT4K profile 8
      }
      else if(ecapbytes[6] == 30){
        if(SVS==0)Serial.println(F("remote prof9\r"));
        else{
          Serial.println(F("SVS NEW INPUT=9\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=9\r"));
        }
        if(RT5Xir == 1){irsend.sendNEC(0xB3,0xC4,2);delay(30);} // RT5X profile 9
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x01,2); // RT4K profile 9
      }
      else if(ecapbytes[6] == 31){
        if(SVS==0 && !DP0)Serial.println(F("remote prof10\r"));
        else{
          Serial.println(F("SVS NEW INPUT=10\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=10\r"));
        }
        if((RT5Xir == 1) && !DP0){irsend.sendNEC(0xB3,0x87,2);delay(30);} // RT5X profile 10
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x25,2); // RT4K profile 10
      }
      else if(ecapbytes[6] == 32){
        if(SVS==0)Serial.println(F("remote prof11\r"));
        else{
          Serial.println(F("SVS NEW INPUT=11\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=11\r"));
        }
        if(RT4Kir == 1)irsend.sendNEC(0x49,0x26,2); // RT4K profile 11
      }
      else if(ecapbytes[6] == 33){
        if(SVS==0 && !DP0)Serial.println(F("remote prof12\r")); // okay to use this profile if DP0 is set to false
        else{
          Serial.println(F("SVS NEW INPUT=12\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=12\r"));
        }
        if((RT4Kir == 1) && !DP0)irsend.sendNEC(0x49,0x27,2); // RT4K profile 12
      }
      else if(ecapbytes[6] > 33 && ecapbytes[6] < 38){
      Serial.print(F("SVS NEW INPUT="));
      Serial.print(ecapbytes[6] - 21);
      Serial.println(F("\r"));
      delay(1000);
      Serial.print(F("SVS CURRENT INPUT="));
      Serial.print(ecapbytes[6] - 21);
      Serial.println(F("\r"));
      }
    }

    // set ecapbytes to 0 for next read
    for(int i = 0; i < 13; i++){
      ecapbytes[i] = 0;
    }

    // for Otaku Games Scart Switch
    if(ecap.substring(0,6) == "remote"){
      if(ecap == "remote prof10"){
          if((RT5Xir == 1) && !DP0){irsend.sendNEC(0xB3,0x87,2);delay(30);} // RT5X profile 10
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x25,2); // RT4K profile 10
          if(SVS == 0){
            Serial.println(F("remote prof10\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=10\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=10\r"));
          }
      }
      else if(ecap == "remote prof12"){
          delay(1);
      }
      else if(ecap.substring(0,12) == "remote prof1"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 1 
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x0B,2); // RT4K profile 1
          if(SVS == 0){
            Serial.println(F("remote prof1\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=1\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=1\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof2"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 2
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x07,2); // RT4K profile 2
          if(SVS == 0){
            Serial.println(F("remote prof2\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=2\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=2\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof3"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0xCC,2);delay(30);} // RT5X profile 3
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x03,2); // RT4K profile 3
          if(SVS == 0){
            Serial.println(F("remote prof3\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=3\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=3\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof4"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8E,2);delay(30);} // RT5X profile 4
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x0A,2); // RT4K profile 4
          if(SVS == 0){
            Serial.println(F("remote prof4\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=4\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=4\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof5"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8F,2);delay(30);} // RT5X profile 5
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x06,2); // RT4K profile 5
          if(SVS == 0){
            Serial.println(F("remote prof5\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=5\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=5\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof6"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0xC8,2);delay(30);} // RT5X profile 6
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x02,2); // RT4K profile 6
          if(SVS == 0){
            Serial.println(F("remote prof6\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=6\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=6\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof7"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8A,2);delay(30);} // RT5X profile 7
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x09,2); // RT4K profile 7
          if(SVS == 0){
            Serial.println(F("remote prof7\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=7\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=7\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof8"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0x8B,2);delay(30);} // RT5X profile 8
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x05,2); // RT4K profile 8
          if(SVS == 0){
            Serial.println(F("remote prof8\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=8\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=8\r"));
          }
      }
      else if(ecap.substring(0,12) == "remote prof9"){
          if(RT5Xir == 1){irsend.sendNEC(0xB3,0xC4,2);delay(30);} // RT5X profile 9
          if(RT4Kir == 1)irsend.sendNEC(0x49,0x01,2); // RT4K profile 9
          if(SVS == 0){
            Serial.println(F("remote prof9\r"));
          }
          else{
            Serial.println(F("SVS NEW INPUT=9\r"));
            delay(1000);
            Serial.println(F("SVS CURRENT INPUT=9\r"));
          }
      }

    }

} // end of readExtron1()


void readExtron2(){
    
    // listens to the Extron sw2 Port for changes
    if(extronSerial2.available() > 0){ // if there is data available for reading, read
    extronSerial2.readBytes(ecapbytes2,13); // read in and store only the first 13 chars for every status message received from 2nd Extron port
    }
    ecap2 = String((char *)ecapbytes2);

    if(ecap2.substring(0,3) == "Out"){ // store only the input and output states, some Extron devices report output first instead of input
      einput2 = ecap2.substring(6,10);
      eoutput2 = ecap2.substring(3,5);
    }
    else if(ecap2.substring(0,1) == "F"){
      einput2 = ecap2.substring(4,8);
      eoutput2 = "00";
    }
    else{                              // less complex switches only report input status, no output status
      einput2 = ecap2.substring(0,4);
      eoutput2 = "00";
    }


    // For Extron devices, use remaining results to see which input is now active and change profile accordingly, cross-references voutMaxtrix
    if(einput2.substring(0,2) == "In" && voutMatrix[eoutput2.toInt()+32]){
      if(einput2 != "In0 " && einput2 != "In00"){ // much easier method for switch 2 since ALL inputs will respond with SVS commands regardless of SVS option above
        Serial.print(F("SVS NEW INPUT="));
        if(einput2.substring(3,4) == " ")
          Serial.print(einput2.substring(2,3).toInt()+100);
        else
          Serial.print(einput2.substring(2,4).toInt()+100);
        Serial.println(F("\r"));
        delay(1000);
        Serial.print(F("SVS CURRENT INPUT="));
        if(einput2.substring(3,4) == " ")
          Serial.print(einput2.substring(2,3).toInt()+100);
        else
          Serial.print(einput2.substring(2,4).toInt()+100);
        Serial.println(F("\r"));
        }

        previnput2 = einput2;
      }

    // for TESmart HDMI switch on Extron sw2 Port
    if(ecapbytes2[4] == 17){
      if(ecapbytes2[6] > 21 && ecapbytes2[6] < 38){
        Serial.print(F("SVS NEW INPUT="));
        Serial.print(ecapbytes2[6] + 79);
        Serial.println(F("\r"));
        delay(1000);
        Serial.print(F("SVS CURRENT INPUT="));
        Serial.print(ecapbytes2[6] + 79);
        Serial.println(F("\r"));
      }
    }

    // set ecapbytes2 to 0 for next read
    for(int i = 0; i < 13; i++){
      ecapbytes2[i] = 0;
    }

    // for Otaku Games Scart Switch
    if(ecap2.substring(0,6) == "remote"){
      if(ecap2 == "remote prof10"){
          Serial.println(F("SVS NEW INPUT=110\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=110\r"));
      }
      else if(ecap2 == "remote prof12"){
          delay(1);
      }
      else if(ecap2.substring(0,12) == "remote prof1"){
          Serial.println(F("SVS NEW INPUT=101\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=101\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof2"){
          Serial.println(F("SVS NEW INPUT=102\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=102\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof3"){
          Serial.println(F("SVS NEW INPUT=103\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=103\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof4"){
          Serial.println(F("SVS NEW INPUT=104\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=104\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof5"){
          Serial.println(F("SVS NEW INPUT=105\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=105\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof6"){
          Serial.println(F("SVS NEW INPUT=106\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=106\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof7"){
          Serial.println(F("SVS NEW INPUT=107\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=107\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof8"){
          Serial.println(F("SVS NEW INPUT=108\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=108\r"));
      }
      else if(ecap2.substring(0,12) == "remote prof9"){
          Serial.println(F("SVS NEW INPUT=109\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=109\r"));
      }

    }

}// end of readExtron2()


void readGscart1(){

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

    gscart1 = PINC & B00000111; // read state of pins A0,A1,A2 (IN_BIT0, IN_BIT1, IN_BIT2 for gscart sw1)

    // has active port changed on gscart sw1?
    if(gscart1 != gscart1prev){
      //Detect which scart port is now active and change profile accordingly
      if(!(gscart1 ^ B00000000)){
        if(SVS==2)Serial.println(F("remote prof1\r"));
        else{
          Serial.println(F("SVS NEW INPUT=201\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=201\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 1 
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x0B,2); // RT4K profile 1
      } 
      else if(!(gscart1 ^ B00000001)){
        if(SVS==2)Serial.println(F("remote prof2\r"));
        else{
          Serial.println(F("SVS NEW INPUT=202\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=202\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 2
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x07,2);  // RT4K profile 2
      }
      else if(!(gscart1 ^ B00000010)){
        if(SVS==2)Serial.println(F("remote prof3\r"));
        else{
          Serial.println(F("SVS NEW INPUT=203\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=203\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0xCC,2);delay(30);} // RT5X profile 3
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x03,2);  // RT4K profile 3
      }
      else if(!(gscart1 ^ B00000011)){
        if(SVS==2)Serial.println(F("remote prof4\r"));
        else{
          Serial.println(F("SVS NEW INPUT=204\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=204\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x8E,2);delay(30);} // RT5X profile 4
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x0A,2);  // RT4K profile 4
      }
      else if(!(gscart1 ^ B00000100)){
        if(SVS==2)Serial.println(F("remote prof5\r"));
        else{
          Serial.println(F("SVS NEW INPUT=205\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=205\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x8F,2);delay(30);} // RT5X profile 5
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x06,2);  // RT4K profile 5
      } 
      else if(!(gscart1 ^ B00000101)){
        if(SVS==2)Serial.println(F("remote prof6\r"));
        else{
          Serial.println(F("SVS NEW INPUT=206\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=206\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0xC8,2);delay(30);} // RT5X profile 6
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x02,2);  // RT4K profile 6
      }   
      else if(!(gscart1 ^ B00000110)){
        if(SVS==2)Serial.println(F("remote prof7\r"));
        else{
          Serial.println(F("SVS NEW INPUT=207\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=207\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x8A,2);delay(30);} // RT5X profile 7
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x09,2);  // RT4K profile 7
      } 
      else if(!(gscart1 ^ B00000111)){
        if(SVS==2)Serial.println(F("remote prof8\r"));
        else{
          Serial.println(F("SVS NEW INPUT=208\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=208\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x8B,2);delay(30);} // RT5X profile 8
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x05,2);  // RT4K profile 8
      }
        

      gscart1prev = gscart1;
    }
} // end of readGscart1()


void readGscart2(){

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

    gscart2 = PINC & B00111000; //read state of pins A3,A4,A5 (IN_BIT0, IN_BIT1, IN_BIT2 for gscart sw2)

    // has active port changed on gscart sw2?
    if(gscart2 != gscart2prev){
      //Detect which scart port is now active and change profile accordingly
      if(!(gscart2 ^ B00000000)){
        if(SVS==2)Serial.println(F("remote prof9\r"));
        else{
          Serial.println(F("SVS NEW INPUT=209\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=209\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 9
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x01,2);  // RT4K profile 9
      } 
      else if(!(gscart2 ^ B00001000)){
        if(SVS==2)Serial.println(F("remote prof10\r"));
        else{
          Serial.println(F("SVS NEW INPUT=210\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=210\r"));
        }
        if(RT5Xir == 2){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 10
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x25,2);  // RT4K profile 10
      }
      else if(!(gscart2 ^ B00010000)){
        if(SVS==2)Serial.println(F("remote prof11\r"));
        else{
          Serial.println(F("SVS NEW INPUT=211\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=211\r"));
        }
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x26,2);  // RT4K profile 11
      }
      else if(!(gscart2 ^ B00011000)){
        if(SVS==2)Serial.println(F("remote prof12\r"));
        else{
          Serial.println(F("SVS NEW INPUT=212\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=212\r"));
        }
        if(RT4Kir == 2)irsend.sendNEC(0x49,0x27,2); // RT4K profile 12
      }
      else if(!(gscart2 ^ B00100000)){
        Serial.println(F("SVS NEW INPUT=213\r"));
        delay(1000);
        Serial.println(F("SVS CURRENT INPUT=213\r"));
      } 
      else if(!(gscart2 ^ B00101000)){
        Serial.println(F("SVS NEW INPUT=214\r"));
        delay(1000);
        Serial.println(F("SVS CURRENT INPUT=214\r"));
      }   
      else if(!(gscart2 ^ B00110000)){
        Serial.println(F("SVS NEW INPUT=215\r"));
        delay(1000);
        Serial.println(F("SVS CURRENT INPUT=215\r"));
      } 
      else if(!(gscart2 ^ B00111000)){
        Serial.println(F("SVS NEW INPUT=216\r"));
        delay(1000);
        Serial.println(F("SVS CURRENT INPUT=216\r"));
      }
        

      gscart2prev = gscart2;
     }
} // end of readGscart2()


void all_inactive_ports_check(){

    // when both Extron switches match In0 or In00 (no active ports), a default profile can be loaded if DP0 is enabled
    if(((previnput == "In0 " || previnput == "In00") && (previnput2 == "In0 " || previnput2 == "In00" || previnput2 == "discon")) && DP0 && voutMatrix[eoutput.toInt()] && (previnput2 == "discon" || voutMatrix[eoutput2.toInt()+32])){
      if(SVS==0)Serial.println(F("remote prof12\r"));
      else{
        Serial.println(F("SVS NEW INPUT=0\r"));
        delay(1000);
        Serial.println(F("SVS CURRENT INPUT=0\r"));
      }
      if(RT5Xir == 1){irsend.sendNEC(0xB3,0x87,2);delay(30);} // RT5X profile 10
      if(RT4Kir == 1)irsend.sendNEC(0x49,0x27,2); // RT4K profile 12

      previnput = "0";
      if(previnput2 != "discon")previnput2 = "0";

    }

    if(previnput == "0" && previnput2.substring(0,2) == "In")previnput = "In00";  // changes previnput "0" state to "In00" when there is a newly active input on the other switch
    if(previnput2 == "0" && previnput.substring(0,2) == "In")previnput2 = "In00"; 

} // end of all_inactive_ports_check

void irRec(){

  int ir_recv_command = 0;
  int ir_recv_address = 0;

  if(TinyReceiverDecode()){

    ir_recv_command = TinyIRReceiverData.Command;
    ir_recv_address = TinyIRReceiverData.Address;
        
      // printTinyReceiverResultMinimal(&Serial);
      // Serial.print("Address=0x");Serial.print(ir_recv_address,HEX);
      // Serial.print(" Command=0x");Serial.println(ir_recv_command,HEX);
      // Serial.print(F("Address="));
      // Serial.print(ir_recv_address);
      // Serial.print(F(" Command="));
      // Serial.println(ir_recv_command);

    if(ir_recv_address == 73 && TinyIRReceiverData.Flags != IRDATA_FLAGS_IS_REPEAT && extrabuttonprof == 2){
      if(ir_recv_command == 11){                                                                        // load SVS profiles 301 - 312 (profile button 1 - 12)
        //Serial.println(F("remote prof1\r"));
        svsbutton += 1;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 7){
        //Serial.println(F("remote prof2\r"));
        svsbutton += 2;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 3){
        //Serial.println(F("remote prof3\r"));
        svsbutton += 3;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 10){
        //Serial.println(F("remote prof4\r"));
        svsbutton += 4;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 6){
        //Serial.println(F("remote prof5\r"));
        svsbutton += 5;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 2){
        //Serial.println(F("remote prof6\r"));
        svsbutton += 6;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 9){
        //Serial.println(F("remote prof7\r"));
        svsbutton += 7;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 5){
        //Serial.println(F("remote prof8\r"));
        svsbutton += 8;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 1){
        //Serial.println(F("remote prof9\r"));
        svsbutton += 9;
        nument++;
        ir_recv_command = 0;
      }
      else if(ir_recv_command == 37 || ir_recv_command == 38 || ir_recv_command == 39){
        svsbutton += 0;
        nument++;
        ir_recv_command = 0;
      }
      else{
        extrabuttonprof = 0;
        svsbutton = "";
        nument = 0;
      }

      
    } // end of extrabutton == 2

    if(ir_recv_address == 73 && TinyIRReceiverData.Flags != IRDATA_FLAGS_IS_REPEAT && extrabuttonprof == 1){ // if AUX8 was pressed and a profile button is pressed next,
      if(ir_recv_command == 11){                                                                        // load SVS profiles 301 - 312 (profile button 1 - 12)
        //Serial.println(F("remote prof1\r"));
          Serial.println(F("SVS NEW INPUT=301\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=301\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 7){
        //Serial.println(F("remote prof2\r"));
          Serial.println(F("SVS NEW INPUT=302\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=302\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 3){
        //Serial.println(F("remote prof3\r"));
          Serial.println(F("SVS NEW INPUT=303\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=303\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 10){
        //Serial.println(F("remote prof4\r"));
          Serial.println(F("SVS NEW INPUT=304\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=304\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 6){
        //Serial.println(F("remote prof5\r"));
          Serial.println(F("SVS NEW INPUT=305\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=305\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 2){
        //Serial.println(F("remote prof6\r"));
          Serial.println(F("SVS NEW INPUT=306\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=306\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 9){
        //Serial.println(F("remote prof7\r"));
          Serial.println(F("SVS NEW INPUT=307\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=307\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 5){
        //Serial.println(F("remote prof8\r"));
          Serial.println(F("SVS NEW INPUT=308\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=308\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 1){
        //Serial.println(F("remote prof9\r"));
          Serial.println(F("SVS NEW INPUT=309\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=309\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 37){
        //Serial.println(F("remote prof10\r"));
          Serial.println(F("SVS NEW INPUT=310\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=310\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 38){
        //Serial.println(F("remote prof11\r"));
          Serial.println(F("SVS NEW INPUT=311\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=311\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 39){
        //Serial.println(F("remote prof12\r"));
          Serial.println(F("SVS NEW INPUT=312\r"));
          delay(1000);
          Serial.println(F("SVS CURRENT INPUT=312\r"));
          ir_recv_command = 0;
          extrabuttonprof = 0;
      }
      else if(ir_recv_command == 63){
        ir_recv_command = 0;
        extrabuttonprof++;

      }
      else{
        extrabuttonprof = 0;
        svsbutton = "";
        nument = 0;
      }
      
    } // end extrabuttonprof == 1


    if(nument == 3){
      Serial.print(F("SVS NEW INPUT="));Serial.print(svsbutton);Serial.println(F("\r"));
      delay(1000);
      Serial.print(F("SVS CURRENT INPUT="));Serial.print(svsbutton);Serial.println(F("\r"));
      nument = 0;
      svsbutton = "";
      extrabuttonprof = 0;
      ir_recv_command = 0;
    }
    
    if(TinyIRReceiverData.Flags == IRDATA_FLAGS_IS_REPEAT){repeatcount++;} // directional buttons have to be held down for just a bit before repeating

    if(ir_recv_address == 73 && TinyIRReceiverData.Flags != IRDATA_FLAGS_IS_REPEAT){ // block most buttons from being repeated when held
      repeatcount = 0;
      if(ir_recv_command == 63){
        //Serial.println(F("remote aux8\r"));
        extrabuttonprof++;
      }
      else if(ir_recv_command == 62){
        Serial.println(F("remote aux7\r"));
      }
      else if(ir_recv_command == 61){
        Serial.println(F("remote aux6\r"));
      }
      else if(ir_recv_command == 60){
        Serial.println(F("remote aux5\r"));
      }
      else if(ir_recv_command == 59){
        Serial.println(F("remote aux4\r"));
      }
      else if(ir_recv_command == 58){
        Serial.println(F("remote aux3\r"));
      }
      else if(ir_recv_command == 57){
        Serial.println(F("remote aux2\r"));
      }
      else if(ir_recv_command == 56){
        Serial.println(F("remote aux1\r"));
      }
      else if(ir_recv_command == 52){
        Serial.println(F("remote res1\r"));
      }
      else if(ir_recv_command == 53){
        Serial.println(F("remote res2\r"));
      }
      else if(ir_recv_command == 54){
        Serial.println(F("remote res3\r"));
      }
      else if(ir_recv_command == 55){
        Serial.println(F("remote res4\r"));
      }
      else if(ir_recv_command == 51){
        Serial.println(F("remote res480p\r"));
      }
      else if(ir_recv_command == 50){
        Serial.println(F("remote res1440p\r"));
      }
      else if(ir_recv_command == 49){
        Serial.println(F("remote res1080p\r"));
      }
      else if(ir_recv_command == 48){
        Serial.println(F("remote res4k\r"));
      }
      else if(ir_recv_command == 47){
        Serial.println(F("remote buffer\r"));
      }
      else if(ir_recv_command == 44){
        Serial.println(F("remote genlock\r"));
      }
      else if(ir_recv_command == 46){
        Serial.println(F("remote safe\r"));
      }
      else if(ir_recv_command == 86){
        Serial.println(F("remote pause\r"));
      }
      else if(ir_recv_command == 45){
        Serial.println(F("remote phase\r"));
      }
      else if(ir_recv_command == 43){
        Serial.println(F("remote gain\r"));
      }
      else if(ir_recv_command == 36){
        Serial.println(F("remote prof\r"));
      }
      else if(ir_recv_command == 11){
        Serial.println(F("remote prof1\r"));
      }
      else if(ir_recv_command == 7){
        Serial.println(F("remote prof2\r"));
      }
      else if(ir_recv_command == 3){
        Serial.println(F("remote prof3\r"));
      }
      else if(ir_recv_command == 10){
        Serial.println(F("remote prof4\r"));
      }
      else if(ir_recv_command == 6){
        Serial.println(F("remote prof5\r"));
      }
      else if(ir_recv_command == 2){
        Serial.println(F("remote prof6\r"));
      }
      else if(ir_recv_command == 9){
        Serial.println(F("remote prof7\r"));
      }
      else if(ir_recv_command == 5){
        Serial.println(F("remote prof8\r"));
      }
      else if(ir_recv_command == 1){
        Serial.println(F("remote prof9\r"));
      }
      else if(ir_recv_command == 37){
        Serial.println(F("remote prof10\r"));
      }
      else if(ir_recv_command == 38){
        Serial.println(F("remote prof11\r"));
      }
      else if(ir_recv_command == 39){
        Serial.println(F("remote prof12\r"));
      }
      else if(ir_recv_command == 35){
        Serial.println(F("remote adc\r"));
      }
      else if(ir_recv_command == 34){
        Serial.println(F("remote sfx\r"));
      }
      else if(ir_recv_command == 33){
        Serial.println(F("remote scaler\r"));
      }
      else if(ir_recv_command == 32){
        Serial.println(F("remote output\r"));
      }
      else if(ir_recv_command == 17){
        Serial.println(F("remote input\r"));
      }
      else if(ir_recv_command == 41){
        Serial.println(F("remote stat\r"));
      }
      else if(ir_recv_command == 40){
        Serial.println(F("remote diag\r"));
      }
      else if(ir_recv_command == 66){
        Serial.println(F("remote back\r"));
      }
      else if(ir_recv_command == 83){
        Serial.println(F("remote ok\r"));
      }
      else if(ir_recv_command == 79){
        Serial.println(F("remote right\r"));
      }
      else if(ir_recv_command == 16){
        Serial.println(F("remote down\r"));
      }
      else if(ir_recv_command == 87){
        Serial.println(F("remote left\r"));
      }
      else if(ir_recv_command == 24){
        Serial.println(F("remote up\r"));
      }
      else if(ir_recv_command == 92){
        Serial.println(F("remote menu\r"));
      }
      else if(ir_recv_command == 26){
        if(pwrtoggle){
          Serial.println(F("pwr on\r"));
          pwrtoggle = 0;
        }
        else{
          Serial.println(F("remote pwr\r"));
          pwrtoggle = 1;
        }
      }
    }
    else if(ir_recv_address == 73 && repeatcount > 4){ // directional buttons have to be held down for just a bit before repeating
      if(ir_recv_command == 24){
        Serial.println(F("remote up\r"));
      }
      else if(ir_recv_command == 16){
        Serial.println(F("remote down\r"));
      }
      else if(ir_recv_command == 87){
        Serial.println(F("remote left\r"));
      }
      else if(ir_recv_command == 79){
        Serial.println(F("remote right\r"));
      }
    } // end of if(ir_recv_address)

  } // end of TinyReceiverDecode() 

} // end of irRec()
