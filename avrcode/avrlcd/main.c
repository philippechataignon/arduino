/*
    File:       main.c
    Version:    1.0.0
    Date:       Feb. 25, 2006
    
    AVR 3-Wire Interface to a PC-1202-A LCD Display (4-wires if you want the AVR
    to control the backlight as well).
    
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


#include <avr/io.h>    
#include "lcd4.h"
#include <stdio.h>


int
main (void)
{
		int x;
         
        /* initialize the LCD */
		/* also configures the specified I/O port - see lcd4.h */
        lcd_initialize(LCD_FUNCTION_8x2, LCD_CMD_ENTRY_INC, LCD_CMD_ON);
        
        /* write my first name */
        lcd_putc('M'); 
        lcd_putc('i');
        lcd_putc('c');
        lcd_putc('a');
        lcd_putc('h');
        
        /* goto line 2 */
        lcd_move_cursor(LCD_LINE_2, 0);
        
        /* write my last name */
        lcd_putc('C');
        lcd_putc('a');
        lcd_putc('r');
        lcd_putc('r');
        lcd_putc('i');
        lcd_putc('c');
        lcd_putc('k');

		/*use printf style printing */

		x=15;

        lcd_move_cursor(LCD_LINE_2, 8); /* line 2, character 8 */
		lcd_printf("x=%d", x);        /* NB 1.7kbytes code size is mostly printf! */
		                                /* see libc documentation for how to reduce this */


        
        return (0);
}


