#include <arch/types.h>
#include <arch/cpu.h>
#include <drivers/serial.h>
#include <drivers/fb.h>
#include <drivers/console.h>
#include <drivers/keyboard.h>
#include <kernel/device.h>
#include <lib/string.h>

void kernel_main(void) {
    serial_write("kernel_main started\n");
    
    //initialize framebuffer
    fb_init();
    keyboard_init();
    
    if (fb_available()) {
        //initialize console
        con_init();
        con_clear();
        
        con_set_fg(FB_RGB(0, 255, 128));
        con_print("DeltaOS Kernel\n");
        con_print("==============\n\n");
        
        con_set_fg(FB_WHITE);
        con_print("Console: initialized\n");
        con_print("Timer: running @ 100Hz\n");
        
        //demonstrate device manager works
        struct device *con = device_find("console");
        if (con && con->ops->write) {
            con->ops->write(con, "Device manager: working!\n", 25);
        }
    }
    
    //main kernel loop
    for (;;) {
        con_print("[]$ ");

        char buffer[128] = {0};
        uint8 i = 0;
        while (1) {
            // optimisation: waits for keyboard interrupt before re-looping
            keyboard_wait();

            char c;
            if (!get_key(&c)) continue;
            if (c == '\b') {
                if (i > 0) {
                    i--;
                    con_putc('\b');
                }
            } else {
                if (i >= sizeof(buffer) - 1) {
                    buffer[i] = 0;
                    break;
                }

                con_putc(c);
                buffer[i++] = c;
                if (c == '\n') {
                    buffer[i - 1] = '\0';
                    break;
                }
            }
        }

        if (strcmp(strtok(buffer, " "), "help") == 0) {
            con_print("HELP MEEEE\n");
        } else if (buffer[0] != '\0') {
            con_print(buffer);
            con_print(": command not found\n");
        }
    }
}