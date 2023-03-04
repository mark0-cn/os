#include <os/global.h>
#include <os/string.h>

descriptor_t gdt[GDT_SIZE];
pointer_t gdt_ptr;

void gdt_init()
{
    asm volatile("sgdt gdt_ptr");

    memcpy(&gdt, (void*)gdt_ptr.base, gdt_ptr.limit+1);

    gdt_ptr.base = (u32)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    asm volatile("lgdt gdt_ptr");
    
}

