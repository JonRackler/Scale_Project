# Scale Project

Rebuild of a Transcell Model TCS3T-60LB

## Description

The scale's original controller board stopped working so I built a controller with two ATMega328 chips, one for the HX711 load cell amplifier and LCD and one chip as an keypad controller.
This project was coded on the Arduino IDE using the built in Wire library. The two ATMega devices are programmed with the ISP connection with a USBTiny programmer from SparkFun.

## Getting Started

### Dependencies

* HX711 library by bogde on Github available here https://github.com/bogde/HX711

### Installing

* Place the HX711 library in the libraries folder
* Program ATMega connected to LCD and load cell amp with My_scale_use
* Program ATMega connected to keypad with scale_keypad


## Version History

* 2.6
   * Initial Release

## License

This project is licensed under the GPLv3 License - see the LICENSE.txt file for details
