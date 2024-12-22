// RetroTink 4K Turbo Remote
//
// With an IR Receiver connected to the Arduino that has must better reception, RT4K IR remote control button presses get relayed through the RetroTink 4K's serial
// interface instead. Ultimately making the remote control much more responsive. Can also be used when the RetroTink's IR window is hidden or not easily reached by the 
// IR of the remote.
//
// TLDR; intercepts the remote's button presses and relays them through the Serial interface giving a much more responsive experience

#define IR_RECEIVE_PIN 12
#include "TinyIRReceiver.hpp"
int pwrtoggle = 0; // used to toggle remote power button on/off 

void setup(){

  Serial.begin(9600);
  initPCIInterruptForTinyReceiver();

}

void loop(){

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

    if(ir_recv_address == 73 && TinyIRReceiverData.Flags != IRDATA_FLAGS_IS_REPEAT){ // block most buttons from being repeated when held
      if(ir_recv_command == 63){
        Serial.println(F("remote aux8\r"));
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
    else if(ir_recv_address == 73){ // allow directional buttons to be repeated when held down
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

} // end of loop()
