[BITS 32]
[GLOBAL memmove]
memmove:
    push edi
    push esi
    mov  edi, [esp + 12]       ; dst
    mov  esi, [esp + 16]       ; src
    mov  ecx, [esp + 20]       ; size
    cmp  edi, esi
    je   .exit                 ; if dst == src => exit
    jb   .usual                ; if dst < src => just copy from start to end
    std                        
    lea  esi, [esi + ecx - 1]  ; src + size - 1 = last byte of data
    lea  edi, [edi + ecx - 1]  ; dst + size - 1 = last byte of destination area
    rep  movsb                 ; copy from end of data to start
    cld
    jmp  .exit
    .usual:
        cld
        rep  movsb
    .exit:
        pop  esi
        pop  edi
        ret