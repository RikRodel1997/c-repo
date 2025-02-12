#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define GROW_CAPACITY(cap) ((cap) < 8 ? 8 : (cap) * 2)
#define GROW_ARRAY(type, ptr, old_count, new_count)                                                                    \
    (type*) reallocate(ptr, sizeof(type) * (old_count), sizeof(type) * (new_count))

#define FREE_ARRAY(type, ptr, old_count) reallocate(ptr, sizeof(type) * (old_count), 0)

void* reallocate(void* ptr, size_t old_size, size_t new_size);

#endif