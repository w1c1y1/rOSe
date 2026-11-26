#ifndef INTERRUPTIONS_H
#define INTERRUPTIONS_H

#include "allocator.h"
#include "stdint.h"
#define NUMBER_OF_VECTORS 256
void load_idt(uint8_t type);

#pragma pack(push, 1)
typedef struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t vector;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} context_t;

typedef struct {
    uint16_t low_offset;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t type;
    uint16_t offset_high;
} interrupt_descriptor;

typedef struct {
    uint16_t limit;
    uint32_t base;
} fake_idt;

#pragma pack(pop)

void print_context(context_t* ctx);

#endif