// Send Hello! over serial every second

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

#define BAUD_PRESCALE(fcpu,br) ((fcpu / 16 / br) - 1)

void usartInit(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);
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

int usartDataAvailable()
{
    return UCSR0A & _BV(RXC0);
}

uint8_t usartGetRxByte()
{
    return UDR0;
}

int main()
{
    usartInit(9600);
    uint8_t str[] = "Hello!\r\n";

    // receives a char and sends it every second
    while (1) {
        usartSendStr(str);
        _delay_ms(1000);
    }
    return 0;
}
