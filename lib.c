#include "lib.h"
#include "uart.h"
#include "gpio.h"

void init(void) {
	uart_init();
}

int putc(unsigned char c) {
	if (c == '\n')
		uart_send('\r');
	uart_send(c);
	return 0;
}

unsigned char getc(void) {
	unsigned char c = uart_getc();
	c = (c == '\r') ? '\n' : c;
	putc(c);
	return c;
}

int puts(unsigned char *str) {
	while (*str)
		putc(*(str++));
	return 0;
}

int gets(unsigned char *buf) {
	int i = 0;
	unsigned char c;
	do {
		c = getc();
		if (c == '\n')
			c = '\0';
		buf[i++] = c;
	} while (c);
	return i - 1;
}

int strcmp(const char *s1, const char *s2) {
	while (*s1 || *s2) {
		if (*s1 != *s2)
			return (*s1 > *s2) ? 1 : -1;
		s1++;
		s2++;
	}
	return 0;
}

int strncmp(const char *s1, const char *s2, int len) {
	while ((*s1 || *s2) && (len > 0)) {
		if (*s1 != *s2)
			return (*s1 > *s2) ? 1 : -1;
		s1++;
		s2++;
		len--;
	}
	return 0;
}

void rand_init(void) {
	*RNG_STATUS = 0x40000;
	*RNG_INT_MASK |= 1;
	*RNG_CTRL |= 1;
	while(!((*RNG_STATUS) >> 24)) asm volatile("nop");
}

unsigned int rand(unsigned int min, unsigned int max) {
	return *RNG_DATA % (max - min) + min;
}

void on(register unsigned int n) {
    register unsigned int r;
// ON
	*GPFSEL1 = 0x01 << 18;
	*GPSET0 = 0x01 << 16;

// UARTできるように切り替える
    r=*GPFSEL1;
    r&=~((7<<12)|(7<<15)); // gpio14, gpio15
    r|=(2<<12)|(2<<15);    // alt5
    *GPFSEL1 = r;
}

void off(register unsigned int n) {
    register unsigned int r;
// OFF
	*GPFSEL1 = 0x01 << 18;
	*GPCLR0 = 0x01 << 16;

// UARTできるように切り替える
    r=*GPFSEL1;
    r&=~((7<<12)|(7<<15)); // gpio14, gpio15
    r|=(2<<12)|(2<<15);    // alt5
    *GPFSEL1 = r;
}

void aa(void) {
	puts("  _____ _    _          ____   _____ \n");
	puts(" / ____| |  | |   /\   / __ \ / ____|\n");
	puts("| |    | |__| |  /  \ | |  | | (___  \n");
	puts("| |    |  __  | / /\ \| |  | |\___ \ \n");
	puts("| |____| |  | |/ ____ \ |__| |____) |\n");
	puts(" \_____|_|  |_/_/    \_\____/|_____/ \n\n");
}
