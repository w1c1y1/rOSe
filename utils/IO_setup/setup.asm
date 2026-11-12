[BITS 32]
; in al, dx - read
; out dx, al - write from al to dx


;MASTER 0x20 0x21
;SLAVE 0xA0 0xA1
waiting_loop:
    mov ecx, 15
    .loop:
        dec ecx
        cpuid
        cmp ecx, 0
        jne .loop


[GLOBAL pic_setup]
pic_setup:
    mov dx, 0x20        ;setup MASTER
    mov al, 0b0001001   ; IWC1
    out dx, al
    mov dx, 0x21        ; IWC2
    mov al, 0x20
    out dx, al
    


    