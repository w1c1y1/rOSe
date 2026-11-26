#include "ioutils.h"

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

uint32_t timer_counter = 0;

extern void endless_loop();
extern void sti();

void (*irq_handlers[16])(context_t*) = {0};

void enable_device(uint8_t irq, void (*handler)(context_t*)) {
    irq_handlers[irq] = handler;
    uint16_t port;
    uint8_t bit;
    if (irq < 8) {
        port = MASTER_DATA;
        bit = irq;
    } else {
        port = SLAVE_DATA;
        bit = irq - 8;
    }
    uint8_t mask = in(port);
    mask &= ~(1 << bit);
    out(port, mask);
}

void disable_device(uint8_t irq) {
    irq_handlers[irq] = 0;
    int16_t port;
    uint8_t bit;
    if (irq < 8) {
        port = MASTER_DATA;
        bit = irq;
    } else {
        port = SLAVE_DATA;
        bit = irq - 8;
    }
    uint8_t mask = in(port);
    mask |= (1 << bit);
    out(port, mask);
}

void send_EOI(uint8_t irq) {
    if (irq >= 8) {
        out(SLAVE_COMMAND, EOI);
    }
    out(MASTER_COMMAND, EOI);
}

void timer_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
    send_EOI(SYSTEM_TIMER);
}