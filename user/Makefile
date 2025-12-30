.PHONY: all clean


CC := gcc
LD := ld

CFLAGS := -ffreestanding -fno-builtin -fno-stack-protector -m64 -c -O2
LDFLAGS := -T link.ld

all: init.bin

init.o: init.c
	$(CC) $(CFLAGS) -o $@ $<

init.bin: init.o
	$(LD) $(LDFLAGS) -o $@ $<

clean:
	rm -f *.o *.bin
