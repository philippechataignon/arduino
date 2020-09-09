/*
 * main.c
 */

/********************************************************************************
 *                                                                              *
 * Sample code controlling zibit module from AVR microcontroller using SerialNet*
 *                                                                              *
 * This code can be modified as required                                        *
 *                                                                              *
 * Functions used are documented in header files (include\*.h)                  *
 *                                                                              *
 * The serial link to the Zigbit is interrupt driven, so interrupts must be     *
 * switched on for the code to work.                                            *
 *                                                                              *
 *******************************************************************************/


#include "lcd4.h"
#include "clockset.h"
#include "uart.h"
#include "rxproc.h"
#include "zigbit.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>


#define TEST_VIEW_ON 1 /* 1 => display every command as it is sent, 0=> do not */
#define TEST_BLINK_TIME 20 /* time in ms waiting between sending commands to switch LED on & off */

#define TEST_LCD_FUNCTION LCD_FUNCTION_ONE_LINE

/*
 * test_serialnet() initialises the zigbit module & then outputs some sample
 * commands
 *
 */

void test_serialnet(void)
{
  int j, rc;
  char ch;

  usart_init(); // initialise serial link functions & switch interrupts on

  _delay_ms(1000); // wait 1s

  lcd_cmd(LCD_CMD_CLEAR); //clear the LCD

  send_command_wait_rx_ok_flash("AT&F\r", TEST_VIEW_ON); // reset zigbit to factory settings
  //
  // These commands configure Zigbit as node in a network
  // For standalone testing it is configured as controller and will work but
  // do nothing even if there are no other nodes
  //
  // see SerialNet command Reference for detailed decription of commands.
  send_command_wait_rx_ok_flash("ATX\r",TEST_VIEW_ON); // send to host EVENT & DATA info
  send_command_wait_rx_ok_flash("AT+GSN=1\r",TEST_VIEW_ON); // set MAC
  send_command_wait_rx_ok_flash("AT+WPANID=1620\r",TEST_VIEW_ON); // set PAN ID
  send_command_wait_rx_ok_flash("AT+WCHMASK=100000\r",TEST_VIEW_ON); // set channel mask
  send_command_wait_rx_ok_flash("AT+WROLE=0 +WSRC=0\r",TEST_VIEW_ON); // set function=controller, logical address=0
  send_command_wait_rx_ok_flash("AT+WAUTONET=1 Z\r",TEST_VIEW_ON); //
  send_command_wait_rx_ok_flash("ATS120=3\r",TEST_VIEW_ON); // set GPIO0 to output

  for (j=0;;j++) {
  // main loop blinking LED

     _delay_ms(TEST_BLINK_TIME);

#if TEST_BLINK_TIME > 200
     lcd_cmd(LCD_CMD_CLEAR); /* clear LCD if delay is long enough to read it */
#else
	 lcd_move_cursor(0);
#endif

     //set GPIO0 to either logical 0 (LED on) or 1 (LED off) as j is even or odd
	 // Note - printf_P( fmt, ...) works like printf(fmt,...) but format string fmt
	 // must be enclosed in PSTR(...) and therefore defined in flash memory, so
	 // savng RAM over a normal string constant (which is defined in flash and copied
	 // to RAM. Flash string constants are NOT compatible with standard (RAM) data
	 // constants but pgnspace.h has many functions which will work directly on flash
	 // string constants
     usart_printf_P( PSTR("ATS130=%c\r"), (j % 2 == 0)? '1' : '0');

	 // print iteration number on LCD
     lcd_printf_P( PSTR("#%d."), j);

	 // wait till 'OK' reply
	 rx_info_code = 0;
	 rc = wait_for_rx_ok(1) ;

	 switch (rx_info_code) { // indicates if unexpected information reply is received
	   case IS_JOINED:  ch = 'J'; break;
	   case IS_LOST:    ch = 'L'; break;
	   case IS_DATA:    ch = 'D'; break;
	   case 0:          ch = ' '; break; // normal case, no unexpected reply
	   default:         ch = 'M'; break; // if more than one info reply is detected
	 }
	 if (rc == ERROR_REPLY) { // if command failed (ERROR reply)
	    lcd_printf_P( PSTR("Error"));
		for (;;);
     }
	 if (rc == ERROR_WAIT_TIMEOUT) { // if no reply print error indication & wait
	    lcd_printf_P(PSTR("E%cE(%d)"), ch, Rx_error_count);
		for (;;);
     }
	 if (ch !=' ') lcd_printf_P(PSTR("*%c*"), ch); // if info received print code
  }
}


//initialise & test the LCD display
void test_lcd(void)
{ 
   int i;
   
   lcd_initialize(TEST_LCD_FUNCTION);
   for ( i=0; i < 1; i++) {
      lcd_backlight(1);
      _delay_ms(200);
      lcd_backlight(0);
	  _delay_ms(200);
   }

   lcd_cmd(LCD_CMD_HOME);
   lcd_printf_P( PSTR("LCD OK!"));


   _delay_ms(1000);
   lcd_cmd(LCD_CMD_CLEAR);
}



//initialise the AVR to well-defined state
void init_cpu(void)
{
   /* performs initialisation to make sure CPU is in normal state */
   /* and no unwanted interrupts or watchdog timeouts will happen */
   /* Normally this is unnecessary after reset, but it is safer */
   /* After this I/O ports which are used as outputs or with pullups */
   /*  should be configured, other devices configured, etc */
   asm("cli");
   MCUCR = 0x00;
   EICRA = 0x00;  //extended ext ints
   EIMSK = 0x00;
 
   TIMSK0 = 0x00; //timer 0 interrupt sources
   TIMSK1 = 0x00; //timer 1 interrupt sources
   TIMSK2 = 0x00; //timer 2 interrupt sources
 
   PCMSK0 = 0x00; //pin change mask 0 
   PCMSK1 = 0x00; //pin change mask 1 
   PCMSK2 = 0x00; //pin change mask 2
   PCICR = 0x00;  //pin change enable 
   PRR = 0x00;    //power controller
   WDTCSR = 0;    // watchdog controller
   MCUSR = 0;     // reset source information
}


int main()
{
   init_cpu();
   test_lcd();//initialize LCD & print test message
   calibrate_internal_rc_osc(); // calibrate internal oscillator
   test_serialnet(); // send repeated commands to Zigbit diaplaying results */

   return 0;
}
