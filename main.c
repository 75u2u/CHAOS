#include "lib.h"
#include "gpio.h"
#include "uart.h"

int main() {
    char buf[64];

    init();
    start();
    while(1) {
        puts(">");
        puts(" ");
        gets(buf);
        if(!strncmp(buf, "echo", 4)) {
			puts(buf + 5);
			puts("\n");
    	} else {
			puts(buf);
			puts(" is ");
			puts("unknown\n");
    	}
    }
    return 0;
}
