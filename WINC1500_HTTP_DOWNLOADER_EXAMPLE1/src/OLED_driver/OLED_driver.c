/**************************************************************************//**
* @file      OLED_driver.c
* @brief     Driver for the OLED from Sparkfun. See https://github.com/sparkfun/SparkFun_Micro_OLED_Arduino_Library/blob/master/src/SFE_MicroOLED.cpp

* @author    Chen Chen
* @date      2021-04-16

******************************************************************************/

#include "OLED_driver.h"
#include "I2cDriver/I2cDriver.h"
#include "SerialConsole.h"
/******************************************************************************
* Includes
******************************************************************************/
#include "OLED_driver/util/font5x7.h"				// Font 0
#include "OLED_driver/util/font8x16.h"				// Font 1
#include "OLED_driver/util/7segment.h"				// Font 2
#include "OLED_driver/util/fontlargenumber.h"		// Font 3
#include "OLED_driver/util/fontlargeletter31x48.h"	// Font 4 (excluded by default - see below)

/******************************************************************************
* Defines
******************************************************************************/
#ifndef _BV
#define _BV(x) (1 << x)
#endif
#define MAXFONTS 5 // Do not change this line - except when _adding_ new fonts

#define i2cTransactionSize 32

#ifndef INCLUDE_FONT_5x7
#define INCLUDE_FONT_5x7 1			// Change this to 0 to exclude the 5x7 font
#endif
#ifndef INCLUDE_FONT_8x16
#define INCLUDE_FONT_8x16 1			// Change this to 0 to exclude the 8x16 font
#endif
#ifndef INCLUDE_FONT_7SEG
#define INCLUDE_FONT_7SEG 1			// Change this to 0 to exclude the seven segment font
#endif
#ifndef INCLUDE_FONT_LARGENUMBER
#define INCLUDE_FONT_LARGENUMBER 1	// Change this to 0 to exclude the large number font
#endif
#ifndef INCLUDE_FONT_LARGELETTER
#define INCLUDE_FONT_LARGELETTER 0	// Change this to 1 to include the large letter font
#endif
/******************************************************************************
* Variables
******************************************************************************/
I2C_Data OLEDData; ///<Global variable to use for I2C communications with the Seesaw Device
/******************************************************************************
* Forward Declarations
******************************************************************************/

/** \brief MicroOLED screen buffer.

Page buffer 64 x 48 divided by 8 = 384 bytes
Page buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.  This page buffer serves as a scratch RAM for graphical functions.  All drawing function will first be drawn on this page buffer, only upon calling display() function will transfer the page buffer to the actual LCD controller's memory.
*/
static uint8_t screenmemory[] = {
	/* LCD Memory organised in 64 horizontal pixel and 6 rows of byte
	 B  B .............B  -----
	 y  y .............y        \
	 t  t .............t         \
	 e  e .............e          \
	 0  1 .............63          \
	                                \
	 D0 D0.............D0            \
	 D1 D1.............D1            / ROW 0
	 D2 D2.............D2           /
	 D3 D3.............D3          /
	 D4 D4.............D4         /
	 D5 D5.............D5        /
	 D6 D6.............D6       /
	 D7 D7.............D7  ----
	*/
	//SparkFun Electronics LOGO

	// ROW0, BYTE0 to BYTE63
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW1, BYTE64 to BYTE127
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW2, BYTE128 to BYTE191
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW3, BYTE192 to BYTE255
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW4, BYTE256 to BYTE319
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW5, BYTE320 to BYTE383
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	
	

// Add the font name as declared in the header file.
// Exclude as many as possible to conserve FLASH memory.
const unsigned int *fontsPointer[] = {
	#if INCLUDE_FONT_5x7
	font5x7,
	#else
	NULL,
	#endif
	#if INCLUDE_FONT_8x16
	font8x16,
	#else
	NULL,
	#endif
	#if INCLUDE_FONT_7SEG
	sevensegment,
	#else
	NULL,
	#endif
	#if INCLUDE_FONT_LARGENUMBER
	fontlargenumber,
	#else
	NULL,
	#endif
	#if INCLUDE_FONT_LARGELETTER
	fontlargeletter31x48
	#else
	NULL
	#endif
};
/******************************************************************************
* Callback Functions
******************************************************************************/


/******************************************************************************
* Functions
******************************************************************************/

/**************************************************************************//**
* @fn		int InitializeSeesaw(void)
* @brief	Initializes the Seesaw 4x4
* @details 	Assumes I2C is already initialized
                				
* @return		Returns 0 if no errors.
* @note         
*****************************************************************************/
int InitializeOLEDdriver(void)
{
	// default 5x7 font
	MicroOLEDsetFontType(0);
	MicroOLEDsetColor(WHITE);
	MicroOLEDsetDrawMode(NORM);
	MicroOLEDsetCursor(0, 0);

	// Display Init sequence for 64x48 OLED module
	int error = MicroOLEDcommand(DISPLAYOFF); // 0xAE
	if(ERROR_NONE != error)
	{
		SerialConsoleWriteString("Error initializing OLED!\r\n");
	}
	else
	{
		SerialConsoleWriteString("OLED Turned off!\r\n");
	}

	error = MicroOLEDcommand(SETDISPLAYCLOCKDIV); // 0xD5
	if(ERROR_NONE != error)
	{
		SerialConsoleWriteString("Error initializing OLED clock!\r\n");
	}
	else
	{
		SerialConsoleWriteString("OLED clock set!\r\n");
	}
	MicroOLEDcommand(0x80);				 // the suggested ratio 0x80

	MicroOLEDcommand(SETMULTIPLEX); // 0xA8
	MicroOLEDcommand(0x2F);

	MicroOLEDcommand(SETDISPLAYOFFSET); // 0xD3
	MicroOLEDcommand(0x0);			   // no offset

	MicroOLEDcommand(SETSTARTLINE | 0x0); // line #0

	MicroOLEDcommand(CHARGEPUMP); // enable charge pump
	MicroOLEDcommand(0x14);

	MicroOLEDcommand(NORMALDISPLAY);		 // 0xA6
	MicroOLEDcommand(DISPLAYALLONRESUME); // 0xA4

	MicroOLEDcommand(SEGREMAP | 0x1);
	MicroOLEDcommand(COMSCANDEC);

	MicroOLEDcommand(SETCOMPINS); // 0xDA
	MicroOLEDcommand(0x12);

	MicroOLEDcommand(SETCONTRAST); // 0x81
	MicroOLEDcommand(0x8F);

	MicroOLEDcommand(SETPRECHARGE); // 0xd9
	MicroOLEDcommand(0xF1);

	MicroOLEDcommand(SETVCOMDESELECT); // 0xDB
	MicroOLEDcommand(0x40);

	MicroOLEDcommand(DISPLAYON); //--turn on oled panel
	error = MicroOLEDclear(ALL);			// Erase hardware memory inside the OLED controller to avoid random data in memory.
	return error;
}

/** \brief Send the display a command byte
    Send a command via I2C 	to SSD1306 controller.
	For I2C  we use the write functions
	defined in hardware.cpp to send the data.
*/

int MicroOLEDcommand(uint8_t c)
{
	// Write to our address, make sure it knows we're sending a
	// command:
	uint8_t msg[2]={I2C_COMMAND,c};
	OLEDData.address = OLED_I2C_ADDRESS_SA0_1;
	OLEDData.msgOut = &msg;
	OLEDData.lenOut = sizeof(msg);
	OLEDData.lenIn = 0;
	int error = I2cWriteDataWait(&OLEDData, 100);
	return error;

}


/** \brief Send the display a data byte
    Send a data byte via SPI, I2C or parallel to SSD1306 controller.
	For SPI we set the DC and CS pins here, and call spiTransfer(byte)
	to send the data. For I2C and Parallel we use the write functions
	defined in hardware.cpp to send the data.
*/
int MicroOLEDdata(uint8_t c)
{
	uint8_t msg[2]={I2C_DATA,c};
	OLEDData.address = OLED_I2C_ADDRESS_SA0_1;
	OLEDData.msgOut = &msg;
	OLEDData.lenOut = sizeof(msg);
	OLEDData.lenIn = 0;
	int error = I2cWriteDataWait(&OLEDData, 100);
	return error;
	
}


/** \brief Clear screen buffer or SSD1306's memory.
    To clear GDRAM inside the LCD controller, pass in the variable mode = ALL and to clear screen page buffer pass in the variable mode = PAGE.
*/
int MicroOLEDclear(uint8_t mode)
{
	//	uint8_t page=6, col=0x40;
int error = NULL;
	if (mode == ALL)
	{
		for (int i = 0; i < 8; i++)
		{
			MicroOLEDsetPageAddress(i);
			MicroOLEDsetColumnAddress(0);
			uint8_t zeros[0x80];
			memset(zeros, 0, 0x80);
			for (int j = 0; j < 0x80; j++)
			{
				error= MicroOLEDdata(0);
				if (ERROR_NONE != error){
					return error;
				}
				
			}
			
		}
	}
	else
	{
		memset(screenmemory, 0, 384); // (64 x 48) / 8 = 384
		MicroOLEDdisplay();
	}
	return error;
}


/** \brief Transfer display memory.
    Bulk move the screen buffer to the SSD1306 controller's memory so that images/graphics drawn on the screen buffer will be displayed on the OLED.
*/
int MicroOLEDdisplay(void)
{
	uint8_t i, j;
int error = NULL;
	for (i = 0; i < 6; i++)
	{
		MicroOLEDsetPageAddress(i);
		MicroOLEDsetColumnAddress(0);
		for (j = 0; j < 0x40; j++)
		{
			error= MicroOLEDdata(screenmemory[i * 0x40 + j]);
			if (ERROR_NONE != error){
				return error;
			}
		}
	}
	return error;
}

/** \brief Set SSD1306 page address.
    Send page address command and address to the SSD1306 OLED controller.
*/
void MicroOLEDsetPageAddress(uint8_t add)
{
	add = 0xb0 | add;
	MicroOLEDcommand(add);
	return;
}

/** \brief Set SSD1306 column address.
    Send column address command and address to the SSD1306 OLED controller.
*/
void MicroOLEDsetColumnAddress(uint8_t add)
{
	MicroOLEDcommand((0x10 | (add >> 4)) + 0x02);
	MicroOLEDcommand((0x0f & add));
	return;
}

/** \brief Get LCD width.
    The width of the LCD return as byte.
*/
uint8_t MicroOLEDgetLCDWidth(void)
{
	return LCDWIDTH;
}


/** \brief Get LCD height.
    The height of the LCD return as byte.
*/
uint8_t MicroOLEDgetLCDHeight(void)
{
	return LCDHEIGHT;
}


/*
Draw circle with radius using color and mode at x,y of the screen buffer.
*/
void MicroOLEDcircle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode)
{

	//TODO - find a way to check for no overlapping of pixels so that XOR draw mode will work perfectly
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	MicroOLEDpixel(x0, y0 + radius, color, mode);
	MicroOLEDpixel(x0, y0 - radius, color, mode);
	MicroOLEDpixel(x0 + radius, y0, color, mode);
	MicroOLEDpixel(x0 - radius, y0, color, mode);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		MicroOLEDpixel(x0 + x, y0 + y, color, mode);
		MicroOLEDpixel(x0 - x, y0 + y, color, mode);
		MicroOLEDpixel(x0 + x, y0 - y, color, mode);
		MicroOLEDpixel(x0 - x, y0 - y, color, mode);
		MicroOLEDpixel(x0 + y, y0 + x, color, mode);
		MicroOLEDpixel(x0 - y, y0 + x, color, mode);
		MicroOLEDpixel(x0 + y, y0 - x, color, mode);
		MicroOLEDpixel(x0 - y, y0 - x, color, mode);
	}
}


/** \brief Draw pixel with color and mode.
Draw color pixel in the screen buffer's x,y position with NORM or XOR draw mode.
*/
void MicroOLEDpixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode)
{
	if ((x < 0) || (x >= LCDWIDTH) || (y < 0) || (y >= LCDHEIGHT))
	return;

	if (mode == XOR)
	{
		if (color == WHITE)
		screenmemory[x + (y / 8) * LCDWIDTH] ^= _BV((y % 8));
	}
	else
	{
		if (color == WHITE)
		screenmemory[x + (y / 8) * LCDWIDTH] |= _BV((y % 8));
		else
		screenmemory[x + (y / 8) * LCDWIDTH] &= ~_BV((y % 8));
	}
}

/** \brief Set draw mode.
    Set current draw mode with NORM or XOR.
*/
void MicroOLEDsetDrawMode(uint8_t mode)
{
	drawMode = mode;
}

/** \brief Set cursor position.
MicroOLED's cursor position to x,y.
*/
void MicroOLEDsetCursor(uint8_t x, uint8_t y)
{
	cursorX = x;
	cursorY = y;
}

/** \brief Set font type.
    Set the current font type number, ie changing to different fonts base on the type provided.
*/
uint8_t MicroOLEDsetFontType(uint8_t type)
{
    if ((type >= MAXFONTS) || (fontsPointer[type] == NULL))
        return false;

	fontType = type;
	fontWidth = *(fontsPointer[fontType] + 0);
	fontHeight = *(fontsPointer[fontType] + 1);
	fontStartChar = *(fontsPointer[fontType] + 2);
	fontTotalChar = *(fontsPointer[fontType] + 3);
	fontMapWidth = (*(fontsPointer[fontType] + 4) * 100) + *(fontsPointer[fontType] + 5); // two bytes values into integer 16
	return true;
}

/** \brief Set color.
    Set the current draw's color. Only WHITE and BLACK available.
*/
void MicroOLEDsetColor(uint8_t color)
{
	foreColor = color;
}

