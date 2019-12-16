#include "gpio.h"

// mini UART レジスタ
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

// GPIO, ボーレート設定(115200 8N1)
void uart_init() {
    register unsigned int r;

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
    *GPPUD = 0;            // pins 14 and 15
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = 0;        // GPIO setup 書き込み
    *AUX_MU_CNTL = 3;      // Tx, Rx 許可
}

// 文字送信
void uart_send(unsigned int c) {
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20));
    *AUX_MU_IO=c;
}

// 文字読み取り
char uart_getc() {
    char r;
    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01));
    r=(char)(*AUX_MU_IO);
    return r=='\r'?'\n':r;
}

// 文字列表示
void uart_puts(char *s) {
    while(*s) {
        if(*s=='\n')
            uart_send('\r');
        uart_send(*s++);
    }
}
