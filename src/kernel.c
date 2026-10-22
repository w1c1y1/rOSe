#include "assertions.h"
#include "kernel_panic.h"
#include "printutil.h"
#include "vgautil.h"

void endless_loop();

void kernel_entry() {
    vga_clear_screen();
    assert(3 == 2);
    endless_loop();
}