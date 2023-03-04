#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <os/types.h>

#define IDT_SIZE 256

typedef struct gate_t
{
    u16 offset0;
    u16 selector;
    u8 reserved;
    u8 type : 4;
    u8 segment : 1;
    u8 DPL : 2;
    u8 present : 1;
    u16 offset1;
}_packed gate_t;

void interrupt_init();

#endif //INTERRUPT_H