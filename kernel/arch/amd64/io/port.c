#include <arch/amd64/io.h>

uint8 inb(uint16 port) {
    uint8 result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(uint16 port, uint8 value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}