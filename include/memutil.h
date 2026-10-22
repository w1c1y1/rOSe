#ifndef MEMUTIL_H
#define MEMUTIL_H

#include "stddef.h"
void* memmove(void* dst, const void* src, size_t n);
void* memzero(void* dst, size_t n);

#endif