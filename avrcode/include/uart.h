/*********************
 * Header for uart.c *
 *********************/
#ifndef _UART_H
#define _UART_H

#include <inttypes.h>

/* termiation characters used by Zigbit modules */
#define S3TRAILER 13
#define S4TRAILER 10

#define HARDWAREFLOWCONTROL 0 /* non-zero enables hardware flow control on Tx */


/* size of Tx buffer - when buffer is full usart_printf will block as necessary */
#define TX_BUFF_SIZE 20

/* size of Rx buffer - when buffer is full received chars will be dropped */
#define RX_BUFF_SIZE 30

/* baud rate for serial link */
#define BAUD 38400

//#define RECORD_ERRORS /*uncomment for extra diagnostic info */




// global variables exported from uart.c
#ifdef RECORD_ERRORS
extern int volatile framing_errors;
extern int volatile overrun_errors;
extern int volatile should_never_happen_errors;
#endif


// initialise hardware & q data structures
extern void usart_init(void);

// send char ch via serial port (asynchronous)
extern void usart_send(char ch);

// called from Rx interrupt once for each received character ch
// see rxproc.c for implementation of process_rx_char
extern void process_rx_char(char ch);


// higher level formatted output to UART
int usart_printf( const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));

/* fmt must be flash string - use PSTR(...) */
int usart_printf_P( const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));


/* number of characters recieved but not yet read and stored in buffer */
uint8_t usart_txq_length(void);


#endif




