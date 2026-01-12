CC = arm-none-eabi-gcc
CFLAGS = -c -mcpu=cortex-m3 -mthumb -std=gnu11

final : LED.elf

syscalls.o : syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

sysmem.o : sysmem.c
	$(CC) $(CFLAGS) $^ -o $@

main.o : main.c
	$(CC) $(CFLAGS) $^ -o $@

timer.o : timer.c
	$(CC) $(CFLAGS) $^ -o $@

uart.o : uart.c
	$(CC) $(CFLAGS) $^ -o $@

startup_stm32f103rbtx.o : startup_stm32f103rbtx.c
	$(CC) $(CFLAGS) $^ -o $@

LED.elf : syscalls.o sysmem.o main.o timer.o uart.o startup_stm32f103rbtx.o
	$(CC) -nostdlib -T stm32_ls.ld -Wl,-Map=led_make.map $^ -o $@ -lc -lgcc

load :
	openocd -f board/st_nucleo_f103rb.cfg
clean:
	del    -f *.o *.elf *.map