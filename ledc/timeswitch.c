// button on PD7 = pin 7
// light PB5 = pin 13 during 2000ms
// use PCINT2 and TIMER1

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "binary.h"

#define T1_MAX 0xFFFFUL
#define T1_PRESCALER 1024
#define T1_TICK_US (T1_PRESCALER/(F_CPU/1000000UL)) /* 64us @ 16MHz */
#define T1_MAX_US (T1_TICK_US * T1_MAX) /* ~4.2s @ 16MHz */

#define set_bit(VAR, BIT) (VAR |= _BV(BIT))
#define clr_bit(VAR, BIT) (VAR &= ~_BV(BIT))

static void led_on(void)
{
    set_bit(PORTB, PORTB5);
}

static void led_off(void)
{
    clr_bit(PORTB, PORTB5);
}

static void led_init(void)
{
    set_bit(DDRB, DDB5);        /* PORTB5 as output */
    led_off();
}

static void timer_stop(void)
{
    TCCR1B = B00000000;         /* stop timer clock */
    clr_bit(TIMSK1, TOIE1);     /* disable interrupt */
    set_bit(TIFR1, TOV1);       /* clear interrupt flag */
}

static void timer_init(void)
{
    /* normal mode */
    clr_bit(TCCR1A, WGM10);
    clr_bit(TCCR1A, WGM11);
    clr_bit(TCCR1B, WGM12);
    clr_bit(TCCR1B, WGM13);
    timer_stop();
}

static void timer_start(unsigned long us)
{
    unsigned long ticks_long;
    unsigned short ticks;

    ticks_long = us / T1_TICK_US;
    if (ticks_long >= T1_MAX) {
        ticks = T1_MAX;
    } else {
        ticks = ticks_long;
    }
    TCNT1 = T1_MAX - ticks;     /* overflow in ticks*1024 clock cycles */
    TIMSK1 = B00000001;         /* set the Timer Overflow Interrupt Enable bit */
    TCCR1B = B00000101;         /* prescaler: 1024 */
}

static void timer_start_ms(unsigned short ms)
{
    timer_start(ms * 1000UL);
}

ISR(TIMER1_OVF_vect)
{                               /* timer 1 interrupt service routine */
    timer_stop();
    led_off();                  /* timeout expired: turn off LED */
}

ISR(PCINT2_vect)
{                               /* pin change interrupt service routine */
    led_on();
    timer_stop();
    if (bit_is_set(PIND, PIND7)) {  /* button released */
        timer_start_ms(2000);   /* timeout to turn off LED */
    }
}

static void button_init(void)
{
    clr_bit(DDRD, DDD7);        /* PORTD7 as input */
    set_bit(PORTD, PORTD7);     /* enable pull-up */
    set_bit(PCICR, PCIE2);      /* enable Pin Change 2 interrupt */
    set_bit(PCMSK2, PCINT23);   /* PORTD7 is also PCINT23 */
}

int main(void)
{
    led_init();
    button_init();
    timer_init();
    sei();                      /* enable interrupts globally */
    for (;;) {
        sleep_mode();
    }
}
