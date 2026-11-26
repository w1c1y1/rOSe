#include "allocator.h"
#include "assertions.h"
#include "interruptions.h"
#include "ioutils.h"
#include "kernel_panic.h"
#include "printutil.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"
extern void pic_setup(bool EOI_type);
extern void sti();
extern void endless_loop();
extern uint32_t timer_counter;

void timer_handler_1(context_t* ctx) {
    printf("%d", timer_counter++);
    send_EOI(SYSTEM_TIMER);
}

void experiment_1() {
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    sti();
    endless_loop();
}

void experiment_2_handler(context_t* ctx) {
    print_context(ctx);
    endless_loop();
}

void experiment_2() {
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_2_handler);
    sti();
    endless_loop();
}

void experiment_3() {
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    enable_device(KEYBOARD, experiment_2_handler);
    sti();
    endless_loop();
}

void experiment_4_handler(context_t* ctx) { printf("%d ", timer_counter++); }
void experiment_5_handler(context_t* ctx) { timer_counter = 0; }

void experiment_4() {
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_4_handler);
    sti();
    endless_loop();
}

void experiment_5() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_5_handler);
    sti();
    while (1) {
        printf("%d ", timer_counter++);
    }
}

void experiment_6_handler(context_t* ctx) { printf("%d ", timer_counter++); }
void experiment_6() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_6_handler);
    sti();
    while (1) {
        printf("%d ", timer_counter++);
    }
}

void experiment_7() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_6_handler);
    sti();
    endless_loop();
}

void experiment_8_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
    send_EOI(SYSTEM_TIMER);
}
void experiment_8() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_8_handler);
    sti();
    endless_loop();
}

void experiment_9_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
    send_EOI(SYSTEM_TIMER);
    sti();
    endless_loop();
}

void experiment_9() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_9_handler);
    sti();
    endless_loop();
}

void experiment_10_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
    if (timer_counter < 30) {
        send_EOI(SYSTEM_TIMER);
        sti();
    }
    endless_loop();
}

void experiment_10() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_10_handler);
    sti();
    endless_loop();
}

void experiment_11_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
    if (timer_counter < 30) {
        send_EOI(SYSTEM_TIMER);
    }
    endless_loop();
}

void experiment_11() {
    timer_counter = 0;
    init_printer();
    load_idt(TRAP_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_11_handler);
    sti();
    endless_loop();
}

void experiment_12_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
    if (timer_counter < 30) {
        sti();
    }
    endless_loop();
}

void experiment_12() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_12_handler);
    sti();
    endless_loop();
}

void experiment_13_handler(context_t* ctx) {
    uint16_t port = 0x60;
    uint8_t scancode = in(port);
    printf("%d ", scancode);
}

void experiment_13() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(KEYBOARD, experiment_13_handler);
    sti();
    endless_loop();
}

void experiment_14_timer_handler(context_t* ctx) {
    printf("%d ", timer_counter++);
}

void experiment_14() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_14_timer_handler);
    enable_device(KEYBOARD, experiment_13_handler);
    sti();
    endless_loop();
}

void experiment_15_timer(context_t* ctx) {
    printf("%d ", timer_counter++);
    endless_loop();
}

void experiment_15() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_15_timer);
    enable_device(KEYBOARD, experiment_13_handler);
    sti();
    endless_loop();
}

void experiment_16_keyboard(context_t* ctx) {
    uint16_t port = 0x60;
    uint8_t scancode = in(port);
    printf("%d ", scancode);
    endless_loop();
}

void experiment_16() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_14_timer_handler);
    enable_device(KEYBOARD, experiment_16_keyboard);
    sti();
    endless_loop();
}

void experiment_17_keyboard(context_t* ctx) {
    uint16_t port = 0x60;
    uint8_t scancode = in(port);
    printf("%d ", scancode);
    sti();
    endless_loop();
}

void experiment_17() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_14_timer_handler);
    enable_device(KEYBOARD, experiment_17_keyboard);
    sti();
    endless_loop();
}

void experiment_18_timer(context_t* ctx) {
    printf("%d ", timer_counter++);
    endless_loop();
}

void experiment_18_keyboard(context_t* ctx) {
    uint16_t port = 0x60;
    uint8_t scancode = in(port);
    printf("%d ", scancode);
    send_EOI(KEYBOARD);
}

void experiment_18() {
    timer_counter = 0;
    init_printer();
    load_idt(TRAP_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_18_timer);
    enable_device(KEYBOARD, experiment_18_keyboard);
    sti();
    endless_loop();
}

void experiment_19_timer(context_t* ctx) {
    printf("%d ", timer_counter++);
    send_EOI(SYSTEM_TIMER);
}

void experiment_19_keyboard(context_t* ctx) {
    uint16_t port = 0x60;
    uint8_t scancode = in(port);
    printf("%d ", scancode);
    endless_loop();
}

void experiment_19() {
    timer_counter = 0;
    init_printer();
    load_idt(TRAP_GATE);
    pic_setup(0);
    enable_device(SYSTEM_TIMER, experiment_19_timer);
    enable_device(KEYBOARD, experiment_19_keyboard);
    sti();
    endless_loop();
}

void delay() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 50000; j++) {
            out(0x80, 0x80);
        }
        printf("%d ", i);
    }
    printf("\n");
}

void experiment_20_timer(context_t* ctx) {
    disable_device(SYSTEM_TIMER);
    delay();
    sti();
    delay();
}

void experiment_20() {
    timer_counter = 0;
    init_printer();
    load_idt(INTERRUPT_GATE);
    pic_setup(1);
    enable_device(SYSTEM_TIMER, experiment_20_timer);
    enable_device(KEYBOARD, experiment_19_keyboard);
    sti();
    endless_loop();
}