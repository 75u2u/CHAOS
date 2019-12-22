#include "lib.h"
#include "power.h"
#include "gpio.h"
#include "delays.h"

int main() {
	int i;
	int j;
	char c;
	static char buf[64];

	init();
	rand_init();
	start();
    while(1) {
		puts(">");
		puts(" ");
		gets(buf);
		if(!strncmp(buf, "echo", 4)) {
			puts(buf + 5);
			puts("\n");
    	} else if(!strcmp(buf, "exit")) {
      		break;
		} else if(!strcmp(buf, "reboot")) {
			puts("rebooting...\n");
			reboot();
		} else if(!strcmp(buf, "shutdown")) {
			puts("Bye!\n");
			shutdown();
		} else if(!strcmp(buf, "rand")) {
			uart_hex(rand(0,4294967295));
			puts("\n");
		} else if(!strncmp(buf, "on", 2)) {
			puts("GPIO");
//			puts(buf + 3);
			puts(" ON!\n");
			on((unsigned int)(buf + 3));
		} else if(!strncmp(buf, "off", 3)) {
			puts("GPIO");
//			puts(buf + 4);
			puts(" OFF!\n");
			off((unsigned int)(buf + 4));
		} else if(!strcmp(buf, "led")) {
			for(i=0;i<=1000;i++) {
				on((unsigned int)16);
				for(j=0;j<=100000;j++){ puts(""); }
//				wait_msec(100000000);
				off((unsigned int)16);
				for(j=0;j<=100000;j++){ puts(""); }
//				wait_msec(100000000);
			}
		} else if(!strcmp(buf, "help")) {
				help();
    	} else {
			puts(buf);
			puts(" is ");
			puts("unknown command.\n");
    	}
    }
	puts("Bye!\n");
	return 0;
}
