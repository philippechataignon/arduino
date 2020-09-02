// buffer receive and xon/xoff

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD_PRESCALE(fcpu,br) ((fcpu / 16 / br) - 1)

#define LIMIT 64
#define LIMIT_LOW 8

uint8_t rbuff[256] = {0};
uint8_t volatile h_rbuff = 0;
uint8_t volatile t_rbuff = 0;

void usart_init(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void send_byte(uint8_t ch)
{
    while (!(UCSR0A & _BV(UDRE0))) ;
    UDR0 = ch;
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

void read_byte() {
    if (h_rbuff != t_rbuff) {
        uint8_t ch = rbuff[t_rbuff++];
        uint8_t delta = h_rbuff - t_rbuff;
        if (delta < LIMIT_LOW) {
            // send XON
            send_byte(0x11);
        }
        use_char(ch);
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
    usart_init(9600);
    DDRB |= _BV(DDB5);
    /* set pin 5 low to turn led off */
    PORTB &= ~_BV(PORTB5);

    sei();
    // send XON
    send_byte(0x11);
    while(1) {
        read_byte();
        _delay_ms(10);
    }
    return 0;
}
