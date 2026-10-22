#include "allocator.h"
#include "assertions.h"
#include "kernel_panic.h"
#include "printutil.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"

void endless_loop();

void kernel_entry() {
    vga_clear_screen();
    void* array = (uint32_t*)calloc_undead(5 * sizeof(uint32_t), 4);
    printf("%x", (uint32_t)array);
    endless_loop();
}