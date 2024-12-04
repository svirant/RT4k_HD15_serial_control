# RT4k HD15 serial control
***<Updates:>***
- Extron RGBHV code added with guide notes below
- gscartsw / gcompsw EXT implementation has been confirmed to work
- added **"Scart Off Profile"** option that will load Remote Profile 12 when all inputs are off.  You can assign it to a HDMI input profile for example. Default is off. This is currently only implemented for <ins>rt4k_serial</ins> and <ins>rt4k_serial_nano</ins>.
##
> [!CAUTION]
**DISCLAIMER**</red>
- GSCARTSW / GCOMPSW - Do not connect usb power to the Arduino AFTER it's wired in unless first disconnecting either EXT pin 4 (5v) or RAW on the Arduino.  Please Upload/Program the Arduino BEFORE connecting it to the EXT pins.
- Otaku Games Scart Switch - Do not connect usb power to the Arduino (AFTER it's wired in) and the switch at the same time. If Programming the Arduino, please make sure the Switch's USB power cable is first disconnected.
 
## GSCARTSW / GCOMPSW install notes
### Program Arduino (Pro Micro or Nano)
I recommend the Official Arduino IDE and guide if you unfamiliar with Arduinos.
- https://www.arduino.cc/en/Guide

### Connect RT4K to Arduino
- RT4K HD15 pin 15 = RX --> Arduino TX0 pin
- RT4K HD15 pin 12 = TX --> Arduino RX1 pin
- RT4K HD15 pin 5 = Gnd --> Arduino Gnd pin (any Gnd pin is fine)

### Connect EXT pins to Arduino
- gscartsw / gcompsw EXT pin 5 ---> Pin A2 (Pro Micro) or Pin D5 (Nano)
- gscartsw / gcompsw EXT pin 6 ---> Pin A1 (Pro Micro) or Pin D6 (Nano)
- gscartsw / gcompsw EXT pin 7 ---> Pin A0 (Pro Micro) or Pin D7 (Nano)

  Pins 5-7 represent selected input in binary.

### Power / Gnd
- EXT pin 1 (Gnd) ---> any Arduino GND pin 
- EXT pin 4 (5v)  ---> RAW (Pro Micro) or VIN (Nano)
- Read disclaimer above :)

https://shmups.system11.org/viewtopic.php?p=1307320#p1307320
### gscartsw / gcompsw EXT pinout:
- Pin 1: GND
- Pin 2: Override
- Pin 3: N/C
- Pin 4: +5V
- Pin 5: IN_BIT0
- Pin 6: IN_BIT1
- Pin 7: IN_BIT2
- Pin 8: N/C

## Extron RGBHV Series
### Parts needed:
- MAX232 TTL Converter
  - Connects to Extron Serial port
  - Voltage swing on Extron TX pin can be up to 18v, that's why you need this part to read and relay to the Arduino
  - https://www.amazon.com/dp/B07LBDZ9WG is what I used
- Arduino Nano or Pro Micro
- Permanent power source for Arduino. Nowhere to wire this one in. :-/

In the future it would be nice to have a custom PCB for all this... hmm...  


![IMG_7138](https://github.com/user-attachments/assets/89e9c99d-5797-4d88-83fd-f8a4e5b9f5fc)


## Otaku Games Scart Switch
RetroTink 4K HD15 Serial Control using an Arduino Pro Micro or Nano, Integrated with a 10 Port Otaku Games Scart Switch. The code is very basic at the moment, but allows executing multiple commands to the RT4K (even with delays inbetween) if needed. Please feel free to fork / create PRs / etc. 

See it in action: https://youtu.be/L7HAki0e440

This implementation taps into 2x ULN2003 Darlington Arrays that are used to activate a led and relays for the LAST ACTIVE scart port. Each individual led/relay line is connected to an input pin on the Arduino. If an input pin on the Arduino detects a voltage drop, then a command is sent to the RetroTink telling it to change to that pin's / scart ports assigned remote profile. This does not touch the Scart signal or MCU and should not affect image quality. This is all based on my general observation of the PCB, I don't have any official documentation to go by. 

![IMG_7124](https://github.com/user-attachments/assets/133d5b34-9a24-4d79-8ec8-e0fd2d12612a)

## Pro Micro (confirmed working)
![IMG_7125](https://github.com/user-attachments/assets/cb80afc6-6046-4799-8f1e-254dff0abf41)

![IMG_7126 2](https://github.com/user-attachments/assets/e4909073-4bfe-493b-b2da-54f2a3deb295)
Standard 3.5mm stereo jack and cable used to connect tx,rx,gnd lines to HD15

![IMG_7127](https://github.com/user-attachments/assets/cab39bfc-166b-4c0b-815d-2e5f10645496)

![IMG_7128](https://github.com/user-attachments/assets/fbd2362e-8122-4716-ba14-3989fca82af9)


## Nano [5v tolerant] (should work, but not confirmed yet)
No need for all the extra voltage divider resistors! Hurrah!
![nano](https://github.com/user-attachments/assets/790f8e01-e7bd-4262-bc83-e47fe5e16e1c)

