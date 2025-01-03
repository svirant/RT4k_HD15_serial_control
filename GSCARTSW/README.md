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
