// Extron Arduino Nano edition
// RT4K HD15 serial control
//
// Features:  - supports up to 99 inputs w/ 32 selectable outputs for Matrix switchers
//            - remote or SVS profiles, or a mix of both
//            - option to auto load profile when no inputs are active
//            - RetroTink 5X support with IR Led for inputs 1 - 10
//            - RT4K IR support as well, not as reliable as Serial
// 
//
// HD15 pin 15 = RX --> Arduino Nano TX pin
// HD15 pin 12 = TX --> Arduino Nano RX pin
// HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)
//
// Max3232 Rx = Arduino Nano D3 pin 
// Max3232 Tx = Arduino Nano D4 pin 
//
// If connecting a 2nd Extron Switch you must follow these steps:
//
// Step 1 - Goto: https://github.com/PaulStoffregen/AltSoftSerial 
// Step 2 - Click the GREEN "<> Code" box and "Download ZIP"
// Step 3 - In Arudino IDE; goto "Sketch" -> "Include Library" -> "Add .ZIP Library"
// Step 4 - Connect the following pins to a 2nd Max3232
//
// 2nd Max3232 Rx = Arduino Nano D8 pin 
// 2nd Max3232 Tx = Arduino Nano D9 pin 
//
//

#define IR_SEND_PIN 11  // Optional IR LED for RT5X compatibility. Sends IR data out Arduino pin D11
#include <IRremote.h>
#include <SoftwareSerial.h>
#include <AltSoftSerial.h> // https://github.com/PaulStoffregen/AltSoftSerial in order to have a 3rd Serial port for 2nd Extron Switch
                           // Step 1 - Goto the github link above. Click the GREEN "<> Code" box and "Download ZIP"
                           // Step 2 - In Arudino IDE; goto "Sketch" -> "Include Library" -> "Add .ZIP Library"
IRsend irsend;

/*
////////////////////
//    OPTIONS    //
//////////////////
*/


int SVS = 0; //
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu, a requirement for using options 1 or 2
             //
             // 0 - use only "remote" profiles 1-12 for up to 12 inputs on 1st Extron Switch, SVS profiles used on 2nd Extron Switch if connected
             //     If INPUT0 below is set to true - profile 12 is used when all ports are in-active
             //
             // 1 - use only "SVS" profiles for up to 99 inputs. SVS Profiles 101-199 must be used for a 2nd Extron Switch, if connected
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu
             //     RT4K checks the /profile/SVS subfolder for profiles and need to be named: "S<input number>_<user defined>.rt4"
             //     For example, SVS input 2 would look for a profile that is named S2_SNES…rt4
             //     If there’s more than one profile that fits the pattern, the first match is used
             //
             //  ** If INPUT0 below is set to true - create "S0_<user defined>.rt4" for when all ports are in-active. Ex: S0_DefaultHDMI.rt4
             //
             // 2 - use "remote" profiles 1-12 and SVS profiles for 13-99. SVS Profiles 101-199 must be used for a 2nd Extron Switch, if connected
             //     Make sure "Auto Load SVS" is "On" under the RT4K Profiles menu
             //     RT4K checks the /profile/SVS subfolder for profiles and need to be named: "S<input number>_<user defined>.rt4"
             //     For example, SVS input 2 would look for a profile that is named S2_SNES…rt4
             //     If there’s more than one profile that fits the pattern, the first match is used
             //
             //  ** If INPUT0 below is set to true - create "S0_<user defined>.rt4" for when all ports are in-active. Ex: S0_DefaultHDMI.rt4


bool INPUT0  = false;    // set true to load profile 12 (if SVS=0) when all ports are in-active on 1st Extron Connection. You can assign it to a generic HDMI profile for example
                         // If your device has a 12th input, this option disables profile changes for it. "IF" you have a 2nd Extron Switch connected, the profile will only load
                         // if "BOTH" switches have all in-active ports.
                         //
                         // If SVS=1 or 2 - load profile /profile/SVS/ "S0_<user defined>.rt4" when all ports are in-active
                         //
                         // default is false // set false to filter out unstable Extron inputs that can result in spamming the RT4K with profile changes 

bool EXTRON2 = false;   // Set to true if connecting to 2 Extron Switches. If this is not set correctly, INPUT0 functionality will be lost.                         


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


bool RT5Xir = false;      // sends Profile 1 - 10 commands to RetroTink 5x. Must have IR LED connected: Wiring info in README.
                          // recommend placing the LED as close as you can (taping in front of) to the RT5X sensor for a more reliable connection
                          // INPUT0 - if enabled uses Profile 10 on the RT5X

bool RT4Kir = false;      // sends Profile 1 - 12 commands to RT4K. Must have IR LED connected: Wiring info in README.
                          // recommend placing the LED as close as you can (taping in front of) to the RT4K sensor for a more reliable connection
                          
                          



#define rxPin 3 // sets Rx pin to D3 on Arduino to talk to MAX3232 TTL Serial IC
#define txPin 4 // sets Tx pin to D4 ...

//////////////////                         

                          
String ecap; // used to store Extron status messages
String einput; // used to store first 4 chars of Extron input
String prevzero; // used to keep track of previous input
String eoutput; // used to store first 2 chars of Extron output

String ecap2; // used to store Extron status messages for 2nd Extron Connection
String einput2; // used to store first 4 chars of Extron input for 2nd Extron Connection
String prevzero2; // used to keep track of previous input
String eoutput2; // used to store first 2 chars of Extron output for 2nd Extron Connection

SoftwareSerial extronSerial = SoftwareSerial(rxPin,txPin); // setup an additional serial port for listening to the Extron
AltSoftSerial extronSerial2; // setup an another serial port for listening to a 2nd connected Extron


void setup(){

    pinMode(rxPin,INPUT); // set pin modes for RX and TX for Extron Connection
    pinMode(txPin,OUTPUT);
    
    Serial.begin(9600); // set the baud rate for the RT4K Serial Connection
    while(!Serial){;}
    Serial.print("\r"); // if the Arduino first powers on while connected to a computer, it sends some bytes out the serial port. 
                        // this can show up in the RT4K diag screen as garbage text. sending a carriage return allows the first input change to work correctly
    extronSerial.begin(9600); // set the baud rate for the Extron Connection
    extronSerial.setTimeout(50); // sets the timeout for reading / saving reads into a string
    extronSerial2.begin(9600); // set the baud rate for the 2nd Extron Connection
    extronSerial2.setTimeout(50); // sets the timeout for reading / saving reads into a string for the 2nd Extron Connection



} // end of setup

void loop(){


    // listens to the Extron Serial Port for changes
    // SIS Command Responses reference - Page 77 https://media.extron.com/public/download/files/userman/XP300_Matrix_B.pdf
    ecap = extronSerial.readString().substring(0,10); // read in and store only the first 10 chars for every Extron status message received from 1st Extron Connection
    if(ecap.substring(0,3) == "Out"){ // store only the input and output states, some Extron devices report output first instead of input
      einput = ecap.substring(6,10);
      eoutput = ecap.substring(3,5);
    }
    else{                             // less complex switches only report input status
      einput = ecap.substring(0,4);
      eoutput = "00";
    }

    // use remaining results to see which input is now active and change profile accordingly
    if(einput.substring(0,2) == "In" && voutMatrix[eoutput.toInt()]){
      if(einput == "In1 " || einput == "In01"){
        if(SVS==0 || SVS==2)Serial.println("remote prof1\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=1\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=1\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0x92,2);delay(30);} // RT5X profile 1 
        if(RT4Kir)irsend.sendNEC(0x49,0x0B,2); // RT4K profile 1
      }
      else if(einput == "In2 " || einput == "In02"){
        if(SVS==0 || SVS==2)Serial.println("remote prof2\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=2\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=2\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0x93,2);delay(30);} // RT5X profile 2
        if(RT4Kir)irsend.sendNEC(0x49,0x07,2); // RT4K profile 2
      }
      else if(einput == "In3 " || einput == "In03"){
        if(SVS==0 || SVS==2)Serial.println("remote prof3\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=3\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=3\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0xCC,2);delay(30);} // RT5X profile 3
        if(RT4Kir)irsend.sendNEC(0x49,0x03,2); // RT4K profile 3
      }
      else if(einput == "In4 " || einput == "In04"){
        if(SVS==0 || SVS==2)Serial.println("remote prof4\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=4\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=4\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8E,2);delay(30);} // RT5X profile 4
        if(RT4Kir)irsend.sendNEC(0x49,0x0A,2); // RT4K profile 4
      }
      else if(einput == "In5 " || einput == "In05"){
        if(SVS==0 || SVS==2)Serial.println("remote prof5\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=5\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=5\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8F,2);delay(30);} // RT5X profile 5
        if(RT4Kir)irsend.sendNEC(0x49,0x06,2); // RT4K profile 5
      }
      else if(einput == "In6 " || einput == "In06"){
        if(SVS==0 || SVS==2)Serial.println("remote prof6\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=6\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=6\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0xC8,2);delay(30);} // RT5X profile 6
        if(RT4Kir)irsend.sendNEC(0x49,0x02,2); // RT4K profile 6
      }
      else if(einput == "In7 " || einput == "In07"){
        if(SVS==0 || SVS==2)Serial.println("remote prof7\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=7\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=7\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8A,2);delay(30);} // RT5X profile 7
        if(RT4Kir)irsend.sendNEC(0x49,0x09,2); // RT4K profile 7
      }
      else if(einput == "In8 " || einput == "In08"){
        if(SVS==0 || SVS==2)Serial.println("remote prof8\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=8\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=8\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0x8B,2);delay(30);} // RT5X profile 8
        if(RT4Kir)irsend.sendNEC(0x49,0x05,2); // RT4K profile 8
      }
      else if(einput == "In9 " || einput == "In09"){
        if(SVS==0 || SVS==2)Serial.println("remote prof9\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=9\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=9\r");
        }
        if(RT5Xir){irsend.sendNEC(0xB3,0xC4,2);delay(30);} // RT5X profile 9
        if(RT4Kir)irsend.sendNEC(0x49,0x01,2); // RT4K profile 9
      }
      else if(einput == "In10"){
        if(SVS==0 || SVS==2)Serial.println("remote prof10\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=10\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=10\r");
        }
        if(RT5Xir && !INPUT0){irsend.sendNEC(0xB3,0x87,2);delay(30);} // RT5X profile 10
        if(RT4Kir)irsend.sendNEC(0x49,0x25,2); // RT4K profile 10
      }
      else if(einput == "In11"){
        if(SVS==0 || SVS==2)Serial.println("remote prof11\r");
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=11\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=11\r");
        }
        if(RT4Kir)irsend.sendNEC(0x49,0x26,2); // RT4K profile 11
      }
      else if(einput == "In12"){
        if((SVS==0 && !INPUT0) || SVS==2)Serial.println("remote prof12\r"); // okay to use this profile if INPUT0 is disabled
        else if(SVS==1){
          Serial.println("SVS NEW INPUT=12\r");
          delay(1000);
          Serial.println("SVS CURRENT INPUT=12\r");
        }
        if(RT4Kir && !INPUT0)irsend.sendNEC(0x49,0x27,2); // RT4K profile 12
      }
      else if(einput != "In0 " && einput != "In00" && einput2 != "In0 " && einput2 != "In00"){
        Serial.print("SVS NEW INPUT=");
        Serial.print(einput.substring(2,4));
        Serial.println("\r");
        delay(1000);
        Serial.print("SVS CURRENT INPUT=");
        Serial.print(einput.substring(2,4));
        Serial.println("\r");
      }
        if((einput == "In0 " && einput == "In00") || INPUT0) // if input0 is true  - always save previous state  
          prevzero = einput;                               // if input0 is false - only save when all ports are in-active         
    }
    
    // listens to 2nd Extron Serial Port for changes
    ecap2 = extronSerial2.readString().substring(0,10); // read in and store only the first 10 chars for every Extron status message received on 2nd Extron Connection
    if(ecap2.substring(0,3) == "Out"){ // store only the input and output states, some Extron devices report output first instead of input
      einput2 = ecap2.substring(6,10);
      eoutput2 = ecap2.substring(3,5);
    }
    else{                             // less complex switches only report input status
      einput2 = ecap2.substring(0,4);
      eoutput2 = "00";
    }

    // use remaining results to see which input is now active and change profile accordingly
    if(einput2.substring(0,2) == "In" && voutMatrix[eoutput2.toInt()+32]){
    if(einput2 != "In0 " && einput2 != "In00"){
      Serial.print("SVS NEW INPUT=");
      if(einput2.substring(3,4) == " ")
        Serial.print(einput2.substring(2,3).toInt()+100);
      else
        Serial.print(einput2.substring(2,4).toInt()+100);
      Serial.println("\r");
      delay(1000);
      Serial.print("SVS CURRENT INPUT=");
      if(einput2.substring(3,4) == " ")
        Serial.print(einput2.substring(2,3).toInt()+100);
      else
        Serial.print(einput2.substring(2,4).toInt()+100);
      Serial.println("\r");
      }
        if((einput2 == "In0 " && einput2 == "In00") || INPUT0) // if input0 is true  - always save previous state  
          prevzero2 = einput2;                               // if input0 is false - only save when all ports are in-active
    }


    if(prevzero == "0" && prevzero2.substring(0,2) == "In")prevzero = "In00"; // used to keep track of non-active inputs when 2 switches are connected
    if(prevzero2 == "0" && prevzero.substring(0,2) == "In")prevzero2 = "In00";

    // debug code
    //Serial.print("prev zero :");Serial.println(prevzero);
    //Serial.print("prev zero2:");Serial.println(prevzero2);
    
    // if INPUT0 is enabled and all inputs are in-active
    if(((prevzero == "In0 " || prevzero == "In00") && (prevzero2 == "In0 " || prevzero2 == "In00" || !EXTRON2)) && INPUT0 && voutMatrix[eoutput.toInt()] && (!EXTRON2 || voutMatrix[eoutput2.toInt()+32])){
      if(SVS==0)Serial.println("remote prof12\r");
      else if(SVS==1 || SVS==2){
        Serial.println("SVS NEW INPUT=0\r");
        delay(1000);
        Serial.println("SVS CURRENT INPUT=0\r");
      }
      if(RT5Xir){irsend.sendNEC(0xB3,0x87,2);delay(30);} // RT5X profile 10
      if(RT4Kir)irsend.sendNEC(0x49,0x27,2); // RT4K profile 12

      prevzero = "0";
      prevzero2 = "0";

    }
    
  delay(250);
} // end of void loop
