# Please Notice:
  - It is recommended to place a 10k resistor between the Arduino's Tx and RT4K's Rx if not adding a buffer for open-drain. In the guide, this could easily be done between the Arduino's Tx and 3.5mm jack. This is the prevent the RT4K from being backpowered by the Arduino when its power is off.
  - No need for wiring "Rx" in the picture guides below as it has no current purpose. You can leave it disconnected.
  - If you choose to forego the open-drain buffer and use just the Nano's Tx, it must be the only serial device connected to the RT4K's HD-15 serial port. You cannot use multiple push-pull serial devices at the same time via a headphone splitter like you can with open-drain. In short: you cannot mix and match push-pull and open-drain.

## Generic HDMI Switch 


RetroTink 4K HD15 Serial Control using an Arduino Nano, integrated with any generic HDMI switch. The code is very basic at the moment, but allows executing multiple commands to the RT4K (even with delays inbetween) if needed. Please feel free to fork / create PRs / etc. 

See it in action: https://youtu.be/L7HAki0e440

This implementation taps into the LEDs themselves. When an LED is lit, it naturally follows that the input is active. Each individual LED is connected to an input pin on the Arduino. If an input pin on the Arduino detects a HIGH signal (around 3.3V), then a command is sent to the RetroTINK telling it to change to the input assigned to that specific pin. This does not touch the HDMI signal or MCU and should not affect image quality. We do not have any specific documentation and the user assumes all risk for modifying their own devices. 

## Materials

- A soldering iron and a steady hand
- Wire. I used 28AWG stranded wire, but you can probably get away with any
- An Arduino Nano
- An assembled open drain board. You can find the .gerber [here](https://github.com/svirant/RT4k_HD15_serial_control/blob/main/Otaku%20Games%20Scart%20Switch/rt4k_serial_otaku_pcb_gerber.zip). To build it, you'll need a 10K Ohm 0805 resistor, a 0.1uF 0805 capacitor, and an SN74LVC2G07DBVR open drain buffer IC. You can find it [here](https://www.digikey.com/en/products/detail/umw/SN74LVC2G07DBVR/24889644)
- A M->F 3.5mm TRS cable
- A small hand drill
- BAT85 Diode (optional, but recommended)
- Hot air station (optional, but recommended)

## Example

The following will be a demonstration of how this works with two of my HDMI switches. Please ignore the fact that one of them has actual built-in TX/RX.

**Step 0:** Take your switch apart. Some of them may require you to remove feet or adhesives to get to the screws. I recommend using a hot air station (you may be able to get away with a hair dryer in a pinch) to loosen the adhesive. Otherwise, you may damage the adhesive and getting it back to how it was prior to disassembly. This is a pretty important step, so be sure to take it seriously.

**Step 1:** Once your switch is open, we need to locate a few things. The locations will vary per switch, but nearly always will be present:

- Space to put in your Arduino and open drain board. Something great here is that they don't necessarily have to be together and can be paired with wires.
- Any GND. Try to find one as close as where you want to place your Arduino, as GND should be as short as you can comfortably make it.
- Somewhere 5V is present. The switches pictured here are powered by 5V so this is decently simple. Be sure, however, to know what the voltage you're using to power the switch. If you're using a 12V barrel jack to power your switch, don't grab it from that area. You can power your switch and use a multimeter to find 5V.
- Your LEDs. With your multimeter, take a look at which leg has about 3.3V (which is the HIGH signal we want) when the LED is lit. Then, switch the input. If that leg is 0V (LOW) when it's no longer lit, that is the leg you should be soldering to. Note that in the picture, the multimeter displays 3.3V with the lit LED. I'm using one of the HDMI ports as a GND for the multimeter as it's the most convenient.

![Checking for 3.3V on Board 1](assets/1.jpeg)

**Step 2:** You'll need to solder a lead to your GND, your 5V, and each of your LEDs. If you're using a BAT85 diode, be sure to have the ring closer to the output side (towards where you'd put the Arduino), not the input side. This will make it so your switch powers the Arduino, but so that your Arduino does not power the switch in case you plug it into a computer.

![Wires soldered to the LEDs on Board 1](assets/7.jpeg)

What's best, right now, is to take a bit of a break and plan out what you're going to do. Where the wires will go to, without your iron touching the Arduino. In particular, you'll want to plan out where the LED cables will go to. These should be going to the digital pins in the Arduino. Plan out which pins you'll be using.
Once you're sure, you'll need to program your Arduino with the included .ino. Depending on your setup, you **will** need to change values in the .ino file. For example, the switch with four inputs has Input 1 going to D3, 2 to D4, 3 to D6, and 4 to D7. The switch with seven inputs has input 1 going to D3, 2 to D4, etc. In the switch statement in the .ino (and a couple other places), this is defined. Please check the comments at the top of the .ino file to see exactly where you need to change values.

**Step 3:** Once you've done this, solder them to the Arduino. Your 5V should go to 5V. Your GND should go to GND. Your LED pins should go to whichever digital pins (D) you planned out prior.

![Input wires soldered on Board 2](assets/3.jpeg)

Finally, find somewhere to place your open-drain board and wire it to your Arduino. If you got the one from this repository, it should be labelled for you. TX1 goes to TX1. 5V goes to 5V. GND goes to GND. On one of these boards, I used wires. On the other, I soldered it directly into the Arduino using the through holes and headers.

![All wires soldered on Board 1](assets/6.jpeg)

![All wires soldered on Board 2](assets/4.jpeg)

**Step 4:** Before anything else, test your work. You can do this a couple ways. The easiest is for you to plug in your Arduino to your computer, power your switch, and check the Serial Monitor as you swap the inputs. You should be seeing your commands send as you do. You can also test by plugging in the 3.5mm jack to a VGA serial breakout adapter to your RetroTINK, and checking if your commands work. Be sure to set up your SVS profiles or remote profiles for your tests to work.

**Step 5:** Finally: we'll want to get the output, the female side of the TRS cable, outside of the switch. To do this, we'll use the drill. There is no real "rule" here. Just find somewhere convenient. What I prefer doing is to drill a small hole at the very top of the back. This creates a "U" shape where you can just place your cable. If you do this, be sure to tie a knot in the cable right before it reaches the U, as it acts as a strain relief. In case you pull on the cable, you won't run the risk of anything happening to it.

**Step 6:** You're done! Close the shell, make sure the strain relief works well, and enjoy your auto-changing profiles

![Finished switch 2](assets/5.jpeg)