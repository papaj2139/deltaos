#ifndef ARCH_AMD64_PIC_H
#define ARCH_AMD64_PIC_H

#include <types.h>

void pic_send_eoi(uint8 irq);
void pic_remap(uint8 vector1, uint8 vector2);
void pic_disable(void);
void pic_set_mask(uint8 irqline);
void pic_clear_mask(uint8 irqline);

#endif