/* TJWC March 2009 
 *
 * Sample code for Atmega88 etc
 *
 * Configure USART0 as a serial port
 * Baud rate 38400 (defined by BAUD constant in uart.h)
 * 8 data bits, 1 stop bit, no parity
 * Interrupts on Tx & Rx
 * CTS (active low) input on PD2 provides optional transmit flow control
 * Tx characters are buffered for Tx by interrupt
 * Rx characters are processed during interrupt.
 *
 *------------------------------------------------------------------
 * INTERFACE
 *
 *  void process_rx_char(char) -- function is external to this package
 *                             -- defines Rx functionality. Called from Rx interrupt
 *                             -- see rxproc for normal use
 *
 *  void usart_send(char x) -- queues x to be sent via interrupt as soon as possible
 *
 *  usart_init() -- initialises hardware & data structures
 *
 *  usart_txq_length -- no of chars queued for future transmission
 *
 *  usart_printf, usart_printf_P - version of printf to UART
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include <macros.h>
#include <stdio.h>
#include <stdarg.h>
#include "uart.h"

#ifdef RECORD_ERRORS

int volatile framing_errors;

int volatile overrun_errors;

int volatile should_never_happen_errors;

int volatile full_tx_buffer_errors;

#endif






#define Q_INCR(x) x = x+1; if (x == TX_BUFF_SIZE) x = 0;

#define UDRIE_on() UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0) | BIT(UDRIE0)

#define UDRIE_off() UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0)

#if HARDWAREFLOWCONTROL == 0
#define NCTS (PORTB & 0x04) /*0 to enable send */
#else
#define NCTS 0
#endif


/* data structures for queue
 * it is VERY IMPORTANT that global variables which may be changed by
 * interrupt code are tagged volatile, as here.
 * Otherwise compiler optimisations may lead to incorect code
 */
struct q_t {
unsigned char volatile head_pos;
unsigned char volatile empty_pos;
unsigned char volatile length;
char volatile buffer[TX_BUFF_SIZE];
};

static struct q_t txq;



//initialise q structures
static void usart_q_init(void)
{
	txq.head_pos=0;
	txq.empty_pos=0;
	txq.length=0;
}

static void uart_set_baudrate(void)
{
#include <util/setbaud.h>
   UBRR0H = UBRRH_VALUE;
   UBRR0L = UBRRL_VALUE;
#if USE_2X
   UCSR0A |= (1 << U2X0);
#else
   UCSR0A &= ~(1 << U2X0);
#endif
}



//USART initialize
// desired baud rate: 38400
// actual: baud rate:38462 (0.2%)
void usart_init(void)
{
 UCSR0B = 0x00; //disable (and clear buffer) while setting baud rate

 UCSR0C = 0x06; //set 8 bit, no parity, 1 stop bit, asynch

 uart_set_baudrate(); //set the baudrate to BAUD


 UCSR0B = _BV(RXCIE0) | _BV(RXEN0) | _BV(TXEN0); // enable

 EIMSK = 0x01; //enable ext interrupt 0
 EICRA = 0x01; // falling & rising edge
 usart_q_init();
 sei();
}
 
//send char ch using interrupts
//on exit interrupts will be enabled 
//regardless of state on entry
void usart_send(char ch)
{
	unsigned char len;
	cli();//disable interrupts
	//otherwise check queue full
	while ((len=txq.length) == TX_BUFF_SIZE); // block while buffer is full
	//add ch to buffer
	txq.buffer[txq.empty_pos] = ch;
	Q_INCR(txq.empty_pos);
	txq.length = len+1;
	if (!NCTS) UDRIE_on(); //switch INT on	
	sei();//enable interrupts
}


//USART RX interrupt handler

ISR(USART_RX_vect)
{
  uint8_t sr;
 	//uart has received a character in UDR
	// process the received char right away
  sr = UCSR0A;
#ifdef RECORD_ERRORS
  if ((sr & _BV(RXC0)) == 0) should_never_happen_errors++;
  if (sr & _BV(FE0)) framing_errors++;
  if (sr & _BV(DOR0)) overrun_errors++;
#endif
  process_rx_char(UDR0);
}

//USART TX interrupt handler
ISR(USART_UDRE_vect)
{
    //first check flow control
    if (NCTS) {
  	  UDRIE_off();
	
	} else {


 	//character transferred to shift register 
 	//so UDR is now empty
	//try to get char from tx buffer
	if (txq.length != 0) {
		//get char and send it
		UDR0 = txq.buffer[txq.head_pos];
		Q_INCR(txq.head_pos);
		txq.length = txq.length-1;
	} else {
		UDRIE_off(); // switch off interrupt since no data
	}
	}
}


// INT0 Handler
ISR(INT0_vect)
{
  if (NCTS == 0) {
 	UDRIE_on();
	}
}


static int uart_putchar(char c, FILE *stream);


static FILE uart_out = FDEV_SETUP_STREAM(uart_putchar, NULL,
                                             _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	  usart_send(c);
	  return 0;
}

int usart_printf_P( const char *fmt, ...) 
{
   va_list al;
   va_start(al, fmt);
   return vfprintf_P(&uart_out, fmt, al);
}

int usart_printf( const char *fmt, ...) 
{
   va_list al;
   va_start(al, fmt);
   return vfprintf(&uart_out, fmt, al);
}

uint8_t usart_txq_length(void)
{
  return txq.length;
}



