#include "printutil.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"

void call_cli();
void endless_loop();

void vkernel_panic(const char* fmt, va_list args) {
    call_cli();
    vga_clear_screen();
    set_cursor(0, 0);
    vprintf(fmt, args);
    endless_loop();
}

void kernel_panic(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
    va_end(args);
}