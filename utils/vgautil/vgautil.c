#include "memutil.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

#define VGA_HEIGHT 25
#define VGA_WIDTH 80

void vga_clear_screen() {
    uint16_t* start = (uint16_t*)0xB8000;
    for (size_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        start[i] = 0;
    }
}

void vga_clear_line(size_t index) {
    uint16_t* vga_start = (uint16_t*)0xB8000;
    uint16_t* line_to_clear = vga_start + VGA_WIDTH * index;
    for (size_t i = 0; i < VGA_WIDTH; i++) {
        line_to_clear[i] = 0;
    }
}

void vga_clear_column(size_t index) {
    uint16_t* vga = (uint16_t*)0xB8000;
    for (size_t i = 0; i < VGA_HEIGHT; i++) {
        vga[i * VGA_WIDTH + index] = 0;
    }
}

void vga_print_char(char c, size_t x, size_t y) {
    size_t address = 0xB8000 + 2 * (y * 80 + x);
    *((size_t*)address) = c;
}

void vga_scroll_down() {
    uint16_t* start = (uint16_t*)0xB8000;
    uint16_t* end = start + VGA_WIDTH;
    size_t size = VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t);
    memmove((void*)start, (void*)end, size);
    vga_clear_line(VGA_HEIGHT - 1);
}