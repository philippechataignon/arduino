/*
    File:       lcd4.c
    Version:    0.1.0
    Date:       Feb. 25, 2006
    
    C header file for HD44780 LCD module using a 74HCT164 serial in, parallel 
    out out shift register to operate the LCD in 4-bit mode.  Example schematic
    usuing a Powertip PC-1202A LCD available at http://www.micahcarrick.com

	Modified by Tom Clarke, 2009.
	   4 bit mode
	   Backlight from shift register
	   '\n' moves to & erases next line
	   printf wrapper
	   Minor bug fixes
    
    Uses the avr-libc library for gcc (avr-gcc).
    
    ****************************************************************************
    Copyright (C) 2006 Micah Carrick   <email@micahcarrick.com>

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


#include "lcd4.h"
#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdarg.h>

static void lcd_load_byte4(uint8_t);
static void lcd_load_byte(uint8_t out_byte);
static void lcd_strobe_E(void);
static void lcd_send_cmd(void);


static uint8_t lcd_has_been_initialized; // set to 0 by C init code, 1 when LCD is initialised


/*
 * Turns the backlight on or off. 
 * Parameters:
 *      backlight_on    0=off, 1=on
 */

static uint8_t last_byte_sent;
static uint8_t backlight;
static uint8_t lcd_line_start;

void 
lcd_backlight(uint8_t backlight_on)
{
/* MSB of 74HCT164 shift register controls backlight                          *
 * any LCD activity will update it with current value, stored in backlight    *
 * can use other 3 bits in same way, but note these will not be stable during *
 * LCD activity. For backlight transient glitches do not matter               */
        if (backlight_on)
        {
                /* LCD Backlight on */
                backlight = 0x80;               
        } else {
                /*  LCD Backlight off */
                backlight = 0;
        }
		lcd_load_byte4(0);
}

/*
 * Initializes the LCD.  Should be called during the initialization of the 
 * program.
 *
 * Parameters:
 *      set_function    See LCD_FUNCTION_* definitions in lcd.h
 *      set_entry_mode  See LCD_CMD_ENTRY_* definitions in lcd.h
 *      on              See LCD_CMD_ON_* definitions in lcd.h
*/


static void reset_cmd( uint8_t cmd)
{
        lcd_load_byte4(cmd);
        LCD_PORT &= ~_BV(LCD_RSDS_PIN); 
        lcd_strobe_E();
		//lcd_strobe_E();

        _delay_ms(10);
}	


void lcd_cmd( uint8_t cmd)
{
	lcd_load_byte(cmd);
	lcd_send_cmd();
	if (cmd < 4) // longer commands
		_delay_ms(4);

}



void
lcd_initialize(uint8_t set_function)
{
		LCD_DDR |= LCD_PINS; /* set LCD pins to outputs */
		LCD_PORT &=~LCD_PINS; /* set all pins to 0 */

        backlight = 0; /* ensure initialisation output does not switch backlight on */

		lcd_load_byte4(0); /* switch backlight off ASAP in case it powers up on*/

        /* 30ms delay while LCD powers on */
        _delay_ms(40);	   
        
        reset_cmd(3);
		_delay_ms(10);
		reset_cmd(3);
		reset_cmd(3);
		reset_cmd(2);
		lcd_cmd( set_function);
		lcd_cmd(LCD_CMD_OFF);
		lcd_cmd(LCD_CMD_CLEAR);
		lcd_cmd(LCD_CMD_ENTRY_INC);
		lcd_cmd(LCD_CMD_ON);

		lcd_move_cursor(LCD_LINE_1);	

		lcd_line_start = LCD_LINE_1;

		lcd_has_been_initialized = 1;
};

/*
 * Loads a byte into the shift register (74'164).  Does NOT load into the LCD.
 *
 * Parameters:
 *      out_byte        The byte to load into the '164.
*/
static void 
lcd_load_byte4(uint8_t out_byte)
{
       /* add top 4 bits from backlight variable */
	   out_byte = (out_byte & 0xF) | (backlight & 0xF0);

       /* make sure clock is high */
       LCD_PORT |= _BV(LCD_CLOCK_PIN);
        int i;
        for(i=0; i<8; i++)
        {
                /* loop through bits */
                
                if (out_byte & 0x80)
                {
                        /* this bit is high */
                        LCD_PORT |=_BV(LCD_RSDS_PIN); 
                }
                else
                {
                        /* this bit is low */
                        LCD_PORT &= ~_BV(LCD_RSDS_PIN);
                }
                out_byte = out_byte << 1;
			     _delay_us(1);           
                /* pulse the the shift register clock */
                LCD_PORT &= ~_BV(LCD_CLOCK_PIN);
				_delay_us(1);
                LCD_PORT |= _BV(LCD_CLOCK_PIN); /*active dege shfts data */
        }
}

static void lcd_load_byte(uint8_t out_byte)
{
	last_byte_sent = out_byte;
}

/*
 * Loads the byte in the '164 shift register into the LCD as a command. The
 * '164 should already be loaded with the data using lcd_load_byte4().
 */


static void lcd_send_cmd(void)
{
    lcd_load_byte4(last_byte_sent>>4);
    LCD_PORT &= ~_BV(LCD_RSDS_PIN); 
    lcd_strobe_E();
	lcd_load_byte4(last_byte_sent);
    LCD_PORT &= ~_BV(LCD_RSDS_PIN); 
    lcd_strobe_E();
    _delay_us(60);
}



/*
 * Loads the byte in the '164 shift register into the LCD as a character. The
 * '164 should already be loaded with the data using lcd_load_byte4().
 */


void lcd_send_char(void)
{
    lcd_load_byte4(last_byte_sent>>4);
    LCD_PORT |= _BV(LCD_RSDS_PIN); 
    lcd_strobe_E();
	lcd_load_byte4(last_byte_sent);
    LCD_PORT |= _BV(LCD_RSDS_PIN); 
    lcd_strobe_E();
    _delay_us(60);
}


/*
 * Loads the byte into the shift register and then sends it to the LCD as a char
 * Parameters:
 *      c               The byte (character) to display
 * Time taken = 80us
 */
void 
lcd_putc(const char c)
{
        lcd_load_byte(c);
        lcd_send_char();
}

/*
 * Strobes the E signal on the LCD to "accept" the byte in the '164.  The RS
 * line determines whether the byte is a character or a command.
*/
static void
lcd_strobe_E(void)
{
        /* strobe E signal */
		_delay_us(1);
        LCD_PORT |= _BV(LCD_ENABLE_PIN);
        _delay_us(1); /*Corrected by TJWC (was 450)*/
        LCD_PORT &= ~_BV(LCD_ENABLE_PIN);
}

/*
 * Moves the cursor to the home position.
*/
void
lcd_cursor_home()
{
        lcd_cmd(LCD_CMD_HOME);
		lcd_line_start = LCD_LINE_1;
}

/*
 * Moves the cursor to the specified position.
 * Parameters:
 *      line            Line (row)
 *      pos             Position on that line (column)
*/
void
lcd_move_cursor(uint8_t pos)
{
        lcd_cmd(0x80+pos);
		lcd_line_start=pos;
}


static uint8_t lcd_next_line_start(void)
{
#if LCD_ROWS == 1
   return 0;
#endif

#if LCD_ROWS == 2
   if (lcd_line_start) {
   if (lcd_line_start < LCD_LINE_2) return LCD_LINE_2;
   else: return LCD_LINE_1;
#endif

#if LCD_ROWS == 4
   if (lcd_line_start < LCD_LINE_2) {
      if (lcd_line_start < LCD_LINE_3) return LCD_LINE_3;
	  else return LCD_LINE_1;
   } else {
   	  if (lcd_line_start < LCD_LINE_4) return LCD_LINE_4;
      else return LCD_LINE_1;
   }
#endif
}


/*
 * Moves the cursor a number of spaces to the right
 * Parameters:
 *      spaces          Number of spaces to move
*/
void
lcd_inc_cursor(uint8_t spaces)
{
        while (spaces--)
        {
                lcd_cmd(LCD_CMD_CURSOR_RIGHT);    
        }
}

/*
 * Moves the cursor a number of spaces to the left
 * Parameters:
 *      spaces          Number of spaces to move
*/
void
lcd_dec_cursor(uint8_t spaces)
{
        while (spaces--)
        {
                lcd_cmd(LCD_CMD_CURSOR_LEFT);
        }
}



/*
 *
 * Helper functions for lcd_printf
 *
 */

static int lcd_putchar(char c, FILE *stream);

/* File stream used by lcd_printf() */
static FILE lcd_out = FDEV_SETUP_STREAM(lcd_putchar, NULL,
                                             _FDEV_SETUP_WRITE);

/* Wrapper function for lcd_putc() used by lcd_printf() */
static int lcd_putchar(char c, FILE *stream)
{
   uint8_t i;
   static char errstr[] = "Err no LCD init";
   if (lcd_has_been_initialized == 0) {
      lcd_initialize(LCD_FUNCTION_ONE_LINE);
	  for (i=0; errstr[i]; i++) lcd_putc(errstr[i]);
	  for (;;);
   }
   if (c == '\n') { /* take special action if \n is received */
      lcd_line_start = lcd_next_line_start(); 
      lcd_move_cursor(lcd_line_start);  /* advance one line (will wrap round) */
      for (i=0; i< LCD_COLS; i++) lcd_putc(' '); /*blank new line */
      lcd_move_cursor( lcd_line_start);
   }
   else 
      lcd_putc(0x7f & c);
   return 0;
}

/*
 * This function is slow & resource-intensive - but implements
 * printf functionality to the lcd display.
 *
 *.
 * characters are printed from the current cursor position onwards
 * using lcd_putc().
 *
 * '\n' is interpreted as new line, it moves to next line (& erases it)
 * '\n' wraps round from last to first line hence  "Hello world\n" will
 * rather surprisingly clear the screen on 1 line displays.
 *
 * NB - printf will not automatically wrap line to line
 * line start addresses are NOT contiguous nor (4 line displays)
 * monotonic on RAM. See lcd4.h for constants defining line start
 * addresses. Behaviour if printing beyond line end can be unpredictable.
 *
 */


int lcd_printf( const char *fmt, ...)
{
   va_list al;
   va_start(al, fmt);
   return vfprintf(&lcd_out, fmt, al);
}

int lcd_printf_P( const char *fmt, ...)
{
   va_list al;
   va_start(al, fmt);
   return vfprintf_P(&lcd_out, fmt, al);
}

void lcd_fatal_error_P( const char *fmt, ...)
{
   va_list al;
   va_start(al, fmt);
   lcd_cmd(LCD_CMD_CLEAR);
   vfprintf_P(&lcd_out, fmt, al);
   for (;;);
}

