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

// Special prints for escape sequences and specificators
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
