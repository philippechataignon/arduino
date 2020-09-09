#include "uart.h"
#include "rxproc.h"
#include <stdio.h>
#include <avr/io.h>
#include "lcd4.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

#define RX_Q_INCR(x) { x = x+1; if (x == RX_BUFF_SIZE) x = 0;}

/*
 * data structures for queue
 *
 * It is VERY IMPORTANT that global variables which may be changed by
 * interrupt code are tagged volatile, as below.
 *
 * Failure to do this here leads to a disastrous intermittent bug
 * where rxq.length is occasionally not incremented when it should be!
 */
struct q_t {
    unsigned char head_pos;
    unsigned char volatile empty_pos;
    uint8_t volatile length;
    unsigned char buffer[RX_BUFF_SIZE];
};

static struct q_t rxq;

int volatile Rx_error_count;

void rxproc_init(void)
{
    rxq.head_pos = 0;
    rxq.empty_pos = 0;
    rxq.length = 0;
}

/* this function is called from interrupt in uart.c */
void process_rx_char(char ch)
{
    if (rxq.length < RX_BUFF_SIZE) {
        rxq.buffer[rxq.empty_pos] = ch;
        RX_Q_INCR(rxq.empty_pos);
        rxq.length = rxq.length + 1;
    } else {
        /* error:  rx character has been lost! */
        Rx_error_count++;
#ifndef RXPROC_IGNORE_RX_BUFFER_OVERFLOW
        fatal_error("Rx UART buffer overflow");
#endif
    }
}

int usart_getc(void)
{
    uint8_t ch;
    while (rxq.length == 0) ;   /* wait till char is available */
    /*
     * NB - rxq.length must be declared volatile else this while
     * loop will optimise incorrectly when compiled.
     */
    ch = rxq.buffer[rxq.head_pos];
    RX_Q_INCR(rxq.head_pos);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        rxq.length = rxq.length - 1;
    }
    return ch;
}

int usart_lookahead(void)
{
    if (rxq.length == 0)
        return EOF;
    else
        return rxq.buffer[rxq.head_pos];
}

uint8_t usart_rxq_length(void)
{
    return rxq.length;
}
