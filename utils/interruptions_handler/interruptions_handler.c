#include "allocator.h"
#include "assertions.h"
#include "bitutils.h"
#include "interruptions.h"
#include "kernel_panic.h"
#include "printutil.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"

#define SIZE_OF_VECTOR 16
#define VECTOR_ALIGNMENT

extern void context_save();
extern void lidt(fake_idt* address);
extern void endless_loop();
extern void pic_setup(bool auto_EOI);
extern void sti();
extern uint8_t in(uint16_t port);
extern void out(uint16_t port, uint8_t value);

bool has_error_code(int vector) {
    return (vector == 8) || (vector >= 10 && vector <= 14) || (vector == 17) ||
           (vector == 21);
}

void generate_trampline(int index, uint8_t* current_address) {
    int offset = 0;
    if (!has_error_code(index)) {
        current_address[offset++] = 0x6A;  // push 0x00
        current_address[offset++] = 0x00;
    }
    current_address[offset++] = 0x68;              //  push (current number)
    current_address[offset++] = (uint8_t)(index);  // younger part
    current_address[offset++] = (uint8_t)(index >> 8);  // elder part
    current_address[offset++] = 0x00;                   // filling to 4 bytes
    current_address[offset++] = 0x00;
    current_address[offset++] = 0xE9;
    int32_t rel_offset = (int32_t)((uintptr_t)context_save -
                                   (uintptr_t)(current_address + offset + 4));
    *(int32_t*)(current_address + offset) = rel_offset;
    offset += 4;
    while (offset < 16) {
        current_address[offset++] = 0x90;  // fill to 16 bytes (nop)
    }
}

void load_idt() {
    interrupt_descriptor* idt = (interrupt_descriptor*)malloc_undead(
        256 * sizeof(interrupt_descriptor), 16);
    uint8_t* pool =
        (uint8_t*)malloc_undead(NUMBER_OF_VECTORS * SIZE_OF_VECTOR, 16);
    for (int i = 0; i < NUMBER_OF_VECTORS; i++) {
        uint8_t* current_address = pool + (i * SIZE_OF_VECTOR);
        generate_trampline(i, current_address);
        idt[i].low_offset = (uint16_t)(current_address);  // younger two bytes
        idt[i].segment_selector = 0x08;
        idt[i].reserved = 0;
        idt[i].type = 0b10001110;  // Interrupt gate type
        idt[i].offset_high =
            (uint16_t)((uint32_t)(current_address) >> 16);  // older two bytes
    }
    fake_idt pseudo_idt;
    pseudo_idt.base = (uint32_t)idt;                              // idt address
    pseudo_idt.limit = (256 * sizeof(interrupt_descriptor)) - 1;  // size - 1
    lidt(&pseudo_idt);
}

void print_context(context_t* ctx) {
    printf("Kernel panic: unhandled interrupt #%x\n", ctx->vector);

    printf("At CS:EIP = 0x");
    print_hex_8(ctx->cs);
    printf(":0x");
    print_hex_8(ctx->eip);
    printf("\n\n");

    printf("Registers:\n");

    printf("  EAX: 0x");
    print_hex_8(ctx->eax);
    printf(", ECX: 0x");
    print_hex_8(ctx->ecx);
    printf(", EDX: 0x");
    print_hex_8(ctx->edx);
    printf(", EBX: 0x");
    print_hex_8(ctx->ebx);
    printf("\n");

    printf("  ESP: 0x");
    print_hex_8(ctx->esp);
    printf(", EBP: 0x");
    print_hex_8(ctx->ebp);
    printf(", ESI: 0x");
    print_hex_8(ctx->esi);
    printf(", EDI: 0x");
    print_hex_8(ctx->edi);
    printf("\n");

    printf("  DS : 0x");
    print_hex_8(ctx->ds & 0xFFFF);
    printf(", ES : 0x");
    print_hex_8(ctx->es & 0xFFFF);
    printf(", FS : 0x");
    print_hex_8(ctx->fs & 0xFFFF);
    printf(", GS : 0x");
    print_hex_8(ctx->gs & 0xFFFF);
    printf("\n\n");

    printf("Error code:\n");
    printf("  value: 0x");
    print_hex_8(ctx->error_code);
    printf("\n\n");

    printf("EFLAGS:\n");
    printf("  value: 0x");
    print_hex_8(ctx->eflags);
    printf("\n");

    printf("\nEssential Flags:\n");
    printf("  CF: ");
    printf("%d, ", getBit(ctx->eflags, 0));
    printf("IF: ");
    printf("%d, ", getBit(ctx->eflags, 9));
    printf("DF: ");
    printf("%d, ", getBit(ctx->eflags, 10));
    printf("OF: ");
    printf("%d\n", getBit(ctx->eflags, 11));
}

void universal_handler(context_t* ctx) {
    if (ctx->vector >= 32) {
        if (ctx->vector == 33) {
            return;
        }
        if (ctx->vector >= 40) {
            out(0xA0, 0x20);
        }
        out(0x20, 0x20);
    }
    vga_clear_screen();
    print_context(ctx);
    endless_loop();
}