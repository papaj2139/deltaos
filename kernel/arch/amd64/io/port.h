#ifndef _IO_PORT_H
#define _IO_PORT_H

#include <types.h>

uint8 inb(uint16 port);
void outb(uint16 port, uint8 value);

#endif