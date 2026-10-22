#include "assertions.h"
#include "memutil.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

extern uint32_t kernel_end[];
uintptr_t arena_start = (uintptr_t)(&kernel_end);
uintptr_t arena_end = (uintptr_t)(0x80000);
uintptr_t current_start = (uintptr_t)(&kernel_end);

void* malloc_undead(size_t size, size_t alignment) {
    uintptr_t aligned_adress =
        (current_start + alignment - 1) & ~(alignment - 1);
    assert((aligned_adress + size) < aligned_adress);
    assert(aligned_adress + size > arena_end);
    void* adress_to_return = (void*)aligned_adress;
    current_start = aligned_adress += size;
    return adress_to_return;
}

void* calloc_undead(size_t size, size_t alignment) {
    uintptr_t aligned_adress =
        (current_start + alignment - 1) & ~(alignment - 1);
    assert((aligned_adress + size) < aligned_adress);
    assert(aligned_adress + size > arena_end);
    void* adress_to_return = (void*)aligned_adress;
    current_start = aligned_adress += size;
    memzero(adress_to_return, size);
    return adress_to_return;
}