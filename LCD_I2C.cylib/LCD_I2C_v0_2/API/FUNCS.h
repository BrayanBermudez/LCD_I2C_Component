/****************************************************************************
* This file is part of the PCF8574AT Custom Component for PSoC Devices.
*
* Copyright (C) 2017 Carlos Diaz <carlos.santiago.diaz@gmail.com>
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

#ifndef `$INSTANCE_NAME`_FUNCS_H
#define `$INSTANCE_NAME`_FUNCS_H
    
#include <cytypes.h>
#include <cyfitter.h>

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

struct `$INSTANCE_NAME`_Config {
    uint8_t addr;
    uint8_t displayfunction;
    uint8_t displaycontrol;
    uint8_t displaymode;
    uint8_t cols;
    uint8_t rows;
    uint8_t charsize;
    uint8_t backlightval;
};

/**
 * Constructor
 *
 * @param lcd_addr	I2C slave address of the LCD display. Most likely printed on the
 *					LCD circuit board, or look in the supplied LCD documentation.
 * @param lcd_cols	Number of columns your LCD display has.
 * @param lcd_rows	Number of rows your LCD display has.
 * @param charsize	The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
*/
void `$INSTANCE_NAME`_start(void);

/*
 * Set the LCD display in the correct begin state, must be called before anything else is done.
*/
void `$INSTANCE_NAME`_begin(void);
	
/*
 * Remove all the characters currently shown. Next print/write operation will start
 * from the first position on LCD display.
*/
void `$INSTANCE_NAME`_clear(void);
	  
/*
 * Next print/write operation will will start from the first position on the LCD display.
*/
void `$INSTANCE_NAME`_home(void);

/*
 * Do not show any characters on the LCD display. Backlight state will remain unchanged.
 * Also all characters written on the display will return, when the display in enabled again.
*/
void `$INSTANCE_NAME`_noDisplay(void);
	  
/*
 * Show the characters on the LCD display, this is the normal behaviour. This method should
 * only be used after noDisplay() has been used.
*/ 
void `$INSTANCE_NAME`_display(void);

/*
 * Do not blink the cursor indicator.
*/
void `$INSTANCE_NAME`_noBlink(void);
	 
/*
 * Start blinking the cursor indicator.
*/ 
void `$INSTANCE_NAME`_blink(void);	 

/*
 * Do not show a cursor indicator.
*/
void `$INSTANCE_NAME`_noCursor(void);

/*
 * Show a cursor indicator, cursor can blink on not blink. Use the
 * methods blink() and noBlink() for changing cursor blink.*/ 
void `$INSTANCE_NAME`_cursor(void);

void `$INSTANCE_NAME`_scrollDisplayLeft(void);
void `$INSTANCE_NAME`_scrollDisplayRight(void);
void `$INSTANCE_NAME`_printLeft(void);
void `$INSTANCE_NAME`_printRight(void);
void `$INSTANCE_NAME`_leftToRight(void);
void `$INSTANCE_NAME`_rightToLeft(void);
void `$INSTANCE_NAME`_shiftIncrement(void);
void `$INSTANCE_NAME`_shiftDecrement(void);
void `$INSTANCE_NAME`_noBacklight(void);
void `$INSTANCE_NAME`_backlight(void);
void `$INSTANCE_NAME`_autoscroll(void);
void `$INSTANCE_NAME`_noAutoscroll(void); 
void `$INSTANCE_NAME`_createChar(uint8_t, uint8_t[]);
void `$INSTANCE_NAME`_setCursor(uint8_t, uint8_t);

void `$INSTANCE_NAME`_write(uint8_t data);
void `$INSTANCE_NAME`_command(uint8_t command);
void `$INSTANCE_NAME`_print(const char word[]);
// alias for backlight() and nobacklight()
void `$INSTANCE_NAME`_setBacklight(bool backlight);
// alias for createChar()
void `$INSTANCE_NAME`_loadCustomCharacter(uint8_t char_num, uint8_t *rows);
void `$INSTANCE_NAME`_pulseEnable(uint8_t data);
void `$INSTANCE_NAME`_expanderWrite(uint8_t data);
void `$INSTANCE_NAME`_send(uint8_t value, const uint8_t mode);
void `$INSTANCE_NAME`_write4bits(const uint8_t data);

#endif /* `$INSTANCE_NAME`_FUNCS_H */

/* [] END OF FILE */
