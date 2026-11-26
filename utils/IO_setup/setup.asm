[BITS 32]
; in al, dx - read
; out dx, al - write from al to dx

%define MASTER_COMMAND 0x20
%define MASTER_DATA 0x21
%define SLAVE_COMMAND 0xA0
%define SLAVE_DATA 0xA1

;MASTER 0x20 0x21
;SLAVE 0xA0 0xA1


waiting_loop:
    mov al, 0
    mov dx, 0x80
    out dx, al
    out dx, al
    out dx, al
    out dx, al
    out dx, al
    out dx, al
    ret 


keybord_and_timer:
    mov dx, MASTER_DATA
    mov al, 0xFF
    out dx, al
    call waiting_loop
    mov dx, SLAVE_DATA
    mov al, 0xFF
    out dx, al
    call waiting_loop
    ret
    



[GLOBAL pic_setup]
pic_setup:
    mov dx, MASTER_COMMAND  ; IWC1 SETUP
    mov al, 0b00010001
    out dx, al
    call waiting_loop
    mov al, 0b00010001
    mov dx, SLAVE_COMMAND
    out dx, al
    call waiting_loop


    mov dx, MASTER_DATA     ; IWC2 setup
    mov al, 0x20
    out dx, al
    call waiting_loop
    mov dx, SLAVE_DATA
    mov al, 0x28
    out dx, al
    call waiting_loop


    mov dx, MASTER_DATA     ; IWC3 setup
    mov al, 0b00000100
    out dx, al
    call waiting_loop
    mov dx, SLAVE_DATA
    mov al, 2
    out dx, al
    call waiting_loop

    mov ecx, [esp + 4]
    cmp ecx, 1
    je .EOI_ON

    
    .EOI_OFF:
        mov dx, MASTER_DATA        ; IWC4 setup without automatic EOI
        mov al, 0b00000001
        out dx, al
        call waiting_loop
        mov dx, SLAVE_DATA
        mov al, 0b00000001
        out dx, al
        call waiting_loop
        call keybord_and_timer
        ret

    
    .EOI_ON:
        mov dx, MASTER_DATA        ; IWC4 setup with automatic EOI
        mov al, 0b00000011
        out dx, al
        call waiting_loop
        mov dx, SLAVE_DATA
        mov al, 0b00000011
        out dx, al
        call waiting_loop
        call keybord_and_timer
        ret



    


    