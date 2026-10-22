#include "stdarg.h"
#include "stddef.h"

void vkernel_panic(const char* fmt, va_list args);
void kernel_panic(const char* fmt, ...);
