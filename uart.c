#include "gpio.h"
#include "mbox.h"

// mini UART register
#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

// PL011 UART register
#define UART0_DR        ((volatile unsigned int*)(MMIO_BASE+0x00201000))
#define UART0_FR        ((volatile unsigned int*)(MMIO_BASE+0x00201018))
#define UART0_IBRD      ((volatile unsigned int*)(MMIO_BASE+0x00201024))
#define UART0_FBRD      ((volatile unsigned int*)(MMIO_BASE+0x00201028))
#define UART0_LCRH      ((volatile unsigned int*)(MMIO_BASE+0x0020102C))
#define UART0_CR        ((volatile unsigned int*)(MMIO_BASE+0x00201030))
#define UART0_IMSC      ((volatile unsigned int*)(MMIO_BASE+0x00201038))
#define UART0_ICR       ((volatile unsigned int*)(MMIO_BASE+0x00201044))

// GPIO (115200 8N1)
void uart_init() {
    register unsigned int r;

    *UART0_CR = 0;

    // クロック設定
    mbox[0] = 9*4;
    mbox[1] = MBOX_REQUEST;
    mbox[2] = MBOX_TAG_SETCLKRATE; // クロックレート
    mbox[3] = 12;
    mbox[4] = 8;
    mbox[5] = 2;           // UART クロック
    mbox[6] = 4000000;     // 4Mhz
    mbox[7] = 0;           // turbo クリア
    mbox[8] = MBOX_TAG_LAST;
    mbox_call(MBOX_CH_PROP);

    // GPIO初期化
    *AUX_ENABLE |=1;       // UART1, AUX mini uart 許可
    *AUX_MU_CNTL = 0;
    *AUX_MU_LCR = 3;       // 8 bit
    *AUX_MU_MCR = 0;
    *AUX_MU_IER = 0;
    *AUX_MU_IIR = 0xc6;    // 割り込み 不可
    *AUX_MU_BAUD = 270;    // ボーレート 115200

    // GPIO設定
    r=*GPFSEL1;
    r&=~((7<<12)|(7<<15)); // gpio14, gpio15
    r|=(2<<12)|(2<<15);    // alt5
    *GPFSEL1 = r;
    *GPPUD = 0;            // pin14,15
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = 0;        // GPIO 初期化
    *AUX_MU_CNTL = 3;      // Tx, Rx 許可

    *UART0_ICR = 0x7FF;    // 割り込み クリア
    *UART0_IBRD = 2;       // 115200 baud
    *UART0_FBRD = 0xB;
    *UART0_LCRH = 0b11<<5; // 8n1
    *UART0_CR = 0x301;     // enable Tx, Rx, FIFO
}

void uart_send(unsigned int c) {
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20));
    *AUX_MU_IO=c;
}

char uart_getc() {
    char r;
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01));
    r=(char)(*AUX_MU_IO);
    return r=='\r'?'\n':r;
}

void uart_puts(char *s) {
    while(*s) {
        if(*s=='\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

void uart_hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        // 4つ組
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        uart_send(n);
    }
}

