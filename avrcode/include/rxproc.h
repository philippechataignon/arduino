/*****************************************************************
 * rxproc.h                                                      *
 *                                                               *
 * Header for rxproc.c package                                   *
 *                                                               *
 * Rxproc implements a queue for received UART characters        *
 *                                                               *
 * It implements process_rx_char() which interfaces uart.c       *         
 *                                                               *
 *****************************************************************/
#ifndef _RXPROC_H
#define _RXPROC_H


/* 
 * Uncomment this to disable fatal error if characters are received
 * but lost due to buffer overflow. Normally buffer overflow should
 * never happen.
 */
//#define RXPROC_IGNORE_RX_BUFFER_OVERFLOW


/*****************************************************************
 * interface, called from uart.c interrupt                       *
 * on reception of character                                     *
 *****************************************************************/
void process_rx_char(char ch);


/*****************************************************************
 *                                                               *
 * Functions  & variables provided for use by user code          *
 *                                                               *
 *****************************************************************/

/* count of received characters lost through rx buffer overflow */
extern int volatile Rx_error_count;


/* initialise data structures (not needed at reset) */
void rxproc_init(void);



/* number of characters currently in buffer */
uint8_t usart_rxq_length(void);

/* read next received character in buffer, or block waiting */
int usart_getc(void);


/*
 * view next character in buffer (without removing it)
 * if no character return EOF (-1) and don't block
 * use usart_getc to remove character from buffer
 */
int usart_lookahead(void);


#endif


