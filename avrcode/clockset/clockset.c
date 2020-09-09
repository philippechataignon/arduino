#include "clockset.h"
#include <util/delay.h>
#include <avr/io.h>
#include "lcd4.h"
#include <avr/pgmspace.h>

#if F_CPU != 8000000UL
# warning - F_CPU must be set to 8000000UL (8MHz) for clockset.c to work
# warning - Set frequency in AVR Studio->project->configuration options->general tab
#endif

static uint8_t try_opt(uint8_t cal, uint8_t delta);

static uint16_t get_freq(uint8_t cal);

void calibrate_internal_rc_osc(void)
{
   uint8_t cal, old_cal;


   _delay_ms(100); /*wait till all has stabilised */
   if (CLKPR == 3) {
      lcd_printf_P(PSTR("CLKDIV8 fuse on")); // this is a fatal error
      for (;;);
   }
   old_cal = OSCCAL;
   cal = OSCCAL; // factory cal byte for 8MHz clock

   SET_CLOCK_DIV(0); /* set clock to 8MHz */

   /* do gradient descent optimisation on factory setting to improve accuracy */
   if (try_opt(cal,2)) {
      while (try_opt(cal,2)) cal=cal+2;
   } else {
      while (try_opt(cal,-2)) cal=cal-2;
   }
   if (try_opt(cal,1)) cal=cal+1;
   if (try_opt(cal,-1)) cal = cal-1;
          
   OSCCAL = cal; //set best cal byte.
   /* frequency should now read 8MHz almost exactly */
   lcd_printf("Cal%d(%dk)->%d(%dk)", old_cal,get_freq(old_cal), cal, get_freq(cal));
}

static uint8_t try_opt(uint8_t cal, uint8_t delta)
// return non-zero if cal+delta is better calibration than cal
// return 0 if cal+delta is outside current cal range (0-0x7f or 0x80-0xff)
{
   uint8_t cal_new;
   cal_new = cal+delta;
   if ((cal_new & 0x80) != (cal & 0x80)) return 0 ; //overflow
   return (uint8_t)(get_freq(cal_new) < get_freq(cal));
}


static uint16_t get_freq(uint8_t cal)
{
   uint16_t x;
   uint8_t sreg;
   OSCCAL=cal;

   TCCR1B=0x6;   /* count external T1 input */
 
   _delay_us(20); /* wait for clock to stabilise */

   sreg = SREG;
   asm( "CLI");
   TCNT1 = 0;
   SREG = sreg;

   _delay_us(1000);

   sreg=SREG;
   asm ("CLI");
   x = TCNT1;
   SREG = sreg;

   return x;
}
