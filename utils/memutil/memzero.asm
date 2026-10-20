[BITS 32]
[GLOBAL memzero]
memzero:
    xor eax, eax
    push edi
    mov edi, [esp + 8]
    mov ecx, [esp + 12]
    cld
    rep stosb
    pop edi
    ret

