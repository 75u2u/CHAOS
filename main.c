#include "lib.h"
#include "power.h"
#include "gpio.h"
#include "delays.h"
#include "mbox.h"
#include "uart.h"

int main() {
	int i;
	int j;
	char buf[64];
	int count = 0;

	init();
	rand_init();
//	display_fill();
//	lfb_print(0, count, "Hello World!");
	count++;
//	lfb_print(0, 1, "Let me just step right in\n");
	start();
    while(1) {
		puts(">");
//		lfb_print(0, count, ">");
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
			on(*buf + 3);
		} else if(!strncmp(buf, "off", 3)) {
			puts("GPIO");
//			puts(buf + 4);
			puts(" OFF!\n");
			off(*buf + 4);
		} else if(!strcmp(buf, "led")) {
			for(i=0;i<=1000;i++) {
				on(16);
				for(j=0;j<=100000;j++){ puts(""); }
//				wait_msec(100000000);
				off(16);
				for(j=0;j<=100000;j++){ puts(""); }
//				wait_msec(100000000);
			}
		} else if(!strcmp(buf, "help")) {
				help();
    	} else {
			puts(buf);
			puts(" is ");
			puts("unknown\n");
			//puts("I'm afraid I can't do that.\n");
    	}
		count++;
    }
	puts("Bye!\n");
	return 0;
}
