#include "vgautil.h"

void endless_loop();

void kernel_entry() {
  vga_clear_screen();
  endless_loop();
}