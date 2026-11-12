[BITS 32]
%define CODE 0x08
%define DATA 0x10


[GLOBAL lidt]
lidt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

[GLOBAL context_save]
context_save:
    push ds
    push es
    push fs
    push gs
    pusha
    cld
    mov ax, DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebx, esp
    and esp, -16
    sub esp, 12
    push ebx
    [EXTERN universal_handler]
    call universal_handler
    mov esp, ebx
    popa
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 8
    iret
