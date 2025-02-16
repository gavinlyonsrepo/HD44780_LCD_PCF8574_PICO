/*!
	@file     HD44780_LCD_PCF8574.hpp
	@author   Gavin Lyons
	@brief    HD44780-based character LCD 16x02 I2C(PCF8574) library header file for RPI Rp2040 PICO
*/

#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include "HD44780_LCD_PCF8574_Print.hpp"
#include "hardware/i2c.h"

/*!
	@brief Class for HD44780 LCD  
*/
class HD44780LCD : public Print{
	public:
		// public enums
		
		/*! Backlight Control ,  Command Byte Code */
		enum LCDBackLight_e : uint8_t{
			LCDBackLightOnMask = 0x0F, /**< XXXX-1111 , Turn on Back light */
			LCDBackLightOffMask = 0x07 /**< XXXX-0111, Turn off Back light */
		}; 
		
		/*! Entry mode control  set command,  Command Byte Code */
		enum LCDEntryMode_e : uint8_t{
			LCDEntryModeOne = 0x04, /**< Display Shift :OFF Decrement Address Counter */
			LCDEntryModeTwo = 0x05, /**< Display Shift :ON  Decrement Address Counter */
			LCDEntryModeThree = 0x06, /**< Display Shift :OFF Increment Address Counter, default */
			LCDEntryModeFour = 0x07 /**<  Display Shift :ON Increment Address Counter */
		}; 

		/*! Cursor mode, Command Byte Code */
		enum LCDCursorType_e : uint8_t {
			LCDCursorTypeOff= 0x0C, /**< Make cursor invisible */
			LCDCursorTypeBlink = 0x0D, /**< Turn on blinking-block cursor */
			LCDCursorTypeOn = 0x0E,  /**< Turn on visible  underline cursor */
			LCDCursorTypeOnBlink  = 0x0F /**<Turn on blinking-block cursor + visible underline cursor */
		}; 
		
		/*! Direction mode for scroll and move */
		enum LCDDirectionType_e : uint8_t {
			LCDMoveRight= 1, /**< move or scroll right */
			LCDMoveLeft = 2   /**< move or scroll left */
		}; 

		/*! Line Row Number */
		enum LCDLineNumber_e : uint8_t{
			LCDLineNumberOne = 1,   /**< row 1 */
			LCDLineNumberTwo = 2,   /**< row 2 */
			LCDLineNumberThree = 3, /**< row 3 */
			LCDLineNumberFour = 4  /**<  row 4 */
		}; 


		HD44780LCD(uint8_t I2Caddress, i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t  SDApin, uint8_t  SCLKpin);
		~HD44780LCD(){};

		bool LCDInit (LCDCursorType_e, uint8_t NumRow, uint8_t NumCol);
		void LCDDeInit(void);
		void LCDDisplayON(bool );
		void LCDResetScreen(LCDCursorType_e);

		void LCDBackLightSet(bool);
		bool LCDBackLightGet(void);

		void LCDSerialDebugSet(bool);
		bool LCDSerialDebugGet(void);

		void LCDSendString (char *str);
		void LCDSendChar (char data);
		void LCDCreateCustomChar(uint8_t location, uint8_t* charmap);
		void LCDPrintCustomChar(uint8_t location);

		void LCDMoveCursor(LCDDirectionType_e, uint8_t moveSize);
		void LCDScroll(LCDDirectionType_e, uint8_t ScrollSize);
		void LCDGOTO(LCDLineNumber_e  lineNo, uint8_t  col);
		void LCDClearLine (LCDLineNumber_e lineNo);
		void LCDClearScreen(void);
		void LCDClearScreenCmd(void);
		void LCDHome(void);
		void LCDChangeEntryMode(LCDEntryMode_e mode);
		virtual size_t write(uint8_t);

	private:

	// Private Enums
	/*!  DDRAM address's used to set cursor position  Note Private */
	enum LCDAddress_e : uint8_t {
		LCDLineAddressOne =  0x80,  /**< Line 1 */
		LCDLineAddressTwo =  0xC0, /**< Line 2 */
		LCDLineAddress3Col20 = 0x94, /**< Line 3 20x04 line 3 */
		LCDLineAddress4Col20 = 0xD4, /**< Line 4 20x04 line 4 */
		LCDLineAddress3Col16  = 0x90, /**< Line 3 16x04  untested, no part */
		LCDLineAddress4Col16  = 0xD0 /**< Line 4 16x04 untested, no part */
	}; 

	/*!  Command Bytes General  Note Private */
	enum LCDCmdBytesGeneral_e : uint8_t {
		LCDModeFourBit = 0x28, /**< Function set (4-bit interface, 2 lines, 5*7 Pixels) */
		LCDHomePosition  = 0x02, /**< Home (move cursor to top/left character position) */
		LCDDisplayOn = 0x0C,  /**< Restore the display (with cursor hidden) */
		LCDDisplayOff = 0x08, /**< Blank the display (without clearing) */
		LCDClearTheScreen = 0x01 /**< clear screen */
	};

		// I2C
		const uint32_t _LCDI2Cdelay = 50000; // uS delay , I2C timeout
		uint8_t _LCDSlaveAddresI2C;
		uint8_t _NumRowsLCD = 2;
		uint8_t _NumColsLCD = 16;
		uint8_t _SDataPin;
		uint8_t _SClkPin;
		uint16_t _CLKSpeed = 100; //I2C bus speed in khz datasheet says 100 for PCF8574 
		i2c_inst_t *i2c; // i2C port number

		// ** DEBUG **  for serial debug I2C errors to console
		bool _LCDSerialDebugFlag = false;

		// Private internal enums

		enum  LCDBackLight_e _LCDBackLight= LCDBackLightOnMask;  /**< Enum to store backlight status*/
		
		void LCDSendCmd (unsigned char cmd);
		void LCDSendData (unsigned char data);
		bool LCD_I2C_ON(void);

	}; // end of HD44780LCD class

#endif // guard header ending
