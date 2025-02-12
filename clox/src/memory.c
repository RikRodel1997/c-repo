#include <stdlib.h>

#include "../__test__/mocks.h"
#include "../memory.h"

/*
    The 2 size arguments control which operation to perform
    if old_size == 0 and new_size != 0          -> Allocate new block
    if old_size != 0 and new_size == 0          -> Free allocation
    if old_size != 0 and new_size < old_size    -> Shrink existing allocation
    if old_size != 0 and new_size > old_size    -> Grow existing allocation
 */
void* reallocate(void* ptr, size_t old_size, size_t new_size) {
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    void* result = realloc(ptr, new_size);
    if (result == NULL) {
        exit(1);
    }
    return result;
}