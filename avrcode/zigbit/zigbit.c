//zigbit.c


/*****************************************************************************
 *                                                                           *
 *    Sample functions to perform host function interfacing with SerialNet   *
 *                                                                           *
 *        This code runs on AVR microcontroller with LCD                     *
 *                                                                           *
 *****************************************************************************/

#include "lcd4.h"
#include "uart.h"
#include "rxproc.h"
#include "zigbit.h"
#include <util/delay.h>



#define NUM_CHARS_TO_CHECK 20


char data_buffer[NUM_DATA_CHARS+1];

uint8_t rx_info_code;

#define IS_STR(str) (!strcmp_P( chars +(NUM_CHARS_TO_CHECK + 1 - sizeof(str)), PSTR(str)))


int wait_for_rx_ok(uint8_t print)
{
   uint8_t i,j;
   uint8_t data_index = -1;
   char ch;
   char chars[NUM_CHARS_TO_CHECK+1];

   for (i=0; i < sizeof(chars); i++) chars[i] = 0;


   for (i=0; i < ZIGBIT_WAIT_FOR_REPLY_TIME_MS; i++) {
      if (usart_lookahead() >= 0) {
		 ch = usart_getc();
		 if (print) {
		    if (ch == '\r') lcd_printf(".");
		    else if (ch < 0x20 || ch == 0x7f || ch == ' ') ;
			else lcd_putc(ch);
         }

         for (j=0; j < NUM_CHARS_TO_CHECK-1; j++) chars[j] = chars[j+1];
		 chars[NUM_CHARS_TO_CHECK-1] = ch;

		 if (data_index >= 0) { // add ch to data_buffer
		    data_buffer[data_index++]=ch;
			if (data_index > NUM_DATA_CHARS) return ERROR_DATA_OVERFLOW;
         }

		 if ( IS_STR("OK\r"))               return 0; //normal end

		 if ( IS_STR("EVENT:JOINED\r")) rx_info_code |= IS_JOINED; //event

		 if ( IS_STR("ERROR\r"))            return ERROR_REPLY; // error end

		 if ( IS_STR("EVENT:LOST\r"))       rx_info_code |= IS_LOST; //event

		 if ( IS_STR("\r\f") && (data_index >= 0)) { //terminate data reception
		                                      data_buffer[data_index] = 0;
											  data_index = -1;
			                                  rx_info_code |= IS_DATA; }

		 if ( IS_STR("\r\fDATA") )            data_index = 0; // start recording data
      }
    _delay_ms(1);
   }
   return ERROR_WAIT_TIMEOUT; //timeout error end
}

void send_command_wait_rx_ok(char *str, uint8_t view)
{
   usart_printf(str);
   if (wait_for_rx_ok(1)==ERROR_WAIT_TIMEOUT) {
      lcd_printf("No reply!");
	  for (;;);
   }
   if (view > 0) {
      _delay_ms(ZIGBIT_COMMAND_VIEW_TIME_MS);
      lcd_cmd(LCD_CMD_CLEAR);
   }
}

void send_command_wait_rx_ok_P(PGM_P str, uint8_t view)
{
   char comstr[TX_BUFF_SIZE+1];
   if (strlen_P(str)>TX_BUFF_SIZE) {
      lcd_fatal_error_P(PSTR("Error: command %c%c%c%c... too long"),str[0],str[1],str[2],str[3]);
   } 
   strcpy_P(comstr, str); //copy from flash to RAM
   send_command_wait_rx_ok(comstr, view);
}

