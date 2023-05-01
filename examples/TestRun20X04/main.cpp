/*!
	@file    main.cpp
	@author   Gavin Lyons
	@brief
		 This file contains the "main" function for  project, a set of test sequence
 		 to test the HD44780_LCD_PCF8574 pico rp2040  library.
		 This is for 20 column 4 row LCD
	@note
		-# Test 1 :: Hello world
		-# Test 2 :: Move the cursor test
		-# Test 3 :: Scroll the display test
		-# Test 4 :: Test GOTO method
		-# Test 5 :: Test clear a line method
		-# Test 6 :: Cursor type (4 off) and screen reset test, Cursor mode is changed with a reset.
		-# Test 7 :: Text entry mode (4 off) if screen is reset the entry mode will be reset to default
		-# Test 8 :: Print numerical data using print() method
		-# Test 9 :: Custom character's from the CGRAM test
		-# Test 10 :: Backlight test.
*/

// Section: Included library
// *** Libraries ***
#include <stdio.h>
#include "pico/stdlib.h"
#include "hd44780/HD44780_LCD_PCF8574.hpp"

// Section: Defines
#define DISPLAY_DELAY_1 1000
#define DISPLAY_DELAY_2 2000
#define DISPLAY_DELAY 5000

// *** Globals ***
HD44780LCD myLCD(LCD_I2C_ADDRESS, i2c0, 100, 16,17); // instantiate an object

// Section: Function Prototypes

void helloWorld(void);
void cursorMoveTest(void);
void scrollTest(void);
void gotoTest(void);
void clearLineTest(void);
void cursorTest(void);
void entryModeTest(void);
void writeNumTest(void);
void customChar(void);
void backLightTest(void);

// Section: Main Loop

int main()
{
  stdio_init_all(); // Initialize chosen serial port, default 38400 baud
  busy_wait_ms(1000);
  printf("HD44780 : Start!\r\n");
  myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn,4 ,20);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);

  while(true){
	helloWorld();
	cursorMoveTest();
	scrollTest();
	gotoTest();
	clearLineTest();
	cursorTest();
	entryModeTest();
	writeNumTest();
	customChar();
	backLightTest();
  };

} // End of Main

// Section :  Functions
void helloWorld(void) {
	char teststr1[] = "Hello";
	char teststr2[] = "World";
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberThree , 0);
	myLCD.PCF8574_LCDSendString(teststr2); // Display a string
	myLCD.PCF8574_LCDSendChar('!'); // Display a single character
	busy_wait_ms(DISPLAY_DELAY_1);
}

void cursorMoveTest(void) {
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2);
	busy_wait_ms(DISPLAY_DELAY);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveLeft, 2);
}

void scrollTest(void) {
	for (uint8_t i = 0; i < 10; i++) {
		myLCD.PCF8574_LCDScroll(myLCD.LCDMoveRight, 1);
		busy_wait_ms(DISPLAY_DELAY_2);
	}
	myLCD.PCF8574_LCDScroll(myLCD.LCDMoveLeft, 10);
	busy_wait_ms(DISPLAY_DELAY_2);
}

void gotoTest(void) {
  myLCD.PCF8574_LCDClearScreen();

	char teststr1[] = "Line 1";
	char teststr2[] = "Line 2";
	char teststr3[] = "Line 3";
	char teststr4[] = "Line 4";
	char testchar = '!';
	uint8_t columnPos = 0;
	uint8_t rowPos = 1;

	// Print a string to each line

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.PCF8574_LCDSendString(teststr2);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberThree , 0);
	myLCD.PCF8574_LCDSendString(teststr3);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberFour , 0);
	myLCD.PCF8574_LCDSendString(teststr4);
	busy_wait_ms(DISPLAY_DELAY);

 	myLCD.PCF8574_LCDClearScreen();

 	 // Print out ASCII table one character at a time in every position
 	 // with a unique goto command
 	for (rowPos = 1; rowPos < 5; rowPos++)
	{
		for (columnPos = 0 ;  columnPos <20 ; columnPos++)
		{
			myLCD.PCF8574_LCDGOTO((HD44780LCD::LCDLineNumber_e)rowPos, columnPos);
			myLCD.PCF8574_LCDSendChar(testchar++);
		}//Line 1
		busy_wait_ms(DISPLAY_DELAY_1);
 	}
	busy_wait_ms(DISPLAY_DELAY);
}

void clearLineTest(void)
{
	uint8_t rowPos = 1;
	 for (rowPos = 1; rowPos < 5; rowPos++)
	 {
		myLCD.PCF8574_LCDClearLine((HD44780LCD::LCDLineNumber_e)rowPos);
		busy_wait_ms(DISPLAY_DELAY_2);
	 }
}


void cursorTest(void) {
	char teststr1[] = "Cursor no 4";
	char teststr2[] = "Cursor no 1";
	char teststr3[] = "Cursor no 2";
	char teststr4[] = "Cursor no 3";

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeOnBlink); //type 4 cursor
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr1);
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeOff); //type 1 cursor
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr2);
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeBlink); //type 2 cursor
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr3);
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);

	myLCD.PCF8574_LCDResetScreen(myLCD.LCDCursorTypeOn); // Back to initial state , type 3
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 0);
	myLCD.PCF8574_LCDSendString(teststr4);
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearLine(myLCD.LCDLineNumberTwo);
}

void writeNumTest()
{
	int numPos = 193;
	int numNeg = -8582;
	double myPI = 3.1456;

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);
	myLCD.print(numPos);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0);
	myLCD.print(numNeg);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2);
	myLCD.print(myPI,3);

	busy_wait_ms(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0); // 11
	myLCD.print(11);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2);  // 13
	myLCD.print(11,OCT);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 0); // B
	myLCD.print(11, HEX);
	myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 2); // 1011
	myLCD.print(11,BIN);

	busy_wait_ms(DISPLAY_DELAY);
}

void customChar(void) {
	uint8_t index = 0; //  Character generator RAM location ,0-7 ,64 bytes
	
	// custom characters data to test custom character function
	uint8_t symbolData[8][8] = {
		{0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00}, // bell
		{0x02, 0x03, 0x02, 0x0E, 0x1E, 0x0C, 0x00, 0x00},  // Note
		{0x00, 0x0E, 0x15, 0x17, 0x11, 0x0E, 0x00, 0x00},  // clock
		{0x00, 0x0C, 0x1D, 0x0F, 0x0F, 0x06, 0x00, 0x00},  // duck
		{0x00, 0x01, 0x03, 0x16, 0x1C, 0x08, 0x00, 0x00},  // check
		{0x00, 0x1B, 0x0E, 0x04, 0x0E, 0x1B, 0x00, 0x00},  // cross
		{0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00},  // heart
		{0x01, 0x01, 0x05, 0x09, 0x1F, 0x08, 0x04, 0x00}   // return arrow
	};

	myLCD.PCF8574_LCDClearScreen();

	// Load the CGRAM with the data , custom characters
	// location argument must be 0 to 7  , load the data into LCD memory
 	 for (uint8_t index  = 0; index  < 8; index++) {
	 	 myLCD.PCF8574_LCDCreateCustomChar(index , symbolData[index]);
	}
 	 myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);

	// Print out custom characters from 
	// CGRAM locations 0-7 , location argument must be 0 to 7 
	for (index = 0; index < 8; index ++) {
		myLCD.PCF8574_LCDPrintCustomChar(index);
		myLCD.PCF8574_LCDMoveCursor(myLCD.LCDMoveRight, 1);
	}

	busy_wait_ms(DISPLAY_DELAY);
	myLCD.PCF8574_LCDClearScreen();
}

void backLightTest(void)
{
	char teststr4[] = "Back Light off";
	// Needs another command/data before it changes Light
	myLCD.PCF8574_LCDBackLightSet(false);

	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo , 1);
	myLCD.PCF8574_LCDSendString(teststr4);
	busy_wait_ms(DISPLAY_DELAY);
	myLCD.PCF8574_LCDBackLightSet(true);
	myLCD.PCF8574_LCDClearScreen();
}

void entryModeTest(void) {

	char teststr8[] = "1234";

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeOne);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // <-C4321
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeTwo);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // C4321->
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeFour);
	myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 8);
	myLCD.PCF8574_LCDSendString(teststr8); // <-1234C
	busy_wait_ms(DISPLAY_DELAY_2);
	myLCD.PCF8574_LCDClearScreenCmd();

	myLCD.PCF8574_LCDChangeEntryMode(myLCD.LCDEntryModeThree); // Set back to default entry mode
	myLCD.PCF8574_LCDClearScreenCmd();
	busy_wait_ms(DISPLAY_DELAY_1);
}

// *** EOF ***
