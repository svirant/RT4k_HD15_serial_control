# RetroTink Auto Profile switching 
### With an Arduino, this project showcases many ways to integrate your current console switching setup with the RetroTink 4K and/or RetroTink 5x for Auto Profile switching.
**So far, includes:** TESmart HDMI switch, Extron devices, GSCART/GCOMP switches, and an Otaku Games Scart Switch



--------
***<Updates:>***
- final? VGA Passthrough Adapter / Donut Dongle proto pcb in the mail. Once verified, I'll post them here.
- YC2VGA "shorty" gerber added
- More "Turbo" remote features added.
  - AUX8 button + profile button 1 - 12 loads SVS profiles of your choosing. By default is SVS 1 - 12
  - AUX8 pressed twice, manually enter a SVS profile to load with the profile buttons using 1 - 9 and 10,11,12 buttons for 0. Must use 3 digits. Ex: 001 = 1, 010 = 10, etc
- TESmart HDMI switch support added. being added here soon.
- [RT4K Turbo Remote:](https://github.com/svirant/RT4k_HD15_serial_control/tree/main/README.md#turbo-remote-picture-guide) Intercepts the remote's button presses and relays them through the Serial interface giving a much more responsive experience.
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

--------
## Programming an Arduino (Nano or Pro Micro)
I recommend the Official Arduino IDE and guide if you're unfamiliar with Arduinos.
- https://www.arduino.cc/en/Guide
- All .ino files used for programming the Arduino are listed above


### IR LED Wiring
Can run lengthy wire between LED pins and rest of connecting circuit. Looking into easier implementation.
![IR_LED_Wiring](https://github.com/user-attachments/assets/c56fb2b4-cd95-4b97-8e89-5528a1bd5bcc)



