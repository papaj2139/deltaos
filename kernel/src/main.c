#include <types.h>

#include <drivers/serial.h>
#include <int/idt.h>
#include <int/pit.h>

int main() {
    serial_init();
    serial_write("\x1b[2J\x1b[HHello, world!\n");

    pit_setfreq(1000);
    idt_init();
    serial_write("IDT initialized triggering int $3 t test\n");
    __asm__ volatile ("int $32");
    serial_write("returned from interrupt\n");
    
    return 0;
}