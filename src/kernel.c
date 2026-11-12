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

void kernel_entry() {
    init_printer();
    load_idt();

    zero_interrupt_experiment();
    // int_experiment();
    // sti_experiment();
    endless_loop();
}