/* clockset.h
 *
 * function to improve internal clock calibration accuracy
 * Atmega88 can use external or or internal clock
 * internal clock is 8MHz with preset accuracy +/- 10%
 * accuracy can be set +/- 1% for changing OSCCAL byte
 * This code assumes that T1 input is connected to an accurate external
 * 4MHz clock 9e.g. from Zigbit module)
 *
 * This clock is used as calibration reference to impovr factory
 * accuracy.
 *
 * This allows accurate UART communication between AVR processor and
 * Zigbit module.
 *
 * PREREQUISITES
 * Clock Fuses must be set to use internal 8MHz oscillator as clock source
 * T1 input must be conected to 8MHz clock (JP1 2-3 connected on Z-ZVR-LCD boards)
 * CLKDIV8 fuse must be cleared for 8MHz clock not 1MHz
 * System clock speed will be 8Mhz
 *
 * lcd_initialise should be called before calibrate_freq()
 *
 * OPERATION
 *
 * This code allows AVR to be used with internal clock when Zigbit is switched off.
 *
 * This code will terminate setting clock prescaler 
 * to divide by 1 and cal byte as necessary for precise 8MHz
 *
 * Old & new cal byte values will be printed on LCD
 */

#include <inttypes.h>



/* set the clock prescaler to divide by 2^n */
#define SET_CLOCK_DIV(n) { \
	CLKPR = 0x80; \
	CLKPR = n; \
	} 
	
	
/* Used to set internal RC oscillator to precisely 8 MHz */
/* work out the best value for OSCCAL byte to sync a 8MHz clock with T1
 * used to allow 8MHz clock with Zigbit communication 
 * Note that 8MHz clock can be achieved more easily by setting JP1 1-2 and
 * using external 8MHz clock directly instead of internal clock
 * see discussion above
 *
 * On return the internal oscillator will be set to precisely 8MHz and prescaler to divide by 1
 * so that system clock is 8MHz
 */
void calibrate_internal_rc_osc(void);
