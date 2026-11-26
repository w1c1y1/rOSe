#ifndef IOUTILS_H
#define IOUTILS_H
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
#define SYSTEM_TIMER 0
#define KEYBOARD 1
#define EOI 0x20
#define SLAVE_COMMAND 0xA0
#define MASTER_COMMAND 0x20
#define MASTER_DATA 0x21
#define SLAVE_DATA 0xA1
#define INTERRUPT_GATE 0b10001110
#define TRAP_GATE 0b10001111
typedef void (*irq_handler_t)(context_t*);
extern irq_handler_t irq_handlers[16];
extern void out(uint16_t port, uint8_t value);
extern uint8_t in(uint16_t port);
extern void waiting_loop();
void send_EOI(uint8_t iqr);
void timer_handler(context_t* ctx);
void enable_device(uint8_t irq, void (*handler)(context_t*));
void disable_device(uint8_t irq);
#endif