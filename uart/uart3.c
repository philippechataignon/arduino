#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BAUD_PRESCALE(fcpu,br) ((fcpu / 16 / br) - 1)

void usartInit(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void usartSendByte(uint8_t data)
{
    while (!(UCSR0A & _BV(UDRE0))) ;
    UDR0 = data;
}

int usartDataAvailable()
{
    return UCSR0A & _BV(RXC0);
}

uint8_t usartGetRxByte()
{
    return UDR0;
}

ISR(USART_RX_vect)
{
    uint8_t Temp;
    //Store data to temp
    Temp=UDR0;
    Temp++;//increment
    //send received data back
    // no need to wait for empty send buffer
    UDR0=Temp;
}

int main(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    usartInit(9600);
    sei();
    while(1)
        sleep_mode();
    //nothing here interrupts are working
    return 0;
}
