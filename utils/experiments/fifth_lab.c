#include "allocator.h"
#include "assertions.h"
#include "interruptions.h"
#include "printutil.h"
#include "stdbool.h"
#include "stdint.h"

extern void pic_setup(bool auto_EOI);
extern void sti();
extern uint8_t in(uint16_t port);
extern void out(uint16_t port, uint8_t value);
