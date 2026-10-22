#include "assertions.h"
#include "memutil.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

extern uint8_t kernel_end[];

uintptr_t arena_start = (uintptr_t)kernel_end;
uintptr_t arena_end = (uintptr_t)0x80000;
uintptr_t current_start = (uintptr_t)kernel_end;

void* malloc_undead(size_t size, size_t alignment) {
    uintptr_t aligned_address =
        (current_start + alignment - 1) & ~(alignment - 1);
    assert(aligned_address + size >= aligned_address);
    assert(aligned_address + size <= arena_end);
    void* address_to_return = (void*)aligned_address;
    current_start = aligned_address + size;
    return address_to_return;
}

void* calloc_undead(size_t size, size_t alignment) {
    void* address_to_return = malloc_undead(size, alignment);
    assert(address_to_return != NULL);
    memzero(address_to_return, size);
    return address_to_return;
}