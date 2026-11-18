[BITS 32]
; in al, dx - read
; out dx, al - write from al to dx


;MASTER 0x20 0x21
;SLAVE 0xA0 0xA1


waiting_loop:
    mov al, 0
    out 0x80, al
    out 0x80, al
    ret



[GLOBAL pic_setup]
pic_setup:
    mov dx, 0x20        ; setup MASTER IWC1
    mov al, 0b00010001
    out dx, al
    call waiting_loop
    mov dx, 0xA0        ; setup SLAVE IWC1
    out dx, al
    call waiting_loop


    mov dx, 0xA1        ; SLAVE IWC2
    mov al, 0x28
    out dx, al
    call waiting_loop
    mov dx, 0x21        ; MASTER IWC2
    mov al, 0x20
    out dx, al
    call waiting_loop


    mov al, 0b00000100  ; MASTER IWC3
    out dx, al
    call waiting_loop
    mov al, 2           ; SLAVE IWC3
    mov dx, 0xA1
    out dx, al
    call waiting_loop

    mov edx, [esp + 4]
    cmp edx, 1
    je .EOI_ON

    
    .EOI_OFF:
        mov dx, 0xA1        ; IWC4 without automatic EOI
        mov al, 0b00000001
        out dx, al
        call waiting_loop
        mov dx, 0x21
        out dx, al
        call waiting_loop
        ret

    
    .EOI_ON:
        mov dx, 0xA1        ; IWC4 with automatic EOI
        mov al, 0b00000011
        out dx, al
        call waiting_loop
        mov dx, 0x21
        out dx, al
        call waiting_loop
        ret



    


    