#include "allocator.h"
#include "assertions.h"
#include "experiments.h"
#include "interruptions.h"
#include "kernel_panic.h"
#include "printutil.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"

#define GATE_TYPE_TRAP_GATE 0b10001111
#define INTERRUPT_GATE 0b10001110

extern void zero_interrupt_experiment();
extern void sti_experiment();
extern void int_experiment();
extern void endless_loop();
extern void pic_setup(bool EOI);
extern void sti();

void kernel_entry() { experiment_20(); }