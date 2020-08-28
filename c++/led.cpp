#include <avr/io.h>
#include <util/delay.h>

constexpr double BLINK_DELAY_MS = 500.0;

int main (void){

  // set pin 0 of PORTB (8-13) for output (1).
  DDRB = 0x01;

  const auto toggle= [](volatile unsigned char &p, const int bit){
    p ^= (0x01 << bit);
  };

  while(true) {
    toggle(PORTB, 0),
    _delay_ms(BLINK_DELAY_MS);
  }
}

