// uart with buffer receive and xon/xoff

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

/* 75 ms = 1172 * 64 us */
#define DELAY 1172

typedef struct {
    uint8_t buff[256];
    uint8_t volatile head;
    uint8_t volatile tail;
} buffer;

buffer receive = {{0}, 0, 0};

void usart_init(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void send_byte(uint8_t ch)
{
    while (!(UCSR0A & _BV(UDRE0)));
    UDR0 = ch;
}

void send_str(uint8_t str[])
{
    for (uint8_t* c = str; *c; c++) {
        send_byte(*c);
    }
}

void use_char(uint8_t ch) {
    // _delay_ms(50);
}

static void timer_stop()
{
    TCCR1B = B00000000;         /* stop timer clock */
    clr_bit(TIMSK1, TOIE1);     /* disable interrupt */
    set_bit(TIFR1, TOV1);       /* clear interrupt flag */
}

/* at 16MHz, 1 tick = 1s / fcpu * div = 1000000 / 16000000 * 1024 = 64 ms */
/* INTR in value * 64 ms */
static void timer_start(int value)
{
    cli(); /* no interrupt because TCNT1 is 16 bits = 2 cycles to write */
    clr_bit(TCCR1A, WGM10);
    clr_bit(TCCR1A, WGM11);
    set_bit(TIFR1, TOV1);
    TCNT1 =  0xFFFF - (value & 0xFFFF);    /* overflow in value * 64 us*/
    TIMSK1 = B00000001;         /* set the Timer Overflow Interrupt Enable bit */
    TCCR1B = B00000101;         /* prescaler: 1024 */
    sei();
}

ISR(TIMER1_OVF_vect)
{
    cli();
    timer_stop();
    if (receive.head != receive.tail) {
        uint8_t ch = receive.buff[receive.tail++];
        uint8_t delta = receive.head - receive.tail;
        if (delta < LIMIT_LOW) {
            // send XON
            send_byte(0x11);
        }
        use_char(ch);
    }
    timer_start(DELAY);
    sei();
}

ISR(USART_RX_vect)
{
    cli();
    uint8_t ch = UDR0;
    receive.buff[receive.head++] = ch;
    uint8_t delta = receive.head - receive.tail;
    if (delta > LIMIT) {
        // send XOFF
        send_byte(0x13);
    }
    sei();
}

int main(void)
{
    usart_init(9600);
    timer_start(DELAY);

    /* set pin 5 low to turn led off */
    DDRB |= _BV(DDB5);
    PORTB &= ~_BV(PORTB5);

    sei();
    // send XON
    send_byte(0x11);
    while(1) {
        sleep_mode();
    }
    return 0;
}
