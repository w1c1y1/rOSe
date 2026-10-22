#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "vgautil.h"

#define VGA_HEIGHT 25
#define VGA_WIDTH 80

size_t cursor_x = 0;
size_t cursor_y = 0;

void set_cursor(size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    cursor_x = x;
    cursor_y = y;
}

void init_printer() {
    vga_clear_screen();
    set_cursor(0, 0);
}

// Special prints for escape sequences and specifiers
// \n sequence
void _static_next_line() {
    if (++cursor_y >= VGA_HEIGHT) {
        vga_scroll_down();
        cursor_y = VGA_HEIGHT - 1;
    }
}

// \r sequence
void _move_carriage() { cursor_x = 0; }

// \n + \r
void _next_line() {
    _static_next_line();
    _move_carriage();
}

// Prints char
void _printchar(uint8_t c) {
    if (c == '\n') {
        _next_line();
        return;
    }
    if (c == '\r') {
        _move_carriage();
        return;
    }
    vga_print_char(c, cursor_x, cursor_y);
    if (++cursor_x >= VGA_WIDTH) {
        _next_line();
    }
}

// prints uint
void _print_uint(uint32_t value, uint32_t base, bool is_negative) {
    char buffer[15];
    int i = 0;
    if (value == 0) {
        _printchar('0');
    }
    while (value > 0) {
        uint32_t current_val = value % base;
        buffer[i++] = (char)(current_val < 10 ? '0' + current_val
                                              : 'a' + (current_val - 10));
        value /= base;
    }
    if (is_negative) {
        _printchar('-');
    }
    while (i-- > 0) {
        _printchar(buffer[i]);
    }
}

void _print_int(int32_t value) {
    if (value < 0) {
        _print_uint((uint32_t)(-(uint32_t)value), 10, 1);
    } else {
        _print_uint((uint32_t)value, 10, 0);
    }
}

void vprintf(const char* fmt, va_list args) {
    for (const char* p = fmt; *p; p++) {
        if (*p != '%') {
            _printchar(*p);
            continue;
        }
        p++;
        if (*p == '\0') {
            return;
        }
        switch (*p) {
            case 'd': {
                _print_int(va_arg(args, int32_t));
                break;
            }
            case 'x': {
                _print_uint(va_arg(args, uint32_t), 16, 0);
                break;
            }
            case 'c': {
                _printchar((char)va_arg(args, int32_t));
                break;
            }
            case 's': {
                const char* s = va_arg(args, const char*);
                if (!s) {
                    s = "(null)";
                }
                while (*s) {
                    _printchar(*s++);
                }
                break;
            }
        }
    }
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}
