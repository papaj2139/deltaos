#include <arch/types.h>
#include <arch/io.h>

#define COM1_PORT 0x3F8
#define COM1_MMIO 0  //would be set to MMIO base address on arch's with MMIO

void serial_init(void) {
    if (ARCH_HAS_PORT_IO) {
        outb(COM1_PORT + 1, 0x00);    // Disable interrupts
        outb(COM1_PORT + 3, 0x80);    // Enable DLAB
        outb(COM1_PORT + 0, 0x03);    // Baud divisor low byte (38400 baud)
        outb(COM1_PORT + 1, 0x00);    // Baud divisor high byte
        outb(COM1_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
        outb(COM1_PORT + 2, 0xC7);    // Enable FIFO, clear them, 14-byte threshold
        outb(COM1_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    } else {
        //we don't support any MMIO archs yert but in thefuture
    }
}

static uint8 serial_is_transmit_empty(void) {
    if (ARCH_HAS_PORT_IO) {
        return inb(COM1_PORT + 5) & 0x20;
    } else {
        return arch_mmio_read8(COM1_MMIO + 5) & 0x20;
    }
}

void serial_write_char(char c) {
    while (!serial_is_transmit_empty());
    if (ARCH_HAS_PORT_IO) {
        outb(COM1_PORT, c);
    } else {
        arch_mmio_write8(COM1_MMIO, c);
    }
}

void serial_write(const char* s) {
    while (*s) {
        if (*s == '\n')
            serial_write_char('\r');
        serial_write_char(*s++);
    }
}

void serial_write_hex(uint64 n) {
    char const* hex = "0123456789ABCDEF";
    serial_write("0x");
    for (int i = 60; i >= 0; i -= 4) {
        serial_write_char(hex[(n >> i) & 0xF]);
    }
}
