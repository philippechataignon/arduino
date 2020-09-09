/*********************************************************
 * lcd4-hardware-config.h  				 *
 *						         *
 * LCD hardware configuration                            *
 * Make changes here when porting to different hardware  *
 * values below are compatible with EEE boards           *
 *********************************************************/

 /* Tom Clarke 2009 */

/* AVR port and pins connected to HCT164 and/or LCD */

/* correct pins and clock freq for z_avr_lcd boards */
//#define F_CPU 8000000UL
#define LCD_PORT                PORTC
#define LCD_DDR			DDRC
#define LCD_ENABLE_PIN          0
#define LCD_RSDS_PIN            1
#define LCD_CLOCK_PIN           2

/* correct pins and clock freq for avr88_lcd boards */
//#define F_CPU 8000000UL
//#define LCD_PORT                PORTD
//#define LCD_DDR		  DDRD
//#define LCD_ENABLE_PIN          4
//#define LCD_RSDS_PIN            5
//#define LCD_CLOCK_PIN           6



/* for hardware initialisation */
#define LCD_PINS (_BV(LCD_ENABLE_PIN) | _BV(LCD_RSDS_PIN) | _BV(LCD_CLOCK_PIN) )



/* the following values depend on the LCD module that is being driven */

/* number of columns & rows on the display */
#define LCD_COLS 16 /* used by application, not driver code */
#define LCD_ROWS 4  /* must be set correctly to 1,2 or 4 in order for '\n' to work OK */
#define LCD_LINE_34_OFFSET LCD_COLS /* may need to be different for some displays */







