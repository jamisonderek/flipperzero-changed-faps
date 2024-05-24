# Arkanoid

This is a sample arkanoid game.  You control a paddle and try to hit the ball to break the bricks.  The game is won when all the bricks are broken.

Modifications to the original game by CodeAllNight (MrDerekJamison):
- Added support for Atari 2600 paddle controllers (example of Analog to Digital conversion on the Flipper Zero).
- Added logo screen showing "2600", "Air" or empty when starting the game.
- Changed the application icon, name and app_id.

## Controls

### Using Atari 2600 paddle controllers
- Turn the paddle controller to the far left before loading game.  The logo should show "2600".
- Move the paddle left and right by turning the paddle controller
- Start the game by pressing the fire button

### Using VGM (Video Game Module)
- Disconnect the Atari 2600 paddle controller.
- Connect the VGM to the Flipper Zero before loading the game.  The logo should show "AIR".
- Move the paddle left and right by tilting the VGM left and right
- Start the game by pressing the OK button

### Using D-Pad
- Disconnect both the Atari 2600 paddle controller and the VGM before loading the game.
- Left Arrow: Move the paddle left
- Right Arrow: Move the paddle right
- Ok: Start the game

## Atari 2600
You will need:
- [Breadboard](https://www.amazon.com/HiLetgo-SYB-170-Breadboard-Colorful-Plates/dp/B071KCZZ4K)
- 2x [220K ohm resistors](https://www.amazon.com/ALLECIN-4W-Film-Resistor-Kit/dp/B0CDWW5BFH)
  - or [Decade resistor board set to 440K](https://www.amazon.com/gp/product/B094F7ZPL3)
- [Dupont wires](https://www.amazon.com/Elegoo-EL-CP-004-Multicolored-Breadboard-arduino/dp/B01EV70C78) (male/female and male/male)
- [9-pin D-sub connector with breakout terminals](https://www.amazon.com/Connector-Adapters-Adapter-Terminal-Screwdriver/dp/B093PFX62L)
- [Atari 2600 paddle controllers](https://www.amazon.com/CX30-Paddle-Pack-Not-Machine-Specific/dp/B0CTJ6TW43)

Optional:
- [Video Game Module](https://shop.flipperzero.one/products/video-game-module-for-flipper-zero)
- [HDMI cable](https://www.amazon.com/Cable-Capshi-Speed-18Gbps-Ethernet/dp/B07FFS7RH1)

### Wiring
![Atari 2600 Paddle Controller](./2600.png)
On the breadboard there are rows of 5 pins that are all connected together.  The columns are not connected. The rows on the other side are also connected together, but the two rows are not connected to each other.  We will use one row for GND and then a few columns over, we will use another row for VCC.  Then we will use a row for C1 (which is our button) and a row for C0 (which is our paddle controller).  For simplicity, we will put the C1 row on the other side of the breadboard from the C0 row; leaving a big gap in case you want to add your resistors between C0 and VCC.

- D-sub connector, pin 8 to GND on breadboard (or direct to GND on FZ). **Blue wire**
- D-sub connector, pin 7 to GND on breadboard (or direct to GND on FZ). **Blue wire**
- D-sub connector, pin 5 to C0 on breadboard. **White wire**
- D-sub connector, pin 4 to C1 on breadboard (or direct to C1 on FZ). **Yellow wire**

- Connect the 2-220K ohm resistors in series (making a 440K ohm resistor).
  - Connect one end of resistor to VCC on breadboard.  **Red wire**
  - Connect the other end of other resistor to C0 on breadboard. **White wire**

- Connect VGM VCC to VCC on breadboard. **Red wire**
- Connect VGM GND to GND on breadboard (or direct to GND on FZ). **Blue wire**

- Connect VGM TX to TX on Flipper Zero (we are extending the TX wire, so don't swap with RX like you would for UART communication). **Purple wire**
- Connect VGM RX to RX on Flipper Zero. **Grey wire**

- Connect the Atari 2600 paddle controller to the D-sub connector.

### Building

To build the application, you will need to clone your firmware.  Then copy the `air_arkanoid` folder to the `applications_user` folder in the firmware.  You can then build the firmware as normal.

Connect your Flipper Zero, close qFlipper and run the following commands in a Command Window (cmd):
```cmd
git clone --recursive https://github.com/flipperdevices/flipperzero-firmware.git
git clone --recursive https://github.com/jamisonderek/flipperzero-changed-faps.git
cd flipperzero-firmware
cd applications_user
xcopy -e ../../flipperzero-changed-faps/air_arkanoid .
cd ..
fbt COMPACT=1 DEBUG=0 FORCE=1 flash_usb_full
```

## Support
Support for the modifications can be found on my [Discord server](https://discord.com/invite/NsjCvqwPAd).