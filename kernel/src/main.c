#include <arch/types.h>
#include <arch/cpu.h>
#include <drivers/serial.h>

void kernel_main(void) {
    serial_write("Hello world\n");
    
    //main kernel loop
    while (1) {
        arch_halt();
    }
}