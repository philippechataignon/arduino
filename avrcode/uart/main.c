#include <avr/io.h>
#include <uart.h>

int main(void)
{
	char n=0;
	usart_init();
	while (1) {
		usart_send(n);
		n=n+1;
	}
	for (;;);
	return 0; //to remove compiler warning
}


