// buffer receive and xon/xoff

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "binary.h"

#define BAUD_PRESCALE(fcpu,br) ((fcpu / 16 / br) - 1)

#define LIMIT 64
#define LIMIT_LOW 8

uint8_t rbuff[256] = {0};
uint8_t volatile h_rbuff = 0;
uint8_t volatile t_rbuff = 0;
uint8_t wbuff[256] = {0};
uint8_t volatile h_wbuff = 0;
uint8_t volatile t_wbuff = 0;

void usart_init(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0) | _BV(UDRIE0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void send_byte(uint8_t ch)
{
    wbuff[h_wbuff++] = ch;
    // uint8_t volatile delta = h_wbuff - t_wbuff;
    // if (delta > LIMIT) {
    //     // wait until under LIMIT_LOW
    //     while (delta >= LIMIT_LOW) {
    //         delta = h_wbuff - t_wbuff;
    //     }
    // }
    // UDR intr on
    UCSR0B |= _BV(UDRIE0);
}

void send_str(uint8_t str[])
{
    for (uint8_t* c = str; *c; c++) {
        send_byte(*c);
    }
}

void use_char(uint8_t ch) {
    _delay_ms(75);
}

int inline is_byte_readable() {
    return h_rbuff != t_rbuff;
}

int inline is_byte_writable() {
    return h_wbuff != t_wbuff;
}

static void timer_init(void)
{
    /* normal mode */
    clr_bit(TCCR1A, WGM10);
    clr_bit(TCCR1A, WGM11);
    set_bit(TIFR1, TOV1);
    TCNT1 =  0;                 /* overflow in ticks*1024 clock cycles */
    TIMSK1 = B00000001;         /* set the Timer Overflow Interrupt Enable bit */
    TCCR1B = B00000101;         /* prescaler: 1024 */
}

ISR(TIMER1_OVF_vect)
{
    if (is_byte_readable()) {
        uint8_t ch = rbuff[t_rbuff++];
        use_char(ch);
    }
    uint8_t delta = h_rbuff - t_rbuff;
    if (delta < LIMIT_LOW) {
        // send XON
        send_byte(0x11);
    }
}

ISR(USART_UDRE_vect)
{
    if (is_byte_writable()) {
        UDR0 = wbuff[t_wbuff++];
    } else {
        // UDR intr off
        UCSR0B &= ~_BV(UDRIE0);
    }
}

ISR(USART_RX_vect)
{
    uint8_t ch = UDR0;
    rbuff[h_rbuff++] = ch;
    uint8_t delta = h_rbuff - t_rbuff;
    if (delta > LIMIT) {
        // send XOFF
        send_byte(0x13);
    }
}

int main(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    usart_init(9600);
    timer_init();
    sei();
    // send XON
    send_byte(0x11);
    while(1) {
        sleep_mode();
    }
    return 0;
}

