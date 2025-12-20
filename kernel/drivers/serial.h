#ifndef DRIVERS_SERIAL_H
#define DRIVERS_SERIAL_H

#include <arch/types.h>

void serial_init(void);
uint8 serial_is_transmit_empty(void);
void serial_write_char(char c);
void serial_write(const char* s);
void serial_write_hex(uint64 n);

#endif
