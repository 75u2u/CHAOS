#include "lib.h"
#include "power.h"
#include "gpio.h"

int main() {
	char c;
	static char buf[64];

	init();
	rand_init();
	puts("\nCHAOS started\n");

    while(1) {
		puts("> ");
		gets(buf);
		if(!strncmp(buf, "echo", 4)) {
			puts(buf + 5);
			puts("\n");
    	} else if(!strcmp(buf, "exit")) {
      		break;
		} else if(!strcmp(buf, "reboot")) {
			puts("rebooting CHAOS\n");
			reboot();
		} else if(!strcmp(buf, "shutdown")) {
			puts("shutdown CHAOS\n");
			shutdown();
		} else if(!strcmp(buf, "rand")) {
			uart_hex(rand(0,4294967295));
			puts("\n");
		} else if(!strncmp(buf, "on", 2)) {
			puts("GPIO ");
			puts((buf + 3));
			puts(" ON!\n");
			on((volatile unsigned int)(buf + 3));
		} else if(!strncmp(buf, "off", 3)) {
			puts("GPIO ");
			puts(buf + 4);
			puts(" OFF!\n");
			off((volatile unsigned int)(buf + 4));
    	} else {
			puts(buf);
			puts(" is ");
			puts("unknown command.\n");
    	}
    }
	puts("Bye!\n");
	return 0;
}
