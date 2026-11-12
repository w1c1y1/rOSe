[BITS 32]
[EXTERN endless_loop]
[GLOBAL zero_interrupt_experiment]
zero_interrupt_experiment:
    mov ecx, 0xDEADBEEF
    mov edx, 0xDEADBEEF
    mov ebx, 0xFFFFFFFF
    xor eax, eax
    div eax


[GLOBAL int_experiment]
int_experiment:
    mov ebx, 0xFFFFFFF
    mov eax, 0xDEADBEEF
    mov ecx, 0xDEADBEEF
    mov edx, 0xDEADBEEF
    int 0x40
    


[GLOBAL sti_experiment]
sti_experiment:
    mov eax, 0xDEADBEEF
    mov ecx, 0xDEADBEEF
    mov edx, 0xDEADBEEF
    sti
    call endless_loop
