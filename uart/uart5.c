#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define BAUD_PRESCALE(fcpu,br) ((fcpu / 16 / br) - 1)

uint8_t rbuff[256] = {0};
uint8_t wbuff[256] = {0};
uint8_t n_rbuff = 0;
uint8_t n_wbuff = 0;
uint8_t s_wbuff = 0;


void usart_init(uint32_t baudRate)
{
    UBRR0L = BAUD_PRESCALE(F_CPU, baudRate) & 0xFF;
    UBRR0H = BAUD_PRESCALE(F_CPU, baudRate) >> 8;
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0) | _BV(UDRIE0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void send_byte(uint8_t ch)
{
    cli();
    wbuff[n_wbuff++] = ch;
    // UDR intr on
    UCSR0B |= _BV(UDRIE0);
    sei();
}

void send_str(uint8_t str[])
{
    for (uint8_t* c = str; *c; c++) {
        send_byte(*c);
    }
}

ISR(USART_RX_vect)
{
    uint8_t ch = UDR0;
    rbuff[n_rbuff++] = ch;
    if (n_rbuff > 255)
        n_rbuff = 0;
}

ISR(USART_UDRE_vect)
{
    if (n_wbuff != s_wbuff) {
        UDR0 = wbuff[s_wbuff++];
    } else {
        // UDR intr off
        UCSR0B &= ~_BV(UDRIE0);
    }
}

int main(void)
{
    set_sleep_mode(SLEEP_MODE_IDLE);
    usart_init(9600);
    sei();
    while(1) {
        send_str("Hello!\r\n");
        _delay_ms(1000);
    }
    return 0;
}
