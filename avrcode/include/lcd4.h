/******************************************************************************
    File:       lcd4.h
    Version:    1.0.0
    Date:       Apr. 15, 2009
    
    C header file for HD44780 LCD module using a 74HCT164 serial in, parallel 
    out out shift register to operate the LCD in 4-bit mode.  Example schematic
    is available from EEE project web pages

    Modified by T.Clarke 2009 for 4 bit mode, and to separate machine config.
    
    Uses the avr-libc library for gcc (avr-gcc).
    
    ****************************************************************************
    Copyright (C) 2006 Micah Carrick   <email@micahcarrick.com>
              (C) 2009 Thomas Clarke   <t.clarke@ic.ac.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ****************************************************************************
*/
#ifndef _LCD4_H
#define _LCD4_H

#include <inttypes.h>
#include <avr/pgmspace.h>

/* Hardware-dependent constants */

#include "lcd4-hardware-config.h"



/* LCD commands */
#define LCD_CMD_HOME            0x02    /* cursor home */
#define LCD_CMD_ENTRY_DEC       0x04    /* decrement, no shift */
#define LCD_CMD_ENTRY_DEC_SHIFT 0x05    /* decrement with shift */
#define LCD_CMD_ENTRY_INC       0x06    /* increment, no shift */
#define LCD_CMD_ENTRY_INC_SHIFT 0x07    /* increment with shift */
#define LCD_CMD_CLEAR           0x01    /* clear, cursor home */
#define LCD_CMD_OFF             0x08    /* display off */
#define LCD_CMD_ON              0x0C    /* display on, cursor off */
#define LCD_CMD_ON_BLINK        0x0D    /* display on, cursor off, blink char */
#define LCD_CMD_ON_CURSOR       0x0E    /* display on, cursor on */
#define LCD_CMD_ON_CURSOR_BLINK 0x0F    /* display on, cursor on, blink char */
#define LCD_CMD_CURSOR_LEFT     0x10    /* decrement cursor (left) */
#define LCD_CMD_CURSOR_RIGHT    0x14    /* increment cursor (right) */
#define LCD_CMD_SHIFT_LEFT      0x18    /* shift (left) */
#define LCD_CMD_SHIFT_RIGHT     0x1C    /* shift (right)*/


/* 
 * NB LCD_ROWS must be set correctly for hardware to 1,2 or 4 
 * See lcd4_hardware_config
 */
#define LCD_LINE_1              0x00 /* bit 7 is always set: 0x80 = 0x00 */ 
#define LCD_LINE_2              0x40 /* 2nd line at position 40 */
#define LCD_LINE_3				(LCD_LINE_1+LCD_LINE_34_OFFSET) /* 3rd line after first */
#define LCD_LINE_4				(LCD_LINE_2+LCD_LINE_34_OFFSET) /* 4th line after second */

/* function set: (always 4-bit with the shift-register circuit I'm using)
 * ****** NB - changing this will alter the required LCD bias voltage!
 * If nothing is visible adjust bias till faint black rectangles can be seen 
 * Use MULTI_LINE for 4 line modules
 */
#define LCD_FUNCTION_ONE_LINE    0x20   /* 5x7 characters, single line display, 4 bit */
#define LCD_FUNCTION_MULTI_LINE  0x28   /* 5x7 characters, 2 line display, 4 bit */


/*
 * F_CPU is used by delay routines from util/delay.h. Should already be defined from
 * AVR Studio project. Typical values; 4000000 or 8000000 
 */

//#ifndef F_CPU
//#define F_CPU 8000000UL  /* 8 MHz CPU clock */
//#endif

/************************************************************************************
 *  LCD API functions                                                               *
 ************************************************************************************/

/*
 * switch LCD backlight on or off (non-zero => on)
 * backlight is driven on z-avr-lcd board from MSB of shift register
 */
void lcd_backlight(uint8_t);

/*
 * Initialize the LCD.  Must be called during the initialization of the 
 * program. Clears LCD and sets entry mode to increment position, no cursor
 *
 * Parameter:
 *      set_function    See LCD_FUNCTION_* definitions above
 */
void lcd_initialize(uint8_t set_function);



/*****************************************************************
 *                                                               *
 * LCD control. See above for possible LCD commands  (cmd)       *
 *                                                               *
 *****************************************************************/

void lcd_cmd( uint8_t cmd); /* cmd is one of LCD_CMD_* */




/******************************************************************
 *                                                                *
 * Move current cursor position                                   *
 * This is entry position for putchar whether cursor is on or off *
 * inc/dec only works properly within one line                    *
 * See above for line start addresses                             *
 *                                                                *
 ******************************************************************/
void lcd_cursor_home();

/* 
 * 
 * pos is number: LCD_LINE_* + rowpos
 * NB possible pos start from 0 and are contiguous within one line
 * Line ordering is NOT consecutive and depends on module type
 *
 */
void lcd_move_cursor(uint8_t pos);

/* move cursor within one line */
void lcd_inc_cursor(uint8_t);
void lcd_dec_cursor(uint8_t);


/******************************************************************
 * Write one char to LCD display                                  *
 ******************************************************************/
void lcd_putc(const char c);


/******************************************************************
 * This function is slow & resource-intensive - but implements    *
 * printf functionality to the lcd display.                       *
 *                                                                *
 *.                                                               *
 * characters are printed from the current cursor position        *
 * onwards using lcd_putc().                                      *
 *                                                                *
 * '\n' (0x0D) is interpreted as new line, it moves to next line  *
 * (& erases it)                                                  *
 * '\n' wraps round from last to first line hence                 *
 * "Hello world\n"                                                *
 * will rather surprisingly clear the screen on 1 line displays.  *
 *                                                                *
 * NB - printf will not wrap characters from one line to next     *
 * line start addresses are NOT contiguous nor (4 line displays)  *
 * monotonic on RAM. See lcd4.h for constants defining line       *
 * start addresses                                                *
 *                                                                *
 ******************************************************************/

/* main LCD print function */
int lcd_printf(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));

/* version with fmt string in flash - use PSTR("literal-string") as parameter */
int lcd_printf_P( const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));


/* clears LCD, prints out message, and halts */
void lcd_fatal_error_P( const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));


/* convenience wrapper printing out fixed error message as above */
#define fatal_error(message) lcd_fatal_error_P( PSTR(message))


#endif
