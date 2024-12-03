# RT4k HD15 serial control

***<Updates:>***
- gscartsw / gcompsw EXT implementation has been confirmed to work
- added **"Scart Off Profile"** option that will load Remote Profile 12 when all inputs are off.  You can assign it to a HDMI input profile for example. Default is off. This is currently only implemented for <ins>rt4k_serial</ins> and <ins>rt4k_serial_nano</ins>.

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

