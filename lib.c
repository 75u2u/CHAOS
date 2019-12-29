#include "lib.h"
#include "uart.h"
#include "gpio.h"
#include "mbox.h"

void init(void) {
	uart_init();
}

int putc(unsigned char c) {
	if (c == '\n')
		uart_send('\r');
	if (c != "")
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

void on(unsigned int n) {
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

void off(unsigned int n) {
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

void in() {

}

void start(void) {
	puts("  _|_|_|  _|    _|    _|_|      _|_|      _|_|_|  \n");
	puts("_|        _|    _|  _|    _|  _|    _|  _|        \n");
	puts("_|        _|_|_|_|  _|_|_|_|  _|    _|    _|_|    \n");
	puts("_|        _|    _|  _|    _|  _|    _|        _|  \n");
	puts("  _|_|_|  _|    _|  _|    _|    _|_|    _|_|_|    \n");
	puts("\n");
}

void help(void) {
	puts("help");
	puts("\n");
	puts("■■■■■■■  ■ ■  ■   ■■■■■■■\n");
	puts("■     ■  ■    ■ ■ ■     ■\n");
	puts("■ ■■■ ■  ■■■  ■ ■ ■ ■■■ ■\n");
	puts("■ ■■■ ■ ■ ■■■■■■■ ■ ■■■ ■\n");
	puts("■ ■■■ ■ ■    ■  ■ ■ ■■■ ■\n");
	puts("■     ■  ■■  ■■ ■ ■     ■\n");
	puts("■■■■■■■ ■ ■ ■ ■ ■ ■■■■■■■\n");
	puts("         ■  ■ ■■         \n");
	puts("■■   ■■■ ■■ ■■ ■    ■■   \n");
	puts("■■ ■■     ■    ■   ■■■■■ \n");
	puts("    ■ ■  ■  ■■■■   ■■■ ■■\n");
	puts(" ■ ■■■    ■ ■ ■■■■■■ ■  ■\n");
	puts(" ■    ■■ ■■ ■ ■ ■ ■■    ■\n");
	puts("     ■ ■■ ■   ■■ ■ ■   ■ \n");
	puts(" ■ ■ ■■■■ ■   ■■ ■■■■■ ■■\n");
	puts(" ■  ■   ■■ ■■  ■■ ■■ ■■ ■\n");
	puts(" ■   ■■■■■■ ■■■■■■■■■ ■  \n");
	puts("        ■ ■   ■ ■   ■    \n");
	puts("■■■■■■■ ■  ■ ■  ■ ■ ■   ■\n");
	puts("■     ■ ■ ■■  ■■■   ■    \n");
	puts("■ ■■■ ■   ■■■■ ■■■■■■ ■■ \n");
	puts("■ ■■■ ■  ■■■■  ■■■■    ■■\n");
	puts("■ ■■■ ■      ■       ■■ ■\n");
	puts("■     ■ ■■     ■■  ■■   ■\n");
	puts("■■■■■■■ ■■ ■    ■■■  ■  ■\n");
}

void display_init(void) {
}

void display_fill(void) {
	fb_info_t fb_info = {640, 400, 640, 400, 0, 16, 0, 0, 0, 0};
	lfb_init(&fb_info);

	unsigned char *vram  = fb_info.buf_addr;
	unsigned int  pitch  = fb_info.row_bytes;
	unsigned int  bpp    = fb_info.bpp;
	unsigned int  x = fb_info.display_w;
	unsigned int  y = fb_info.display_h;

	boxfill8(vram, COL8_008484, pitch, bpp,  0,     0,      x -  1, y - 29);
	boxfill8(vram, COL8_C6C6C6, pitch, bpp,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, COL8_FFFFFF, pitch, bpp,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, COL8_C6C6C6, pitch, bpp,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, COL8_FFFFFF, pitch, bpp,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, COL8_FFFFFF, pitch, bpp,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, COL8_848484, pitch, bpp,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, COL8_848484, pitch, bpp, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, COL8_000000, pitch, bpp,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, COL8_000000, pitch, bpp, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, COL8_848484, pitch, bpp, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, COL8_848484, pitch, bpp, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, COL8_FFFFFF, pitch, bpp, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, COL8_FFFFFF, pitch, bpp, x -  3, y - 24, x -  3, y -  3);
}
