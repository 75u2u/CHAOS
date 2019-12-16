#include "uart.h"

void main()
{
    char c;

    // set up serial console
    uart_init();

    // say hello
    uart_puts("Hello World!\n");

    // echo everything back
    while(1) {
        uart_puts("> ");
        c = uart_getc();
	uart_send(c);
	//uart_puts(c);
        //uart_send(uart_getc());
    }
}
