/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief Basic usage example test file for LCD library, "Hello World!"
	@note https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574_PICO
		-# NOTE (1) This is for 16 column 2 row LCD.
		-# HD44780_LCD_PCF8574_PICO project name
*/

// *** Libraries ***
#include <stdio.h>
#include "pico/stdlib.h"
#include "hd44780/HD44780_LCD_PCF8574.hpp"


// *** Globals ***
#define CLOCK_PIN 19
#define DATA_PIN  18
#define CLOCK_SPEED 100
#define I2C_ADDRESS 0x27
HD44780LCD myLCD(I2C_ADDRESS, i2c1, CLOCK_SPEED, DATA_PIN, CLOCK_PIN); 

// *** Main ***
int main()
{
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("HD44780 : Start!\r\n");

	//setup
	if(!myLCD.LCDInit(myLCD.LCDCursorTypeOn, 2, 16))
	{
		printf("Error : main : Failed to Init I2C!\r\n");
		return -1;
	}
	myLCD.LCDClearScreen();
	myLCD.LCDBackLightSet(true);
	myLCD.LCDGOTO(myLCD.LCDLineNumberOne, 0);

	//print Hello World!
	char testString[] = "Hello World";
	myLCD.LCDSendString(testString);
	myLCD.LCDSendChar('!');  // Display a single character
	busy_wait_ms(7000);

	// end test
	myLCD.LCDClearScreen();
	myLCD.LCDDeInit();
	printf("HD44780 : End!\r\n");
	return 0;
}
