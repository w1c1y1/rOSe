#include "allocator.h"
#include "assertions.h"
#include "interruptions.h"
#include "kernel_panic.h"
#include "printutil.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"

extern void zero_interrupt_experiment();
extern void sti_experiment();
extern void int_experiment();
extern void endless_loop();
extern void pic_setup(bool EOI);
extern void sti();

void kernel_entry() {
    init_printer();
    load_idt();
    load_idt();
    pic_setup(0);
    sti();

    // zero_interrupt_experiment();
    // int_experiment();
    // sti_experiment();
    endless_loop();
}