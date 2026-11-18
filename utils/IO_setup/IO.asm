[BITS 32]
[GLOBAL sti]
sti:
    sti
    ret


[GLOBAL out]
out:
    mov edx, [esp + 4]
    mov eax, [esp + 8]
    out dx, al
    ret


[GLOBAL in]
in: 
    mov edx, [esp + 4]
    xor eax, eax
    in al, dx
    ret