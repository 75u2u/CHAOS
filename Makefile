SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -fno-stack-protector

all: clean kernel8.img

start.o: start.S
	/usr/local/cross-compiler/bin/aarch64-elf-gcc $(CFLAGS) -c start.S -o start.o

%.o: %.c
	/usr/local/cross-compiler/bin/aarch64-elf-gcc $(CFLAGS) -c $< -o $@

#font.o: font.psf
#	/usr/local/cross-compiler/bin/aarch64-elf-ld -r -b binary -o font.o font.psf

kernel8.img: start.o $(OBJS)
	/usr/local/cross-compiler/bin/aarch64-elf-ld -nostdlib -nostartfiles start.o $(OBJS) -T link.ld -o kernel8.elf
	/usr/local/cross-compiler/bin/aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

#kernel8.img: start.o font.o $(OBJS)
#	/usr/local/cross-compiler/bin/aarch64-elf-ld -nostdlib -nostartfiles start.o font.o $(OBJS) -T link.ld -o kernel8.elf
#	/usr/local/cross-compiler/bin/aarch64-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm kernel8.elf *.o >/dev/null 2>/dev/null || true

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio

copy:
	cp kernel8.img /media/tsuru/SD/.
	cp bootcode.bin /media/tsuru/SD/.
	cp start.elf /media/tsuru/SD/.
	cp config.txt /media/tsuru/SD/.

com:
	minicom -b 115200 -D /dev/ttyUSB0

eject:
	umount /media/tsuru/SD

push:
	git add .
	git commit -m "update"
	git push origin master

