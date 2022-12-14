;加载到指定内存空间
[org 0x7c00]

;清空终端
mov ax,3
int 0x10

;xchg bx,bx

;初始化寄存器
mov ax,0
mov ds,ax
mov es,ax
mov ss,ax
mov sp,0x7c00

;向终端输出
mov si,booting
call print

;将硬盘读到内存
mov edi,0x1000 ;读取到目标内存
mov ecx,2 ;起始扇区
mov bl,4 ;扇区数量
call read_disk 

cmp word [0x1000],0x1122 ;检查是否正确读入loader
jnz error
jmp 0x1002 ;跳转到loader

read_disk:
    mov dx,0x1f2
    mov al,bl
    out dx,al

    inc dx ;0x1f3
    mov al,cl ;起始扇区的前八位
    out dx,al

    inc dx ;0x1f4
    shr ecx,8
    mov al,cl ;起始扇区的中八位
    out dx,al

    inc dx ;0x1f5
    shr ecx,8
    mov al,cl ;起始扇区的高八位
    out dx,al

    inc dx ;0x1f6
    shr ecx,8
    and cl,0b1111 ;将高四位设置为0

    mov al,0b1110_0000
    or al,cl
    out dx,al ;主盘 - LBA模式

    inc dx ;0x1f7
    mov al,0x20 ;读硬盘
    out dx,al

    xor ecx,ecx
    mov cl,bl ;获取读扇区到数量

    .read:
        push cx
        call .waits ;等待数据准备完毕
        call .reads ;读取一个扇区
        pop cx
        loop .read
    ret

    .waits:
        mov dx,0x1f7
        .check:
            in al,dx
            jmp $+2
            jmp $+2
            jmp $+2
            and al,0b1000_1000
            cmp al,0b0000_1000
            jnz .check
        ret
    
    .reads:
        mov dx,0x1f0
        mov cx,256 ;一个扇区256字
        .readw:
            in ax,dx
            jmp $+2
            jmp $+2
            jmp $+2
            mov [edi],ax
            add edi,2
            loop .readw
        ret

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

error:
    mov si,.msg
    call print
    hlt
    jmp $
    .msg:
        db "error",10,13,0


booting:
    db "booting ...",10,13,0 

;固定大小为510个字节，其余空间用0填充
times 510 - ($ - $$) db 0

;固定的 magic number
db 0x55,0xaa