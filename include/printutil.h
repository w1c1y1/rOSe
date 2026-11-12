#ifndef PRINTUTIL_H
#define PRINTUTIL_H

#include "stdarg.h"
#include "stddef.h"
#include "stdint.h"

void init_printer();
void printf(const char* fmt, ...);
void vprintf(const char* fmt, va_list args);
void set_cursor(size_t x, size_t y);
void print_hex_8(uint32_t val);

#endif