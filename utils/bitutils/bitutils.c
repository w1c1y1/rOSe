#include "memutil.h"
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

bool getBit(uint32_t value, int number_of_bit) {
    return (value & (1 << number_of_bit)) != 0;
}