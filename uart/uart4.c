#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BAUD_PRESCALE(fcpu,br) ((fcpu / 16 / br) - 1)

uint8_t buff[256] = {0};
uint8_t n = 0;


void usartInit(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0) | _BV(TXCIE0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void usartSendByte(uint8_t data)
{
    while (!(UCSR0A & _BV(UDRE0))) ;
    UDR0 = data;
}

void usartSendStr(uint8_t str[])
{
    for (uint8_t* c=str; *c; c++) {
        usartSendByte(*c);
    }
}

ISR(USART_RX_vect)
{
    uint8_t ch = UDR0;
    buff[n] = ch;
    buff[n+1] = 13;
    buff[n+2] = 10;
    buff[n+3] = 0;
    n++;
    if (n > 250)
        n = 0;
}

ISR(USART_TX_vect)
{
    n = 0;
}

int main(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    usartInit(9600);
    sei();
    while(1) {
        if (buff[0]) {
            usartSendStr(buff);
        }
        _delay_ms(1000);
    }
    return 0;
}
