// RetroTink 4K Turbo Remote
//
// With an IR Receiver connected to the Arduino that has must better reception, RT4K IR remote control button presses get relayed through the RetroTink 4K's serial
// interface instead. Ultimately making the remote control much more responsive. Can also be used when the RetroTink's IR window is hidden or not easily reached by the 
// IR of the remote.
//
// Also includes the feature of using AUX8 as a modifier for button profiles 1 - 12. If you press AUX8 and then a profile 1 - 12 button, it will load SVS profiles 
// 301 - 312 in respect to the button number.
//
// TLDR; intercepts the remote's button presses and relays them through the Serial interface giving a much more responsive experience

#define IR_RECEIVE_PIN 12 // can use either pin 12 or 2, was unable to find another pin this worked on
#include "TinyIRReceiver.hpp"
int pwrtoggle = 0; // used to toggle remote power button on/off 
int repeatcount = 0;
int extrabuttonprof = 0;
String svsbutton = "";
int nument = 0;

void setup(){

  Serial.begin(9600);
  initPCIInterruptForTinyReceiver();

}

void loop(){

irRec();

} // end of loop()

void irRec(){

  int ir_recv_command = 0;
  int ir_recv_address = 0;

  if(TinyReceiverDecode()){

    ir_recv_command = TinyIRReceiverData.Command;
    ir_recv_address = TinyIRReceiverData.Address;
        
      //printTinyReceiverResultMinimal(&Serial);
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
