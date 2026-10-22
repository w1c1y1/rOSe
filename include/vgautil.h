#ifndef VGAUTIL_H
#define VGAUTIL_H

#include "stddef.h"
void vga_clear_screen();
void vga_clear_line(size_t index);
void vga_clear_column(size_t index);
void vga_print_char(char c, size_t x, size_t y);
void vga_scroll_down();

#endif