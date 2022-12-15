/*
 * File: HD44780_LCD_PCF8574.cpp
 * Description:
 * HD44780-based character LCD I2C(PCF8574) library cpp file for RPI Rp2040 PICO
 * Description: See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/HD44780_LCD_PCF8574_PICO
 */

// Section : Includes
#include <stdio.h> // optional for printf debug messages
#include "pico/stdlib.h"
#include "../include/hd44780/HD44780_LCD_PCF8574.hpp"

// Section : class constructor
// Param1 : I2C address
// Param2 : I2C instance of port IC20 or I2C1
// Param3 : I2C Bus Clock speed in KHz. Typically 100-400
// Param4 : I2C Data pin
// Param5 : I2C Clock pin
HD44780LCD  :: HD44780LCD(uint8_t I2Caddress, i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t  SDApin, uint8_t  SCLKpin)
{
    _LCDSlaveAddresI2C  = I2Caddress;
    i2c = i2c_type; 
    _SClkPin = SCLKpin;
    _SDataPin = SDApin;
    _CLKSpeed = CLKspeed;
}


// Section : Functions

//  Func Desc: Send data byte to  LCD via I2C
//  Param1: data byte
void HD44780LCD::PCF8574_LCDSendData(unsigned char data) {
    unsigned char dataLower, dataUpper;
    uint8_t dataI2C[4];
    int TransmissionCode = 0;
    
    dataLower = (data << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
    dataUpper = data & 0xf0; //select upper nibble
    dataI2C[0] = dataUpper | (LCD_DATA_BYTE_ON & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
    dataI2C[1] = dataUpper | (LCD_DATA_BYTE_OFF & _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs
    dataI2C[2] = dataLower | (LCD_DATA_BYTE_ON & _LCDBackLight); //enable=1 and rs =1 1101  YYYY-X-en-X-rs
    dataI2C[3] = dataLower | (LCD_DATA_BYTE_OFF &  _LCDBackLight); //enable=0 and rs =1 1001 YYYY-X-en-X-rs

    TransmissionCode = i2c_write_timeout_us(i2c, _LCDSlaveAddresI2C, dataI2C, 4 , false, LCD_I2C_DELAY);
    if (TransmissionCode < 1)
    {
        #ifdef LCD_SERIAL_DEBUG
            printf("1203 data: \r\n");
            printf("I2C error i2c_write_blocking: \r\n");
            printf("Tranmission code : %d \r\n", TransmissionCode );
            busy_wait_ms(100);
        #endif
    }
}

//  Func Desc: Send command byte to  lcd
//  Param1: command byte

void HD44780LCD::PCF8574_LCDSendCmd(unsigned char cmd) {
    unsigned char cmdLower, cmdUpper;
    uint8_t cmdI2C[4];
    int TransmissionCode = 0;
    
    cmdLower = (cmd << 4)&0xf0; //select lower nibble by moving it to the upper nibble position
    cmdUpper = cmd & 0xf0; //select upper nibble
    cmdI2C[0] = cmdUpper | (LCD_CMD_BYTE_ON & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
    cmdI2C[1] = cmdUpper | (LCD_CMD_BYTE_OFF & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
    cmdI2C[2] = cmdLower | (LCD_CMD_BYTE_ON & _LCDBackLight); // YYYY-1100 YYYY-led-en-rw-rs ,enable=1 and rs =0
    cmdI2C[3] = cmdLower | (LCD_CMD_BYTE_OFF & _LCDBackLight); // YYYY-1000 YYYY-led-en-rw-rs ,enable=0 and rs =0
    
    TransmissionCode = i2c_write_timeout_us(i2c, _LCDSlaveAddresI2C, cmdI2C, 4 , false, LCD_I2C_DELAY);
    if (TransmissionCode < 1)
    {
        #ifdef LCD_SERIAL_DEBUG
            printf("1202 cmd : \r\n");
            printf("I2C error  i2c_write_blocking : \r\n");
            printf("Tranmission code : %d \r\n", TransmissionCode );
            busy_wait_ms(100);
        #endif
    }

}

// Func Desc: Clear a line by writing spaces to every position
// Param1: enum LCDLineNumber_e lineNo  1-4

void HD44780LCD::PCF8574_LCDClearLine(LCDLineNumber_e lineNo) {

    switch (lineNo) {
        case LCDLineNumberOne:
            PCF8574_LCDSendCmd(LCD_LINE_ADR1);
            break;
        case LCDLineNumberTwo:
            PCF8574_LCDSendCmd(LCD_LINE_ADR2);
            break;
        case LCDLineNumberThree:
            if (_NumColsLCD == 20)
                PCF8574_LCDSendCmd(LCD_LINE_ADR3_20);
            else
                PCF8574_LCDSendCmd(LCD_LINE_ADR3_16);
            break;
        case LCDLineNumberFour:
            if (_NumColsLCD == 20)
                PCF8574_LCDSendCmd(LCD_LINE_ADR4_20);
            else
                PCF8574_LCDSendCmd(LCD_LINE_ADR4_16);
            break;
    }

    for (uint8_t i = 0; i < _NumColsLCD; i++) {
        PCF8574_LCDSendData(' ');
    }
}

// Func Desc: Clear screen by writing spaces to every position
// Note : See also LCDClearScreenCmd for software command  clear alternative.

void HD44780LCD::PCF8574_LCDClearScreen(void) {
    if (_NumRowsLCD < 1 || _NumRowsLCD >4)
    {
        return;
    }

    PCF8574_LCDClearLine(LCDLineNumberOne);
    
    if (_NumRowsLCD >= 2)
        PCF8574_LCDClearLine(LCDLineNumberTwo);
    if (_NumRowsLCD >= 3)
        PCF8574_LCDClearLine(LCDLineNumberThree);
    if (_NumRowsLCD == 4)
        PCF8574_LCDClearLine(LCDLineNumberFour);
}


// Func Desc: Reset screen
// Param1: enum LCDCursorType_e cursor type, 4 choices

void HD44780LCD::PCF8574_LCDResetScreen(LCDCursorType_e CursorType) {
    PCF8574_LCDSendCmd(LCD_MODE_4BIT);
    PCF8574_LCDSendCmd(LCD_DISPLAY_ON);
    PCF8574_LCDSendCmd(CursorType);
    PCF8574_LCDSendCmd(LCD_CLRSCR);
    PCF8574_LCDSendCmd(LCDEntryModeThree);
    busy_wait_ms(5);
}


// Func Desc: Turn Screen on and off
// Param1: passed bool, True = display on , false = display off

void HD44780LCD::PCF8574_LCDDisplayON(bool OnOff) {
    OnOff ? PCF8574_LCDSendCmd(LCD_DISPLAY_ON) : PCF8574_LCDSendCmd(LCD_DISPLAY_OFF);
    busy_wait_ms(5);
}


// Func Desc: Initialise LCD
// Param1: enum LCDCursorType_e cursor type, 4 choices.

void HD44780LCD::PCF8574_LCDInit(LCDCursorType_e CursorType, uint8_t NumRow, uint8_t NumCol) {

    _NumRowsLCD = NumRow;
    _NumColsLCD = NumCol;

    while (PCF8574_LCD_I2C_ON() != true)
    {
        busy_wait_ms(3000);
    }
    
    busy_wait_ms(15);
    PCF8574_LCDSendCmd(LCD_HOME);
    busy_wait_ms(5);
    PCF8574_LCDSendCmd(LCD_HOME);
    busy_wait_ms(5);
    PCF8574_LCDSendCmd(LCD_HOME);
    busy_wait_ms(5);
    PCF8574_LCDSendCmd(LCD_MODE_4BIT);
    PCF8574_LCDSendCmd(LCD_DISPLAY_ON);
    PCF8574_LCDSendCmd(CursorType);
    PCF8574_LCDSendCmd(LCDEntryModeThree);
    PCF8574_LCDSendCmd(LCD_CLRSCR);
    busy_wait_ms(5);
}

// Func Desc: Send string to LCD
// Param1: Pointer to the char array

void HD44780LCD::PCF8574_LCDSendString(char *str) {
    while (*str) PCF8574_LCDSendData(*str++);
}


// Func Desc: Sends a character to screen , simply wraps SendData command.
// Param1: Character to display
void HD44780LCD::PCF8574_LCDSendChar(char data) {
    PCF8574_LCDSendData(data);
}

// Func Desc: Moves cursor
// Param1. enum LCDDirectionType_e left or right
// Param2. uint8_t number of spaces to move

void HD44780LCD::PCF8574_LCDMoveCursor(LCDDirectionType_e direction, uint8_t moveSize) {
    uint8_t i = 0;
    if (direction == LCDMoveRight) {
        for (i = 0; i < moveSize; i++) {
            PCF8574_LCDSendCmd(LCD_MOV_CURSOR_RIGHT);
        }
    } else {
        for (i = 0; i < moveSize; i++) {
            PCF8574_LCDSendCmd(LCD_MOV_CURSOR_LEFT);
        }
    }

}

// Func Desc: Scrolls screen
// Param1. enum LCDDirectionType_e , left or right
// Param2. uint8_t number of spaces to scroll

void HD44780LCD::PCF8574_LCDScroll(LCDDirectionType_e direction, uint8_t ScrollSize) {
    uint8_t i = 0;
    if (direction == LCDMoveRight) {
        for (i = 0; i < ScrollSize; i++) {
            PCF8574_LCDSendCmd(LCD_SCROLL_RIGHT);
        }
    } else {
        for (i = 0; i < ScrollSize; i++) {
            PCF8574_LCDSendCmd(LCD_SCROLL_LEFT);
        }
    }

}
// Func Desc: moves cursor to an x , y position on display.
// Param1: enum LCDLineNumber_e  row 1-Y where Y is number of lines
// Param2: uint8_t col 0-X where X is width of Screen
void HD44780LCD::PCF8574_LCDGOTO(LCDLineNumber_e line, uint8_t col) {
    switch (line) {
        case LCDLineNumberOne:
            PCF8574_LCDSendCmd(LCD_LINE_ADR1 | col);
            break;
        case LCDLineNumberTwo:
            PCF8574_LCDSendCmd(LCD_LINE_ADR2 | col);
            break;
        case LCDLineNumberThree:
            if (_NumColsLCD == 20)
                PCF8574_LCDSendCmd(LCD_LINE_ADR3_20 + col);
            else
                PCF8574_LCDSendCmd(LCD_LINE_ADR3_16 | col);
            break;
        case LCDLineNumberFour:
            if (_NumColsLCD == 20)
                PCF8574_LCDSendCmd(LCD_LINE_ADR4_20 + col);
            else
                PCF8574_LCDSendCmd(LCD_LINE_ADR4_16 | col);
            ;
            break;
    }
}

// Func Desc: Saves a custom character to a location in CG_RAM
// Param1: CG_RAM location 0-7 we only have 8 locations 0-7
// Param2: An array of 8 bytes representing a custom character data
void HD44780LCD::PCF8574_LCDCreateCustomChar(uint8_t location, uint8_t * charmap)
{
    if (location >= 8) {return;}
    PCF8574_LCDSendCmd(LCD_CG_RAM | (location<<3));
    for (uint8_t i=0; i<8; i++) {
        PCF8574_LCDSendData(charmap[i]);
    }
}

// Func Desc: Turn LED backlight on and off
// Param1: passed bool True = LED on , false = display LED off
// Note: another data or command must be issued before it takes effect.
void HD44780LCD::PCF8574_LCDBackLightSet(bool OnOff)
{
    OnOff ? (_LCDBackLight= LCD_BACKLIGHTON_MASK) : (_LCDBackLight= LCD_BACKLIGHTOFF_MASK);
}


// Func Desc: Setup I2C settings
// Returns false if PCF8574 fails to appear on I2C bus
// Returns true if ok 
// Note if LCD_SERIAL_DEBUG enabled will print I2C error code to console
// 1 = Successful connection 
// <1 = I2C error, i2c_read_blocking return value.
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
        #ifdef LCD_SERIAL_DEBUG
            printf("1201 LCD_I2C_ON: \r\n");
            printf("Check Connection, TX code :: %d ,RX data :: %u \r\n", TransmissionCode , rxdata);
        #endif
        return false;
    }
    return true;
}


// Print out a customer character from CGRAM
// Param1 CGRAM location 0-7
void HD44780LCD::PCF8574_LCDPrintCustomChar(uint8_t location)
{
    if (location >= 8) {return;}
    PCF8574_LCDSendData(location);
}

// Called by print class, used to print out numerical data types etc
size_t HD44780LCD::write(uint8_t c)
{
    PCF8574_LCDSendChar(c) ;
    return 1;
}

// Clear display using software command , set cursor position to zero
// See also LCDClearScreen for manual clear

void HD44780LCD::PCF8574_LCDClearScreenCmd(void) {
    PCF8574_LCDSendCmd(LCD_CLRSCR);
    busy_wait_ms(3); // Requires a delay
}

// Set Cursor position to zero

void HD44780LCD::PCF8574_LCDHome(void) {
    PCF8574_LCDSendCmd(LCD_HOME);
    busy_wait_ms(3); // Requires a delay
}

// Change entry mode 
// Param1 enum LCDEntryMode_e 1-4 , 4 choices.

void HD44780LCD::PCF8574_LCDChangeEntryMode(LCDEntryMode_e newEntryMode)
{
    PCF8574_LCDSendCmd(newEntryMode);
    busy_wait_ms(3); // Requires a delay
}

// Switch off the I2C to be used at end of program
void HD44780LCD::PCF8574_deinitI2C()
{
    gpio_set_function(_SDataPin, GPIO_FUNC_NULL);
    gpio_set_function(_SClkPin, GPIO_FUNC_NULL);
    i2c_deinit(i2c);
}
// **** EOF ****
