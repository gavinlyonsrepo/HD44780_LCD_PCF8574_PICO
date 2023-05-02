

// Section : Includes
#include <stdio.h> // optional for printf debug messages
#include "pico/stdlib.h"
#include "../include/hd44780/HD44780_LCD_PCF8574.hpp"

/*!
	@brief Constructor for class HD44780LCD
	@param I2Caddress  The PCF8574 I2C address, default is 0x27.
	@param i2c_type  I2C instance of port IC20 or I2C1
	@param CLKspeed I2C Bus Clock speed in KHz. Set to 100
	@param SDApin I2C Data pin 
	@param SCLKpin I2C Clock pin   
*/
HD44780LCD  :: HD44780LCD(uint8_t I2Caddress, i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t  SDApin, uint8_t  SCLKpin)
{
	_LCDSlaveAddresI2C  = I2Caddress;
	i2c = i2c_type; 
	_SClkPin = SCLKpin;
	_SDataPin = SDApin;
	_CLKSpeed = CLKspeed;
}


// Section : Methods
/*!
	@brief  Send data byte to  LCD via I2C
	@param data The data byte to send
	@note if _LCDSerialDebugFlag is true, will output data on I2C failures.
*/
void HD44780LCD::PCF8574_LCDSendData(unsigned char data) {
		// I2C MASK Byte = DATA-led-en-rw-rs (en=enable rs = reg select)(rw always write)
	const uint8_t LCDDataByteOn= 0x0D; //enable=1 and rs =1 1101  DATA-led-en-rw-rs
	const  uint8_t LCDDataByteOff = 0x09; // enable=0 and rs =1 1001 DATA-led-en-rw-rs
	
	unsigned char dataNibbleLower, dataNibbleUpper;
	uint8_t dataBufferI2C[4];
	int I2CReturnCode = 0;
	
	dataNibbleLower = (data << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	dataNibbleUpper = data & 0xf0; //select upper nibble
	dataBufferI2C[0] = dataNibbleUpper | (LCDDataByteOn & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	dataBufferI2C[1] = dataNibbleUpper | (LCDDataByteOff & _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs
	dataBufferI2C[2] = dataNibbleLower | (LCDDataByteOn & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
	dataBufferI2C[3] = dataNibbleLower | (LCDDataByteOff &  _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs

	I2CReturnCode = i2c_write_timeout_us(i2c, _LCDSlaveAddresI2C, dataBufferI2C, 4 , false, _LCDI2Cdelay);
	if (I2CReturnCode < 1)
	{
		if (_LCDSerialDebugFlag == true){
			printf("1203 data: \r\n");
			printf("I2C error i2c_write_blocking: \r\n");
			printf("I2CReturnCode : %d \r\n", I2CReturnCode );
			busy_wait_ms(100);
		}
	}
}

/*!
	@brief  Send command byte to  lcd
	@param cmd command byte
	@note if _LCDSerialDebugFlag == true  ,will output data on I2C failures.
*/
void HD44780LCD::PCF8574_LCDSendCmd(unsigned char cmd) {
	// I2C MASK Byte = COMD-led-en-rw-rs (en=enable rs = reg select)(rw always write)
	const uint8_t LCDCmdByteOn = 0x0C;  // enable=1 and rs =0 1100 COMD-led-en-rw-rs 
	const uint8_t LCDCmdByteOff = 0x08; // enable=0 and rs =0 1000 COMD-led-en-rw-rs 
	
	unsigned char cmdNibbleLower, cmdNibbleUpper;
	uint8_t cmdBufferI2C[4];
	int I2CReturnCode = 0;
	
	cmdNibbleLower = (cmd << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
	cmdNibbleUpper = cmd & 0xf0; //select upper nibble
	cmdBufferI2C[0] = cmdNibbleUpper | (LCDCmdByteOn & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmdBufferI2C[1] = cmdNibbleUpper | (LCDCmdByteOff & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
	cmdBufferI2C[2] = cmdNibbleLower | (LCDCmdByteOn & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
	cmdBufferI2C[3] = cmdNibbleLower | (LCDCmdByteOff & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
	
	I2CReturnCode = i2c_write_timeout_us(i2c, _LCDSlaveAddresI2C, cmdBufferI2C, 4 , false, _LCDI2Cdelay);
	if (I2CReturnCode < 1)
	{
		if (_LCDSerialDebugFlag == true){
			printf("1202 cmd : \r\n");
			printf("I2C error  i2c_write_blocking : \r\n");
			printf("I2CReturnCode : %d \r\n", I2CReturnCode );
			busy_wait_ms(100);
		}
	}

}

/*!
	@brief  Clear a line by writing spaces to every position
	@param lineNo LCDLineNumber_e enum lineNo  1-4
*/
void HD44780LCD::PCF8574_LCDClearLine(LCDLineNumber_e lineNo) {

	switch (lineNo) {
		case LCDLineNumberOne:PCF8574_LCDSendCmd(LCDLineAddressOne); break;
		case LCDLineNumberTwo:PCF8574_LCDSendCmd(LCDLineAddressTwo); break;
		case LCDLineNumberThree:
			switch (_NumColsLCD)
			{
				case 16: PCF8574_LCDSendCmd(LCDLineAddress3Col16); break;
				case 20: PCF8574_LCDSendCmd(LCDLineAddress3Col20); break;
			}
		break;
		case LCDLineNumberFour:
			switch (_NumColsLCD)
			{
				case 16: PCF8574_LCDSendCmd(LCDLineAddress4Col16); break;
				case 20: PCF8574_LCDSendCmd(LCDLineAddress4Col20); break;
			}
		break;
	}

	for (uint8_t i = 0; i < _NumColsLCD; i++) {
		PCF8574_LCDSendData(' ');
	}
}

/*!
	@brief  Clear screen by writing spaces to every position
	@note : See also LCDClearScreenCmd for software command  clear alternative.
*/
void HD44780LCD::PCF8574_LCDClearScreen(void) {
	if (_NumRowsLCD < 1 || _NumRowsLCD >4){return;}

	PCF8574_LCDClearLine(LCDLineNumberOne);
	
	if (_NumRowsLCD >= 2)
		PCF8574_LCDClearLine(LCDLineNumberTwo);
	if (_NumRowsLCD >= 3)
		PCF8574_LCDClearLine(LCDLineNumberThree);
	if (_NumRowsLCD == 4)
		PCF8574_LCDClearLine(LCDLineNumberFour);
}


/*!
	@brief  Reset screen
	@param CursorType LCDCursorType_e enum cursor type, 4 choices
*/
void HD44780LCD::PCF8574_LCDResetScreen(LCDCursorType_e CursorType) {
	PCF8574_LCDSendCmd(LCDModeFourBit);
	PCF8574_LCDSendCmd(LCDDisplayOn);
	PCF8574_LCDSendCmd(CursorType);
	PCF8574_LCDSendCmd(LCDClearScreen);
	PCF8574_LCDSendCmd(LCDEntryModeThree);
	busy_wait_ms(5);
}


/*!
	@brief  Turn Screen on and off
	@param OnOff  True = display on , false = display off
*/
void HD44780LCD::PCF8574_LCDDisplayON(bool OnOff) {
	OnOff ? PCF8574_LCDSendCmd(LCDDisplayOn) : PCF8574_LCDSendCmd(LCDDisplayOff);
	busy_wait_ms(5);
}


/*!
	@brief  Initialise LCD
	@param cursorType  The cursor type 4 choices.
	@param NumRow number of rows on LCD
	@param NumCol number of columns on LCD
*/
void HD44780LCD::PCF8574_LCDInit(LCDCursorType_e cursorType, uint8_t NumRow, uint8_t NumCol) {

	_NumRowsLCD = NumRow;
	_NumColsLCD = NumCol;

	while (PCF8574_LCD_I2C_ON() != true)
	{
		busy_wait_ms(3000);
	}
	
	busy_wait_ms(15);
	PCF8574_LCDSendCmd(LCDHomePosition);
	busy_wait_ms(5);
	PCF8574_LCDSendCmd(LCDHomePosition);
	busy_wait_ms(5);
	PCF8574_LCDSendCmd(LCDHomePosition);
	busy_wait_ms(5);
	PCF8574_LCDSendCmd(LCDModeFourBit);
	PCF8574_LCDSendCmd(LCDDisplayOn);
	PCF8574_LCDSendCmd(cursorType);
	PCF8574_LCDSendCmd(LCDEntryModeThree);
	PCF8574_LCDSendCmd(LCDClearScreen);
	busy_wait_ms(5);
}

/*!
	@brief  Send a string to LCD
	@param str  Pointer to the char array
*/
void HD44780LCD::PCF8574_LCDSendString(char *str) {
	while (*str) PCF8574_LCDSendData(*str++);
}


/*!
	@brief  Sends a character to screen , simply wraps SendData command.
	@param data Character to display
*/
void HD44780LCD::PCF8574_LCDSendChar(char data) {
	PCF8574_LCDSendData(data);
}

/*!
	@brief  Moves cursor
	@param direction enum LCDDirectionType_e left or right
	@param moveSize number of spaces to move
*/
void HD44780LCD::PCF8574_LCDMoveCursor(LCDDirectionType_e direction, uint8_t moveSize) {
	uint8_t i = 0;
	const uint8_t LCDMoveCursorLeft = 0x10;  //Command Byte Code:  Move cursor one character left 
	const uint8_t LCDMoveCursorRight = 0x14;  // Command Byte Code : Move cursor one character right 
	switch(direction)
	{
	case LCDMoveRight:
		for (i = 0; i < moveSize; i++) {
			PCF8574_LCDSendCmd(LCDMoveCursorRight);
		}
	break;
	case LCDMoveLeft:
		for (i = 0; i < moveSize; i++) {
			PCF8574_LCDSendCmd(LCDMoveCursorLeft);
		}
	break;
	}

}

/*!
	@brief  Scrolls screen
	@param direction  left or right
	@param ScrollSize number of spaces to scroll
*/
void HD44780LCD::PCF8574_LCDScroll(LCDDirectionType_e direction, uint8_t ScrollSize) {
	uint8_t i = 0;

	const uint8_t LCDScrollRight = 0x1E;  // Command Byte Code: Scroll display one character right (all lines) 
	const uint8_t LCDScrollLeft = 0x18;  //Command Byte Code: Scroll display one character left (all lines) 
	
	switch(direction)
	{
	case LCDMoveRight:
		for (i = 0; i < ScrollSize; i++) {
			PCF8574_LCDSendCmd(LCDScrollRight);
		}
	break;
	case LCDMoveLeft:
		for (i = 0; i < ScrollSize; i++) {
			PCF8574_LCDSendCmd(LCDScrollLeft);
		}
	break;
	}
}

/*!
	@brief  moves cursor to an x , y position on display.
	@param  line  x row 1-4
	@param col y column  0-15 or 0-19
*/
void HD44780LCD::PCF8574_LCDGOTO(LCDLineNumber_e line, uint8_t col) {
	switch (line) {
		case LCDLineNumberOne: PCF8574_LCDSendCmd(LCDLineAddressOne| col); break;
		case LCDLineNumberTwo: PCF8574_LCDSendCmd(LCDLineAddressTwo | col); break;
		case LCDLineNumberThree:
			switch (_NumColsLCD)
			{
				case 16: PCF8574_LCDSendCmd(LCDLineAddress3Col16 | col); break;
				case 20: PCF8574_LCDSendCmd(LCDLineAddress3Col20 + col); break;
			}
		break;
		case LCDLineNumberFour:
			switch (_NumColsLCD)
			{
				case 16: PCF8574_LCDSendCmd(LCDLineAddress4Col16 | col); break;
				case 20: PCF8574_LCDSendCmd(LCDLineAddress4Col20 + col); break;
			}
		break;
	}
}

/*!
	@brief  Saves a custom character to a location in character generator RAM 64 bytes.
	@param location CG_RAM location 0-7, we only have 8 locations 64 bytes
	@param charmap An array of 8 bytes representing a custom character data
*/
void HD44780LCD::PCF8574_LCDCreateCustomChar(uint8_t location, uint8_t * charmap)
{

	const uint8_t LCD_CG_RAM = 0x40;  //  character-generator RAM (CG RAM address) 
	 if (location >= 8) {return;}
	 
	PCF8574_LCDSendCmd(LCD_CG_RAM | (location<<3));
	for (uint8_t i=0; i<8; i++) {
		PCF8574_LCDSendData(charmap[i]);
	}
}

/*!
	@brief  Turn LED backlight on and off
	@param OnOff passed bool True = LED on , false = display LED off
	@note another data or command must be issued before it takes effect.
*/
void HD44780LCD::PCF8574_LCDBackLightSet(bool OnOff)
{
	 OnOff ? (_LCDBackLight= LCDBackLightOnMask) : (_LCDBackLight= LCDBackLightOffMask);
}

/*!
	@brief  get the backlight flag status
	@return the status of backlight on or off , true or false.
*/

bool HD44780LCD::PCF8574_LCDBackLightGet(void)
{
	switch(_LCDBackLight){
		case LCDBackLightOnMask : return true; break;
		case LCDBackLightOffMask: return false; break;
		default : return true ; break ;
	}
}

/*!
	@brief Switch on the I2C
	@returns false if PCF8574 fails to appear on I2C bus , true if ok 
	@note if _LCDSerialDebugFlag enabled will print I2C error code to screen
		wire.endTransmission() return:
		-# 1 = Successful connection 
		-# <1 = I2C error, i2c_read_blocking return value.
*/
bool HD44780LCD::PCF8574_LCD_I2C_ON()
{
	int TransmissionCode = 0;
	uint8_t rxdata;

	// init I2c pins and interface
	gpio_set_function(_SDataPin, GPIO_FUNC_I2C);
	gpio_set_function(_SClkPin, GPIO_FUNC_I2C);
	gpio_pull_up(_SDataPin);
	gpio_pull_up(_SClkPin);
	i2c_init(i2c, _CLKSpeed * 1000);

	// check connection?
	TransmissionCode = i2c_read_blocking(i2c, _LCDSlaveAddresI2C , &rxdata, 1, false);
	if (TransmissionCode < 1){ // no bytes read back from device or error issued
		if (_LCDSerialDebugFlag == true){
			printf("1201 LCD_I2C_ON: \r\n");
			printf("Check Connection, TX code :: %d ,RX data :: %u \r\n", TransmissionCode , rxdata);
		}
		return false;
	}
	return true;
}

/*!
	@brief Print out a customer character from character generator CGRAM 64 bytes 8 characters
	@param location CGRAM  0-7
*/
void HD44780LCD::PCF8574_LCDPrintCustomChar(uint8_t location)
{
	if (location >= 8) {return;}
	PCF8574_LCDSendData(location);
}

/*!
	@brief  Called by print class, used to print out numerical data types etc
	@param character write a character 
	@note used internally. Called by the print method using virtual   
*/
size_t HD44780LCD::write(uint8_t character)
{
	PCF8574_LCDSendChar(character) ;
	return 1;
}

/*!
	@brief Clear display using software command , set cursor position to zero
	@note  See also LCDClearScreen for manual clear
 */
void HD44780LCD::PCF8574_LCDClearScreenCmd(void) {
	PCF8574_LCDSendCmd(LCDClearScreen);
	busy_wait_ms(3); // Requires a delay
}

/*!
	@brief Set cursor position to home position .
 */
void HD44780LCD::PCF8574_LCDHome(void) {
	PCF8574_LCDSendCmd(LCDHomePosition);
	busy_wait_ms(3); // Requires a delay
}

/*!
	@brief Change entry mode  
	@param newEntryMode  1-4 , 4 choices.
 */
void HD44780LCD::PCF8574_LCDChangeEntryMode(LCDEntryMode_e newEntryMode)
{
	PCF8574_LCDSendCmd(newEntryMode);
	busy_wait_ms(3); // Requires a delay
}

/*!
	@brief Switch off the I2C to be used at end of program
 */
void HD44780LCD::PCF8574_deinitI2C()
{
	gpio_set_function(_SDataPin, GPIO_FUNC_NULL);
	gpio_set_function(_SClkPin, GPIO_FUNC_NULL);
	i2c_deinit(i2c);
}


void HD44780LCD::PCF8574_LCDSerialDebugSet(bool OnOff)
{
	OnOff ? (_LCDSerialDebugFlag = true) : (_LCDSerialDebugFlag = false);
	if (_LCDSerialDebugFlag == true) {printf("Serial debug ON: \r\n");}
}

bool HD44780LCD::PCF8574_LCDSerialDebugGet(void){return _LCDSerialDebugFlag;}

// **** EOF ****
