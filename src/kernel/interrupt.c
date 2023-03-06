#include <os/interrupt.h>
#include <os/global.h>
#include <os/printk.h>

gate_t idt[IDT_SIZE];
pointer_t idt_ptr;

extern void interrupt_handler();


void interrupt_init()
{
    for(size_t i = 0; i < IDT_SIZE; i++)
    {
        gate_t* gate = &idt[i];
        gate->offset0 = (u32)interrupt_handler & 0xffff;
        gate->offset1 = ((u32)interrupt_handler>>16) & 0xffff;
        gate->selector = 1<<3;
        gate->reserved = 0;
        gate->type = 0b1110;
        gate->segment = 0;
        gate->DPL = 0;
        gate->present = 1;
    }
    idt_ptr.base = (u32)idt;
    idt_ptr.limit = sizeof(idt) - 1;
    asm volatile("lidt idt_ptr\n");
}
