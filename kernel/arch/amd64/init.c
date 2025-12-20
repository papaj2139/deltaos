#include <arch/amd64/types.h>
#include <arch/amd64/interrupts.h>
#include <arch/amd64/timer.h>
#include <drivers/serial.h>

extern void kernel_main(void);

void arch_init(void) {
    //early console for debugging
    serial_init();
    serial_write("\x1b[2J\x1b[H");
    serial_write("[amd64] initializing...\n");
    
    //set up interrupt infrastructure
    arch_interrupts_init();
    serial_write("[amd64] interrupts initialized\n");
    
    //enable interrupts
    arch_interrupts_enable();
    serial_write("[amd64] interrupts enabled\n");
    
    //initialize timer at 100Hz
    arch_timer_init(100);
    serial_write("[amd64] timer initialized @ 100Hz\n");
    
    //jump to MI kernel
    serial_write("[amd64] jumping to kernel_main\n\n");
    kernel_main();
}
