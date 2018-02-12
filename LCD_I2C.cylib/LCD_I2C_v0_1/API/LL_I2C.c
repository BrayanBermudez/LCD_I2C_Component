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

#include "project.h"
#include <stdint.h>
#include <stdbool.h>

#include "`$INSTANCE_NAME`_LL_I2C.h"

void `$INSTANCE_NAME`_write(uint8_t data)
{
	`$INSTANCE_NAME`_send(data, RS);
}

void `$INSTANCE_NAME`_command(uint8_t command)
{
    `$INSTANCE_NAME`_send(command, 0);
}

void `$INSTANCE_NAME`_print(const char *word)
{
    for (uint32_t i = 0; word[i] != '\0'; ++i) {
        `$INSTANCE_NAME`_write(word[i]);
    }
}

void `$INSTANCE_NAME`_setBacklight(bool backlight)
{
    backlight ? `$INSTANCE_NAME`_backlight() : `$INSTANCE_NAME`_noBacklight();
}

void `$INSTANCE_NAME`_loadCustomCharacter(uint8_t char_num, uint8_t *rows)
{
    `$INSTANCE_NAME`_createChar(char_num, rows);
}

void `$INSTANCE_NAME`_pulseEnable(uint8_t _data)
{
    // EN high
	`$INSTANCE_NAME`_expanderWrite(_data | EN);
    // enable pulse must be >450ns
	CyDelayUs(1);
    // En low
	`$INSTANCE_NAME`_expanderWrite(_data & ~EN);
    // commands need > 37us to settle
	CyDelayUs(50);
}

void `$INSTANCE_NAME`_expanderWrite(uint8_t _data)
{
	`$I2C_INTERFACE`_writeByte(`$INSTANCE_NAME`_addr, _data | `$INSTANCE_NAME`_backlightval);
}

// write either command or data
void `$INSTANCE_NAME`_send(uint8_t value, const uint8_t mode)
{
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	`$INSTANCE_NAME`_write4bits((highnib) | mode);
	`$INSTANCE_NAME`_write4bits((lownib) | mode);
}

void `$INSTANCE_NAME`_write4bits(const uint8_t data)
{    
	`$INSTANCE_NAME`_expanderWrite(data);
	`$INSTANCE_NAME`_pulseEnable(data);
}

void `$I2C_INTERFACE`_writeByte(const uint8_t addr, const uint8_t data)
{
#if defined(CY_SCB_`$I2C_INTERFACE`_H) // SCB Block
    `$I2C_INTERFACE`_I2CMasterSendStart(addr, 0);
    `$I2C_INTERFACE`_I2CMasterWriteByte(data);
    `$I2C_INTERFACE`_I2CMasterSendStop();
#else // UDB Block
    `$I2C_INTERFACE`_MasterSendStart(addr, 0);
    `$I2C_INTERFACE`_MasterWriteByte(data);
    `$I2C_INTERFACE`_MasterSendStop();
#endif
}

/* [] END OF FILE */
