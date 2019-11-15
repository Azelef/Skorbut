#The folder where you put your cross compiler
BIN=$(HOME)/opt/cross/bin

CC=$(BIN)/arm-none-eabi-gcc
FLAGS=-mcpu=arm1176jzf-s -fpic -ffreestanding -std=gnu99 -c -O2 -Wall -Wextra
ASMFLAGS=-mcpu=arm1176jzf-s -fpic -ffreestanding -c 

comp:
	mkdir -p build
	$(CC) $(ASMFLAGS) boot.s -o build/boot.o

	$(CC) $(FLAGS) src/main.c -o build/main.o
	$(CC) $(FLAGS) src/mailbox.c -o build/mailbox.o
	$(CC) $(FLAGS) src/util.c -o build/util.o
	$(CC) $(FLAGS) src/screen.c -o build/screen.o
	$(CC) $(FLAGS) src/shell.c -o build/shell.o
	$(CC) $(FLAGS) src/vitamin.c -o build/vitamin.o
	$(CC) $(FLAGS) src/string.c -o build/string.o
	$(CC) $(FLAGS) src/atags.c -o build/atags.o
	$(CC) $(FLAGS) src/mem.c -o build/mem.o
	$(CC) $(FLAGS) src/file.c -o build/file.o
	
	$(CC) -T linker.ld -o skorbut.elf -ffreestanding -O2 -nostdlib build/*.o
	#$(BIN)/arm-none-eabi-objcopy skorbut.elf -O binary kernel7.img

run:
	qemu-system-arm -m 256 -M raspi2 -serial stdio -kernel skorbut.elf

clean:
	rm -rf build *.elf *.o *.img

test:
	$(MAKE) comp
	$(MAKE) run
