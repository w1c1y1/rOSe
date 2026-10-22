#include "stdarg.h"
#include "stddef.h"
void init_printer();
void printf(const char* fmt, ...);
void vprintf(const char* fmt, va_list args);
void set_cursor(size_t x, size_t y);