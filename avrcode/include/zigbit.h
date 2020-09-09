/*
 * zigbit.h
 *
 * Serialnet command set interface
 *
 */

#ifndef _ZIGBIT_H
#define _ZIGBIT_H
#include <inttypes.h>
#include <avr/pgmspace.h>


/* time to view each command & reply after it is sent */
/* for fast commands set view = 0 to disable this time */
#define ZIGBIT_COMMAND_VIEW_TIME_MS 700 

/*
 * Max time to wait for 'OK' reply from zigbit after command
 * normally response is quick bit &F command takes a few seconds
 */
#define ZIGBIT_WAIT_FOR_REPLY_TIME_MS 3000



/* 
 * low-level function waits for 'OK' response from Zigbit
 * if print is non-zero prints on LCD all
 * characters received while waiting.
 */
int wait_for_rx_ok(uint8_t print);



/* rx_info_code bits */
#define IS_DATA 1 /* data packet has been received and is in data_buffer */
#define IS_JOINED 2 /* JOINED event received */
#define IS_LOST 4 /* LOST event received */

/* return codes from wait_for_rx_ok if no reply received or error */
#define ERROR_WAIT_TIMEOUT 1
#define ERROR_DATA_OVERFLOW 2
#define ERROR_REPLY 3


extern uint8_t rx_info_code; /* returned by wait_for_rx_ok */


#define NUM_DATA_CHARS 50 /*size must be >= max size of data packet sent */


extern char data_buffer[NUM_DATA_CHARS+1]; /* buffer for data received at any time */


/*
 * sends a command str to Zigbit & then waits for 'OK' reply
 * If no reply prints 'No reply' on LCD and hangs.
 *
 * If view is non-zero it prints all received characters,
 * waits some time after reply is received, then clears the LCD
 * ready to display next command.
 * Note that this works if Zigbit is set to echo characters sent, so
 * that the sent command is diaplayed on LCD.
 */

/* interface to functions which puts the command string in flash memory to save RAM */
#define send_command_wait_rx_ok_flash(str,view) send_command_wait_rx_ok_P(PSTR(str), view)

/* send a command - str is in RAM - normal case */
void send_command_wait_rx_ok(char *str, uint8_t view);

/* send a command - str in flash */
void send_command_wait_rx_ok_P(PGM_P str, uint8_t view);



#endif
