# Please Notice:
  - It is recommended to place a 10k resistor between the Arduino's Tx and RT4K's Rx. In the guide, this could easily be done between the Arduino's Tx and 3.5mm jack. This is the prevent the RT4K from being backpowered by the Arduino when it's power is off.
  - No need for wiring "Rx" in the picture guides below as it has no current purpose. You can leave it disconnected.

## Otaku Games Scart Switch


RetroTink 4K HD15 Serial Control using an Arduino Nano, Integrated with a 10 Port Otaku Games Scart Switch. The code is very basic at the moment, but allows executing multiple commands to the RT4K (even with delays inbetween) if needed. Please feel free to fork / create PRs / etc. 

See it in action: https://youtu.be/L7HAki0e440

This implementation taps into 2x ULN2003 Darlington Arrays that are used to activate a led and relays for the LAST ACTIVE scart port. Each individual led/relay line is connected to an input pin on the Arduino. If an input pin on the Arduino detects a voltage drop, then a command is sent to the RetroTink telling it to change to that pin's / scart ports assigned remote profile. This does not touch the Scart signal or MCU and should not affect image quality. This is all based on my general observation of the PCB, I don't have any official documentation to go by. 



## Nano [5v tolerant] (Recommended)
![nano1](https://github.com/user-attachments/assets/9f876e67-234e-41ba-a159-b656da90db71)
![nano2](https://github.com/user-attachments/assets/5dbc1f2d-29f7-4366-87cd-094020f40539)
![nano3](https://github.com/user-attachments/assets/f8a9ece3-6345-4fc6-aa8f-c3bf7cd51a53)

## Pro Micro [not 5v tolerant] must use voltage divider as shown
![IMG_7124](https://github.com/user-attachments/assets/133d5b34-9a24-4d79-8ec8-e0fd2d12612a)
![IMG_7125](https://github.com/user-attachments/assets/cb80afc6-6046-4799-8f1e-254dff0abf41)
![IMG_7126 2](https://github.com/user-attachments/assets/e4909073-4bfe-493b-b2da-54f2a3deb295)
Standard 3.5mm stereo jack and cable used to connect tx,rx,gnd lines to HD15. Pay attention to Tx/Rx. It's very easy to get mixed up. Ex: Tx out of Nano goes to Rx on HD15, vice versa.
![IMG_7127](https://github.com/user-attachments/assets/cab39bfc-166b-4c0b-815d-2e5f10645496)
![IMG_7128](https://github.com/user-attachments/assets/fbd2362e-8122-4716-ba14-3989fca82af9)
