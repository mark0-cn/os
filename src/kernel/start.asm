[bits 32]

global _start

_start:
    mov byte [0xb8000],'k'
    ;xchg bx,bx
    jmp $