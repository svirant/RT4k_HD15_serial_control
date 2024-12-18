# RetroTink Auto Profile switching 
### With an Arduino, this project showcases many ways to integrate your current console switching setup with the RetroTink 4K and/or RetroTink 5x for Auto Profile switching.
**So far, includes:** Extron devices, GSCART/GCOMP switches, and an Otaku Games Scart Switch



--------
***<Updates:>***
- Confirmed: Arduino Nano works in an Otaku Games Scart Switch. updated guide photos below.
- It keeps evolving! New Donut Dongle will add gscart/gcomp support. 
- [Extron Donut Dongle & Adapters coming soon!](https://github.com/svirant/RT4k_HD15_serial_control/tree/main?tab=readme-ov-file#extron-donut-donglecoming-soon)
- Extron: Now supports **2x** simultaneous connected switches with each allowed **99 inputs / 32 outputs** (I don't think more than 32 inputs are necessary but Extron's SIS supports it)
- GSCART/GCOMP: Added **SVS** profile support and **RT5X** support via IR LED addon. Wiring diagram below. Only for Nano atm. Looking into easier implementation.
- Big update for Extron devices:
  - **RT5X** IR LED support, RT4K IR LED support as well 
  - supports up to 16 inputs w/ 16 selectable outputs for Matrix switchers
  - remote or SVS profiles, or a mix of both
  - option to auto load profile when no inputs are active
- most Extron devices should now be supported: MVX, Crosspoint, anything that uses SIS
- Extron RGBHV code added with guide notes below
- gscartsw / gcompsw EXT implementation has been confirmed to work
- added **"Scart Off Profile"** option that will load Remote Profile 12 when all inputs are off.  You can assign it to a HDMI input profile for example. Default is off. This is currently only implemented for <ins>rt4k_serial</ins> and <ins>rt4k_serial_nano</ins>.
##
> [!CAUTION]
**DISCLAIMER**</red>
- GSCARTSW / GCOMPSW - Do not connect usb power to the Arduino AFTER it's wired in unless first disconnecting either EXT pin 4 (5v) or RAW on the Arduino.  Please Upload/Program the Arduino BEFORE connecting it to the EXT pins.
- Otaku Games Scart Switch - Do not connect usb power to the Arduino (AFTER it's wired in) and the switch at the same time. If Programming the Arduino, please make sure the Switch's USB power cable is first disconnected.
--------
## Programming an Arduino (Nano or Pro Micro)
I recommend the Official Arduino IDE and guide if you're unfamiliar with Arduinos.
- https://www.arduino.cc/en/Guide
- All .ino files used for programming the Arduino are listed above
--------

## GSCARTSW / GCOMPSW install notes

I recommend first programming your Arduino before starting

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

### IR LED Wiring
Can run lengthy wire between LED pins and rest of connecting circuit. Looking into easier implementation.
![IR_LED_Wiring](https://github.com/user-attachments/assets/c56fb2b4-cd95-4b97-8e89-5528a1bd5bcc)


--------
## Extron Switches
- Any Extron switch that supports SIS should work. MVX, Crosspoint, RGBHV, etc
- Tested and confirmed operation on RGBHV SW6 BNC switch in auto & manual mode
### Parts needed:
- MAX3232 TTL Converter
  - Connects to Extron Serial port
  - Voltage swing on Extron TX pin can be up to 18v, that's why you need this part to read and relay to the Arduino
  - https://www.amazon.com/dp/B07LBDZ9WG is what I used
- Arduino Nano
- Permanent power source for Arduino. Nowhere to wire this one in. :-/
- If connecting a 2nd Extron Switch you must follow these steps:
   - Step 1 - Goto: https://github.com/PaulStoffregen/AltSoftSerial 
   - Step 2 - Click the GREEN "<> Code" box and "Download ZIP"
   - Step 3 - In Arudino IDE; goto "Sketch" -> "Include Library" -> "Add .ZIP Library"
   - Step 4 - Connect the following pins to a 2nd Max3232
     - 2nd Max3232 Rx = Arduino Nano D8 pin
     - 2nd Max3232 Tx = Arduino Nano D9 pin
     - Power / Gnd to same spots as first Max3232 is fine


For now I also have a picture guide on how to MacGuyver one together.
![IMG_7138](https://github.com/user-attachments/assets/daee57b8-abd5-4a77-9df6-ac2364d38ac5)

--------
# Extron "Donut" Dongle(coming soon)
## Features:
- Connects to RT4K adapters below with standard 3.5mm stereo cable (Serial Rx/Tx)
- 2 integrated Max3232 Serial DB9 to connect to 2x Extron Switches
- 2 connectors for gscart/gcomp hookups. 2x4 ribbon cable connection. pogo pins would be nice for the gscart side.
- Infrared Emitter 3.5mm jack for RetroTink5x Profile switching
<img width="732" alt="donut3" src="https://github.com/user-attachments/assets/3dae5478-4716-4659-8e4d-2a8a518f4599" />


## & Adapters

- Enhanced S-Video & Composite video w/ Serial input

<img width="200" alt="adapter_yc" src="https://github.com/user-attachments/assets/0cbef12b-5f54-44d3-b2f0-800c544cf2a3" />


- VGA Passthrough w/ Serial input

<img width="200" alt="adapter_vga" src="https://github.com/user-attachments/assets/82b86c8f-e67a-4961-9358-ad429d2acb68" />








--------
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


## Nano [5v tolerant] (confirmed working)
No need for all the extra voltage divider resistors! Hurrah!
![nano1](https://github.com/user-attachments/assets/9f876e67-234e-41ba-a159-b656da90db71)
![nano2](https://github.com/user-attachments/assets/5dbc1f2d-29f7-4366-87cd-094020f40539)
![nano3](https://github.com/user-attachments/assets/f8a9ece3-6345-4fc6-aa8f-c3bf7cd51a53)

