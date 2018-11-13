# Spritelet v2

A Spritelet is a small box with a 128 x 128 pixel LCD screen for playing animations.

The Spritelet v2 software includes four applications:

  *  Clouds: Clouds from a kingdom on shrooms drift past the screen.
  *  Carousel: The main application. Displays animations and static images from an SD card.
  *  dX: An overengineered electronic dice roller.
  *  Toasters: Kitchen appliances with wings fly past the screen.

To make a Spritelet you will need the following hardware:

  *  One of the supported microcontroller boards.
  *  [Adafruit 1.44" Color TFT LCD Display w/ MicroSD](https://www.adafruit.com/product/2088)
  *  [Adafruit LiPoly Backpack Addon for Pro Trinket](https://www.adafruit.com/product/2124)
  *  [Lithium Ion Polymer Battery 3.7V 150mAh](https://www.adafruit.com/product/1317)
  *  [Fast Vibration Sensor Switch](https://www.adafruit.com/product/1766)
  *  [SPDT Slide Switch](https://www.adafruit.com/product/805)
  *  [Thru-Hole 5-Way Navigation Switch](https://www.adafruit.com/product/504)
  *  [SD/MicroSD Memory Card](https://www.adafruit.com/product/1294)
  *  An interconnect board to connect all the above together.
  *  [3D-Printed Spritelet v2 Enclosure](https://www.shapeways.com/model/upload-and-buy/7529582)
  *  [4x Machine Screw #4-40](https://www.digikey.com/product-detail/en/pomona-electronics/4862/501-1531-ND/745091)

Any of these supported microcontroller boards can be used:

  *  [Arduino Nano](https://store.arduino.cc/usa/arduino-nano) (not recommended)
  *  [Teensy 3.1/3.2](https://www.pjrc.com/teensy/teensy31.html)
  *  [Teensy LC](https://www.pjrc.com/teensy/teensyLC.html)
  *  [Adafruit Pro Trinket](https://www.adafruit.com/product/2000)
  *  [Adafruit ItsyBitsy 32u4](https://www.adafruit.com/product/3677)
  *  [Adafruit ItsyBitsy M0 Express](https://www.adafruit.com/product/3727)

You can of course use any board supported by the Arduino IDE, but without an appropriate interconnect board you won't be able to make it into a finished product. Also, any board larger than an Arduino Nano will not fit inside the enclosure.

Assembly is easy except for the following caveats:

  *  Solder all other components *before* soldering the LCD display board! Otherwise the LCD display board will block soldering the other components.
  *  Separate the LCD display board from the interconnect board with electrical tape or a piece of cardboard to prevent the MicroSD card connector from shorting the navigation switch.
  *  On Arduino Nano boards, you will need to solder a wire from the VUSB hole of the interconnect board to the VCC pin of the USB connector.
  *  On Teensy boards, you will need to cut the jumper on the back to separate VIN from VUSB.
  *  On the LiPoly Backpack Addon, you will need to cut the trace between the two holes with a box around them to add an on/off switch.

The MicroSD card will need to contain the contents of the [userland](userland) directory. To add images and animations, use the SpriteLoader application (just dragging files into the SPRITES directory will not work).

## Arduino Nano

  *  Store link: https://store.arduino.cc/usa/arduino-nano
  *  Firmware: [spritelet2_protrinket](spritelet2_protrinket)
  *  Interconnect board files: [board-nano](board-nano)
  *  Interconnect board purchase link: https://aisler.net/p/DWZPRWWS
  *  On Arduino Nano boards, you will need to solder a wire from the VUSB hole of the interconnect board to the VCC pin of the USB connector.

TODO more stuff here

## Teensy 3.1/3.2/LC

  *  Store link (3.1/3.2): https://www.pjrc.com/teensy/teensy31.html
  *  Store link (LC): https://www.pjrc.com/teensy/teensyLC.html
  *  Firmware: [spritelet2_teensy](spritelet2_teensy)
  *  Interconnect board files: [board-teensy](board-teensy)
  *  Interconnect board purchase link: https://aisler.net/p/WDFHSKDK
  *  On Teensy boards, you will need to cut the jumper on the back to separate VIN from VUSB.

TODO more stuff here

## Adafruit Pro Trinket

  *  Store link: https://www.adafruit.com/product/2000
  *  Firmware: [spritelet2_protrinket](spritelet2_protrinket)
  *  Interconnect board files: [board-protrinket](board-protrinket)
  *  Interconnect board purchase link: https://aisler.net/p/WMPMXXAI

TODO more stuff here

## Adafruit ItsyBitsy 32u4

  *  Store link: https://www.adafruit.com/product/3677
  *  Firmware: [spritelet2_itsybitsy_32u4](spritelet2_itsybitsy_32u4)
  *  Interconnect board files: [board-itsybitsy](board-itsybitsy)
  *  Interconnect board purchase link: https://aisler.net/p/JNFEJXOJ

TODO more stuff here

## Adafruit ItsyBitsy M0 Express

  *  Store link: https://www.adafruit.com/product/3727
  *  Firmware: [spritelet2_itsybitsy_m0](spritelet2_itsybitsy_m0)
  *  Interconnect board files: [board-itsybitsy](board-itsybitsy)
  *  Interconnect board purchase link: https://aisler.net/p/JNFEJXOJ

TODO more stuff here
