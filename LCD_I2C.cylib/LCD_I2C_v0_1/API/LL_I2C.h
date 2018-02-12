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

#ifndef `$INSTANCE_NAME`_LL_I2C_H
#define `$INSTANCE_NAME`_LL_I2C_H
    
#include <cytypes.h>
#include <cyfitter.h>

#include <stdint.h>
#include <stdbool.h>

void `$INSTANCE_NAME`_write(uint8_t data);
void `$INSTANCE_NAME`_command(uint8_t command);
void `$INSTANCE_NAME`_print(const char word[]);
// alias for backlight() and nobacklight()
void `$INSTANCE_NAME`_setBacklight(bool backlight);
// alias for createChar()
void `$INSTANCE_NAME`_loadCustomCharacter(uint8_t char_num, uint8_t *rows);
void `$INSTANCE_NAME`_pulseEnable(uint8_t _data);
void `$INSTANCE_NAME`_expanderWrite(uint8_t _data);
void `$INSTANCE_NAME`_send(uint8_t value, const uint8_t mode);
void `$INSTANCE_NAME`_write4bits(const uint8_t data);
void `$I2C_INTERFACE`_writeByte(const uint8_t addr, const uint8_t data);

#endif /* `$INSTANCE_NAME`_LL_I2C_H */

/* [] END OF FILE */
