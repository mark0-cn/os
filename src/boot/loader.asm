[org 0x1000]

dw 0x1122 ;magic num 用于检查是否加载正确
mov si,loading
call print

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

loading:
    db "loading ...",10,13,0 