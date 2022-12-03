[org 0x1000]

dw 0x1122 ;magic num 用于检查是否加载正确
mov si,loading
call print

detect_memory:
    xor ebx,ebx

    ; es:di 结构体到缓存位置
    mov ax,0
    mov es,ax
    mov edi,ards_buffer
    mov edx,0x534d4150 ;固定签名
.next:
    ;子功能号
    mov eax,0xe820
    ;ards 结构的大小
    mov ecx,20
    ;调用0x15系统调用
    int 0x15

    ;如果CF置位，表示出错
    jc error

    ;将缓存指针指向下一个结构体
    add di,cx

    ;将结构体数量加一
    inc word [ards_count]

    cmp ebx,0
    jnz .next

    mov si,detecting
    call print
    
    jmp prepare_protected_mode

prepare_protected_mode:
    cli ;关闭中断

    ;打开A20线
    in al,0x92
    or al,0b10
    out 0x92,al

    lgdt [gdt_ptr] ;加载gdt

    ;启动保护模式
    mov eax,cr0
    or eax,1
    mov cr0,eax

    ;用跳转来刷新缓存，启用保护模式
    jmp dword code_selector:protect_mode

print:
    mov ah,0x0e
    .next:
        mov al,[si]
        cmp al,0
        jz .end
        int 0x10
        inc si
        jmp .next
    .end:
        ret

detecting:
    db "detecting memory ...",10,13,0

loading:
    db "loading ...",10,13,0 


error:
    mov si,.msg
    call print
    hlt
    jmp $
    .msg:
        db "loading error !!!",10,13,0

[bits 32]
protect_mode:
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax; 初始化段寄存器
    xchg bx,bx
    mov esp, 0x10000; 修改栈顶

    xchg bx,bx
    jmp $
    

code_selector equ (1 << 3)
data_selector equ (2 << 3)

memory_base equ 0 ;内存开始的位置，基地址
memory_limit equ (1024*1024*1024*4)/(4*1024)-1 ;内存界限

gdt_ptr:
    dw (gdt_end - gdt_base) - 1
    dd gdt_base
gdt_base:
    dd 0, 0; NULL 描述符
gdt_code:
    dw memory_limit & 0xffff; 段界限 0 ~ 15 位
    dw memory_base & 0xffff; 基地址 0 ~ 15 位
    db (memory_base >> 16) & 0xff; 基地址 16 ~ 23 位
    ; 存在 - dlp 0 - S _ 代码 - 非依从 - 可读 - 没有被访问过
    db 0b_1_00_1_1_0_1_0;
    ; 4k - 32 位 - 不是 64 位 - 段界限 16 ~ 19
    db 0b1_1_0_0_0000 | (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff; 基地址 24 ~ 31 位
gdt_data:
    dw memory_limit & 0xffff; 段界限 0 ~ 15 位
    dw memory_base & 0xffff; 基地址 0 ~ 15 位
    db (memory_base >> 16) & 0xff; 基地址 16 ~ 23 位
    ; 存在 - dlp 0 - S _ 数据 - 向上 - 可写 - 没有被访问过
    db 0b_1_00_1_0_0_1_0;
    ; 4k - 32 位 - 不是 64 位 - 段界限 16 ~ 19
    db 0b1_1_0_0_0000 | (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff; 基地址 24 ~ 31 位
gdt_end:

ards_count:
    dd 0

ards_buffer: