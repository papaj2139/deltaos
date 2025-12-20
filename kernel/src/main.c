#include <types.h>

#include <drivers/serial.h>

int main() {
    serial_init();
    serial_write("\x1b[2J\x1b[HHello, world!\n");
    return 0;
}