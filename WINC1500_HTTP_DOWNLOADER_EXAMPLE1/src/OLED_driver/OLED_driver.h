/**************************************************************************//**
* @file      OLED_driver.h
* @brief     Driver for the OLED from Sparkfun. See https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/blob/master/src/SFE_MicroOLED.cpp

* @author    Chen Chen
* @date      2021-04-16

******************************************************************************/


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include <stdio.h>
#include "I2cDriver/I2cDriver.h"
#include "SerialConsole.h"

#define OLED_I2C_ADDRESS_SA0_0 0b0111100
#define OLED_I2C_ADDRESS_SA0_1 0b0111101
#define I2C_ADDRESS_UNDEFINED 0b00000000
#define I2C_COMMAND 0x00
#define I2C_DATA 0x40

#define BLACK 0
#define WHITE 1

#define LCDWIDTH 64
#define LCDHEIGHT 48
#define FONTHEADERSIZE 6

#define NORM 0
#define XOR 1

#define PAGE 0
#define ALL 1

#define WIDGETSTYLE0 0
#define WIDGETSTYLE1 1
#define WIDGETSTYLE2 2

#define SETCONTRAST 0x81
#define DISPLAYALLONRESUME 0xA4
#define DISPLAYALLON 0xA5
#define NORMALDISPLAY 0xA6
#define INVERTDISPLAY 0xA7
#define DISPLAYOFF 0xAE
#define DISPLAYON 0xAF
#define SETDISPLAYOFFSET 0xD3
#define SETCOMPINS 0xDA
#define SETVCOMDESELECT 0xDB
#define SETDISPLAYCLOCKDIV 0xD5
#define SETPRECHARGE 0xD9
#define SETMULTIPLEX 0xA8
#define SETLOWCOLUMN 0x00
#define SETHIGHCOLUMN 0x10
#define SETSTARTLINE 0x40
#define MEMORYMODE 0x20
#define COMSCANINC 0xC0
#define COMSCANDEC 0xC8
#define SEGREMAP 0xA0
#define CHARGEPUMP 0x8D
#define EXTERNALVCC 0x01
#define SWITCHCAPVCC 0x02



typedef enum CMD
{
	CMD_CLEAR,		  //0
	CMD_INVERT,		  //1
	CMD_CONTRAST,	  //2
	CMD_DISPLAY,	  //3
	CMD_SETCURSOR,	  //4
	CMD_PIXEL,		  //5
	CMD_LINE,		  //6
	CMD_LINEH,		  //7
	CMD_LINEV,		  //8
	CMD_RECT,		  //9
	CMD_RECTFILL,	  //10
	CMD_CIRCLE,		  //11
	CMD_CIRCLEFILL,	  //12
	CMD_DRAWCHAR,	  //13
	CMD_DRAWBITMAP,	  //14
	CMD_GETLCDWIDTH,  //15
	CMD_GETLCDHEIGHT, //16
	CMD_SETCOLOR,	  //17
	CMD_SETDRAWMODE	  //18
} commCommand_t;



	
	////Set the max number of bytes set in a given I2C transaction
	//uint8_t i2cTransactionSize = 32; //Default to ATmega328 limit


	uint8_t mosipinmask, sckpinmask, sspinmask, dcpinmask;
	uint8_t foreColor, drawMode, fontWidth, fontHeight, fontType, fontStartChar, fontTotalChar, cursorX, cursorY;
	uint16_t fontMapWidth;
	const unsigned int *fontsPointer[];

	int InitializeOLEDdriver(void);

	// RAW LCD functions
	int MicroOLEDcommand(uint8_t c);
	int MicroOLEDdata(uint8_t c);
	void MicroOLEDsetColumnAddress(uint8_t add);
	void MicroOLEDsetPageAddress(uint8_t add);

	// LCD Draw functions
	int MicroOLEDclear(uint8_t mode);
	void MicroOLEDinvert(bool inv);
	void MicroOLEDcontrast(uint8_t contrast);
	int MicroOLEDdisplay(void);
	void MicroOLEDsetCursor(uint8_t x, uint8_t y);
	void MicroOLEDpixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode);

	void MicroOLEDline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode);


	void MicroOLEDcircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color, uint8_t mode);

	void MicroOLEDcircleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode);

	void MicroOLEDdrawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t mode);
	void MicroOLEDdrawBitmap(uint8_t *bitArray);

	uint8_t MicroOLEDgetLCDWidth(void);
	uint8_t MicroOLEDgetLCDWidth(void);
	void MicroOLEDsetColor(uint8_t color);
	void MicroOLEDsetDrawMode(uint8_t mode);
	uint8_t *getScreenBuffer(void);

	// Font functions
	uint8_t MicroOLEDgetFontWidth(void);
	uint8_t MicroOLEDgetFontHeight(void);
	uint8_t MicroOLEDgetTotalFonts(void);
	uint8_t MicroOLEDgetFontType(void);
	uint8_t MicroOLEDsetFontType(uint8_t type);
	uint8_t MicroOLEDgetFontStartChar(void);
	uint8_t MicroOLEDgetFontTotalChar(void);


	bool MicroOLEDi2cWriteMultiple(uint8_t *dataBytes);

#endif /* OLED_DRIVER_H_ */