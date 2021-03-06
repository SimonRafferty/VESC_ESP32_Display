# VESC_ESP32_Display
This is just a simple display of Speed, Battery Percentage, Power being delivered to the motor and battery voltage on the nice little TTGO Display boards which you can pick up for about $10.

![alt text](https://github.com/SimonRafferty/VESC_ESP32_Display/blob/main/Display.jpg)

The TTGO Board is based on the ESP32 architecture with WiFi, Bluetooth etc but has a very nice, IPS TFT Display on board.  For the price, they are unbeatable!

I've included a 3D Printed case, intended to mount to bicycle Handlebars.  The case could be a lot smaller - but it suited my arrangement.
The top two layers of the case, sandwich a piece of clear plastic. I cut a bit out of one of those crystal boxes you get assortments of components in.  Once you've configured the settings, seal the three layers of the case together with Silicone Mastic and it will be waterproof.  If you want to be able to dismantle it after sealing, spread a thin layer of mastic on the mating surfaces of the bottom two layers and allow them to cure.  The screws provide enough clamping force to seal decently.

![alt text](https://github.com/SimonRafferty/VESC_ESP32_Display/blob/main/20210506_095833.jpg)

To connect to the VESC, the Tx & Rx UART connections connect to pins 26 & 25 on the ESP32 respectively.
The Hardware Serial UART pins on the ESC32 are configured in the code (line 61) so you can use whicever pins are most convenient.
+5V & 0V on the VESC to +5v & GND on the ESP32

Ping me an email if you have any queries - I'm happy to answer.
