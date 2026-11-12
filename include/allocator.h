#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "stddef.h"
#include "stdint.h"

void* malloc_undead(size_t size, size_t alignment);
void* calloc_undead(size_t size, size_t alignment);
#endif