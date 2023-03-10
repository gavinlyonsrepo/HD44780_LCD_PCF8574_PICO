[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)


![ lcd ](https://github.com/gavinlyonsrepo/pic_16F1619_projects/blob/master/images/LCDPCF.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Example files](#example-files)
  * [Software](#software)
  * [Output](#output)
  
Overview
--------------------
* Name : HD44780_LCD_PCF8574_PICO

* Description :

0. Library to support the HD44780 LCD , (I2C PCF8574 "backpack" interface) 
1. C++ library for the rp2040 PICO
2. Backlight, scroll, cursor and entry-mode control.
3. Custom character support + print class for numerical data.
4. Hardware I2C  using SDK functions.
5. Tested on size 16x02 + 20x04 (but may work on other sizes  eg 16X4 but not tested).
6. Can support both I2C ports. IC20 or IC21 selected by user.

* Toolchain
	1. Raspberry pi PICO RP2040
	2. SDK C++ compiler G++ for arm-none-eabi
	3. CMAKE , VScode

7. Port of my Arduino library at [link](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574).

* Author: Gavin Lyons

Example files
-------------------------

The example files are in example folder.
To build the one you want, edit the Cmaketxt file 
add_executable(${PROJECT_NAME} section, comment in one example file path and one only.
The default is the HelloWorld one.

1. examples/HelloWorld/main.cpp Basic usage.
2. examples/TestRun/main.cpp  Test sequence for 16x02 LCD.
3. examples/TestRun20X04/main.cpp Test sequence for 20x04 LCD.
  
Software
-------------------
There are four core files to library , The "print" super-class provides a way to print numerical and other data
to screen. 

1. HD44780_LCD_PCF8574.hpp
2. HD44780_LCD_PCF8574.cpp
3. HD44780_LCD_PCF8574_Print.hpp
4. HD44780_LCD_PCF8574_Print.cpp

The user can enable basic "printf" I2C debug messages by commenting in this line in 
LCD_SERIAL_DEBUG  in the HD44780_LCD_PCF8574.hpp file.
The I2C timeout is set to 50,000 uS and can also be adjusted in same file (LCD_I2C_DELAY).
Both I2C ports can be used, IC20 or IC21 selected by user. The I2C bus speed can also be set. 
For detailed graphical description of entry modes , cursor types, custom characters 
and more see here [link](http://dinceraydin.com/lcd/commands.htm)

Output
---------------------

Output  of custom character test in testrun example  file on 16x02 display.

![ pic ](https://github.com/gavinlyonsrepo/HD44780_LCD_RPI/blob/main/extras/image/custom_output.jpg)

20x04 display. 

![ pic2 ](https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574/blob/main/extras/image/2004.jpg)

