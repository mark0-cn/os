#include <os/interrupt.h>
#include <os/global.h>
#include <os/printk.h>

gate_t idt[IDT_SIZE];
pointer_t idt_ptr;

#define ENTRY_SIZE 0x30

handler_t handler_table[IDT_SIZE];
extern void interrupt_handler();
extern handler_t handler_entry_table[ENTRY_SIZE];

static char *messages[] = {
    "#DE Divide Error\0",
    "#DB RESERVED\0",
    "--  NMI Interrupt\0",
    "#BP Breakpoint\0",
    "#OF Overflow\0",
    "#BR BOUND Range Exceeded\0",
    "#UD Invalid Opcode (Undefined Opcode)\0",
    "#NM Device Not Available (No Math Coprocessor)\0",
    "#DF Double Fault\0",
    "    Coprocessor Segment Overrun (reserved)\0",
    "#TS Invalid TSS\0",
    "#NP Segment Not Present\0",
    "#SS Stack-Segment Fault\0",
    "#GP General Protection\0",
    "#PF Page Fault\0",
    "--  (Intel reserved. Do not use.)\0",
    "#MF x87 FPU Floating-Point Error (Math Fault)\0",
    "#AC Alignment Check\0",
    "#MC Machine Check\0",
    "#XF SIMD Floating-Point Exception\0",
    "#VE Virtualization Exception\0",
    "#CP Control Protection Exception\0",
};

void exception_handler(int vector)
{
    char* message = NULL;
    if(vector < 22)
    {
        message = messages[vector];
    }else
    {
        message = messages[15];
    }
    printk("Exception : [0x%02x] %s \n", vector, messages[vector]);
    while (true);    
}

void interrupt_init()
{
    for(size_t i = 0; i < ENTRY_SIZE; i++)
    {
        gate_t* gate = &idt[i];
        handler_t handler = handler_entry_table[i];

        gate->offset0 = (u32)handler & 0xffff;
        gate->offset1 = ((u32)handler>>16) & 0xffff;
        gate->selector = 1<<3;
        gate->reserved = 0;
        gate->type = 0b1110;
        gate->segment = 0;
        gate->DPL = 0;
        gate->present = 1;
    }

    for (size_t i = 0; i < 0x20; i++)
    {
        handler_table[i] = exception_handler;
    }

    idt_ptr.base = (u32)idt;
    idt_ptr.limit = sizeof(idt) - 1;
    asm volatile("lidt idt_ptr\n");
}
