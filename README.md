# FTCArduinoBeacons
This repository is a group of Arduino sketches that have to do with the FTC beacons.

## beacon_imitate
We (ok, I) accidentally fried the micro controller on some of our beacons when I was trying to chain the power from all of them together. Since the LEDs were still working, I decided to replace the chips with Arduino microcontrollers rather than buy new boards.

The beacon_imitate sketch attempts to replicate the behavior of the normal beacon as closely as possible. The init sequence is a double flash of green to distinguish it from the normal beacons.

### Wiring
Use this guide at your own risk! Also, make sure the microcontroller is actually the problem and the LEDs are not broken. Since the LEDs are NeoPixels, there is no way to test them by hand.

The first step is to remove the old microcontroller from the board.

Put a solder blob on each row of pins, then heat both sides and pry it up with a knife.
Remove the solder, being sure to elimanate any connections between pins.

```
 o|o|o|o|o|o|o|                      o|o|o|o|o|o|o|                   o|o|o|o|o|o|o|
 |o|o|o|o|o|o|o                      |o|(        )o                   |o|o|o|o|o|o|o
 ,,,,,,,,,,,,,,             _        (____________)             _     ,,,,,,,,,,,,,,
----------------      _____| \      ----------------      _____| \
|              |     |        \     |              |     |        \
|O             |     |_____   /     |O             |     |_____   /
----------------           |_/      ----------------           |_/                    make sure
 ``````````````                      (            )                   ``````````````  nothing is
 o|o|o|o|o|o|o|                      (_________)|o|                   o|o|o|o|o|o|o|  connected
 |o|o|o|o|o|o|o                      |o|o|o|o|o|o|o                   |o|o|o|o|o|o|o       
```

The LEDs inside the beacons are NeoPixels, which are controlled by 1 wire. To connect the NeoPixels to the arduino, connect a wire to the bottom-left contact on the left-most LED.
```
 .+----+.  .+----+.  .+----+.                                     .+----+.  .+----+.  .+----+.
  |/  \|    |/  \|    |/  \|                                       |/  \|    |/  \|    |/  \|
  |\__/|    |\__/|    |\__/|                                       |\__/|    |\__/|    |\__/|
 .+----+.  .+----+.  .+----+.                                     .+----+.  .+----+.  .+----+.
 ^
Solder a wire here
```
That wire connects to pin 10 on the Arduino.

Then find the voltage regulator, and connect a wire from the middle pin (5V output) to the arduino 5V.
Connect a wire from ground (available on the input plug, among other places) to the arduino ground.

Then disconnect the buttons from the beacon board. Connect one wire from each button to Arduino or beacon board GND. This can be done by twisting them together and soldering a wire from them to GND.

Connect the other wires from each button to these pins on the Arduino:
```
Button    Arduino pin
Left      7
Back      8
Right     9
```

## beacon_improved
This code is similar to beacon_imitate, but it makes the LEDs fade from red to blue or vice versa instead of blinking. The init sequence is: fade in green, fade out green, fade in green, fade out green.

## beacon_custom
This sketch runs on a custom mod of the beacon that has a DIP switch with 18 switches. Each switch controls one RGB channel of one of the 6 LEDs.

The order is: red1 red2 red3 green1 green2 green3 blue1 blue2 blue3 red4 red5 red6 green4 green5 green6 blue4 blue5 blue6
