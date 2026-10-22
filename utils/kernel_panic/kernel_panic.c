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
    init_printer();
    vprintf(fmt, args);
    endless_loop();
}

void kernel_panic(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
    va_end(args);
}