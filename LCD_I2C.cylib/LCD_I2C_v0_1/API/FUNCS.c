/****************************************************************************
* This file is part of the PCF8574AT Custom Component for PSoC Devices.
*
* Copyright (C) 2017 Carlos Diaz <carlos.santiago.diaz@gmail.com>
*
* Based on the port of: Šarūnas Straigis
* http://www.element14.com/community/people/sarunaszx/blog
*
* Permission to use, copy, modify, and/or distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
****************************************************************************/
    
#include <cytypes.h>
#include <cyfitter.h>

#include "project.h"

#include <stdint.h>
#include <string.h>

#include "`$INSTANCE_NAME`_REGS.h"
#include "`$INSTANCE_NAME`_LL_I2C.h"
#include "`$INSTANCE_NAME`_FUNCS.h"

uint8_t `$INSTANCE_NAME`_addr;
uint8_t `$INSTANCE_NAME`_displayfunction;
uint8_t `$INSTANCE_NAME`_displaycontrol;
uint8_t `$INSTANCE_NAME`_displaymode;
uint8_t `$INSTANCE_NAME`_cols;
uint8_t `$INSTANCE_NAME`_rows;
uint8_t `$INSTANCE_NAME`_charsize;
uint8_t `$INSTANCE_NAME`_backlightval;

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).
void `$INSTANCE_NAME`_start(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
    `$INSTANCE_NAME`_init(lcd_addr, lcd_cols, lcd_rows, charsize);
    `$INSTANCE_NAME`_begin();
}

void `$INSTANCE_NAME`_init(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	`$INSTANCE_NAME`_addr = lcd_addr;
	`$INSTANCE_NAME`_cols = lcd_cols;
	`$INSTANCE_NAME`_rows = lcd_rows;
	`$INSTANCE_NAME`_charsize = charsize;
	`$INSTANCE_NAME`_backlightval = LCD_BACKLIGHT;
}

void `$INSTANCE_NAME`_begin(void)
{
	`$INSTANCE_NAME`_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (`$INSTANCE_NAME`_rows > 1) {
		`$INSTANCE_NAME`_displayfunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((`$INSTANCE_NAME`_charsize != 0) && (`$INSTANCE_NAME`_rows == 1)) {
		`$INSTANCE_NAME`_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	CyDelay(50); 

	// Now we pull both RS and R/W low to begin commands
	`$INSTANCE_NAME`_expanderWrite(`$INSTANCE_NAME`_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	CyDelay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	`$INSTANCE_NAME`_write4bits(0x03 << 4);
	CyDelayUs(4500); // wait min 4.1ms

	// second try
	`$INSTANCE_NAME`_write4bits(0x03 << 4);
	CyDelayUs(4500); // wait min 4.1ms

	// third go!
	`$INSTANCE_NAME`_write4bits(0x03 << 4); 
	CyDelayUs(150);

	// finally, set to 4-bit interface
	`$INSTANCE_NAME`_write4bits(0x02 << 4); 

	// set # lines, font size, etc.
	`$INSTANCE_NAME`_command(LCD_FUNCTIONSET | `$INSTANCE_NAME`_displayfunction);  
	
	// turn the display on with no cursor or blinking default
	`$INSTANCE_NAME`_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	`$INSTANCE_NAME`_display();
	
	// clear it off
	`$INSTANCE_NAME`_clear();
	
	// Initialize to default text direction (for roman languages)
	`$INSTANCE_NAME`_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	`$INSTANCE_NAME`_command(LCD_ENTRYMODESET | `$INSTANCE_NAME`_displaymode);
	
	`$INSTANCE_NAME`_home();
}

/********** high level commands, for the user! */
void `$INSTANCE_NAME`_clear(void)
{
	`$INSTANCE_NAME`_command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	CyDelay(2);  // this command takes a long time!
}

void `$INSTANCE_NAME`_home(void)
{
    `$INSTANCE_NAME`_command(LCD_RETURNHOME);  // set cursor position to zero
	CyDelay(2);  // this command takes a long time!
}

void `$INSTANCE_NAME`_setCursor(uint8_t col, uint8_t row)
{
	const uint8_t row_offsets[4] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > `$INSTANCE_NAME`_rows) {
		row = `$INSTANCE_NAME`_rows - 1;    // we count rows starting w/0
	}
	`$INSTANCE_NAME`_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void `$INSTANCE_NAME`_noDisplay(void)
{
    `$INSTANCE_NAME`_displaycontrol &= ~LCD_DISPLAYON;
	`$INSTANCE_NAME`_command(LCD_DISPLAYCONTROL | `$INSTANCE_NAME`_displaycontrol);
}

void `$INSTANCE_NAME`_display(void)
{
	`$INSTANCE_NAME`_displaycontrol |= LCD_DISPLAYON;
	`$INSTANCE_NAME`_command(LCD_DISPLAYCONTROL | `$INSTANCE_NAME`_displaycontrol);
}

// Turns the underline cursor on/off
void `$INSTANCE_NAME`_noCursor(void)
{
    `$INSTANCE_NAME`_displaycontrol &= ~LCD_CURSORON;
	`$INSTANCE_NAME`_command(LCD_DISPLAYCONTROL | `$INSTANCE_NAME`_displaycontrol);
}

void `$INSTANCE_NAME`_cursor(void)
{
    `$INSTANCE_NAME`_displaycontrol |= LCD_CURSORON;
	`$INSTANCE_NAME`_command(LCD_DISPLAYCONTROL | `$INSTANCE_NAME`_displaycontrol);
}

// Turn on and off the blinking cursor
void `$INSTANCE_NAME`_noBlink(void)
{
    `$INSTANCE_NAME`_displaycontrol &= ~LCD_BLINKON;
	`$INSTANCE_NAME`_command(LCD_DISPLAYCONTROL | `$INSTANCE_NAME`_displaycontrol);
}

void blink(void)
{
	`$INSTANCE_NAME`_displaycontrol |= LCD_BLINKON;
	`$INSTANCE_NAME`_command(LCD_DISPLAYCONTROL | `$INSTANCE_NAME`_displaycontrol);
}

// These commands scroll the display without changing the RAM
void `$INSTANCE_NAME`_scrollDisplayLeft(void)
{
	`$INSTANCE_NAME`_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void `$INSTANCE_NAME`_scrollDisplayRight(void)
{
    `$INSTANCE_NAME`_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void `$INSTANCE_NAME`_leftToRight(void)
{
	`$INSTANCE_NAME`_displaymode |= LCD_ENTRYLEFT;
	`$INSTANCE_NAME`_command(LCD_ENTRYMODESET | `$INSTANCE_NAME`_displaymode);
}

// This is for text that flows Right to Left
void `$INSTANCE_NAME`_rightToLeft(void)
{
	`$INSTANCE_NAME`_displaymode &= ~LCD_ENTRYLEFT;
	`$INSTANCE_NAME`_command(LCD_ENTRYMODESET | `$INSTANCE_NAME`_displaymode);
}

// This will 'right justify' text from the cursor
void `$INSTANCE_NAME`_autoscroll(void)
{
    `$INSTANCE_NAME`_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	`$INSTANCE_NAME`_command(LCD_ENTRYMODESET | `$INSTANCE_NAME`_displaymode);
}

// This will 'left justify' text from the cursor
void `$INSTANCE_NAME`_noAutoscroll(void)
{
    `$INSTANCE_NAME`_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	`$INSTANCE_NAME`_command(LCD_ENTRYMODESET | `$INSTANCE_NAME`_displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void `$INSTANCE_NAME`_createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7; // we only have 8 locations 0-7
	`$INSTANCE_NAME`_command(LCD_SETCGRAMADDR | (location << 3));
	for (uint32_t i = 0; i < 8; i++) {
		`$INSTANCE_NAME`_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void `$INSTANCE_NAME`_noBacklight(void)
{
    `$INSTANCE_NAME`_backlightval = LCD_NOBACKLIGHT;
	`$INSTANCE_NAME`_expanderWrite(0);
}

void `$INSTANCE_NAME`_backlight(void)
{
	`$INSTANCE_NAME`_backlightval = LCD_BACKLIGHT;
	`$INSTANCE_NAME`_expanderWrite(0);
}

/* [] END OF FILE */
