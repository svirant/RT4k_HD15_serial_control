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
