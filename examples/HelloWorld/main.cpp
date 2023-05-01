/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief Basic usage example test file for LCD library, "Hello  World"
	@note https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574_PICO
		-# NOTE (1) This is for 16 column 2 row LCD.
		-# HD44780_LCD_PCF8574_PICO project name
*/

// *** Libraries ***
#include <stdio.h>
#include "pico/stdlib.h"
#include "hd44780/HD44780_LCD_PCF8574.hpp"


// *** Globals ***
HD44780LCD myLCD(LCD_I2C_ADDRESS, i2c1, 100, 18,19); // instantiate an object

// *** Main ***
int main()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("HD44780 : Start!\r\n");

	//setup
	myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn, 2, 16);
	myLCD.PCF8574_LCDClearScreen();
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
	//print hello world!
	char testString[] = "Hello World";
	myLCD.PCF8574_LCDSendString(testString);
	myLCD.PCF8574_LCDSendChar('!');  // Display a single character

	while (true){busy_wait_ms(1000);}; // wait here forever
	
}