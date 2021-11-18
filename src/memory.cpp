//
// Created by Jaran Chao on 11/17/21.
//

#include "memory.h"

void *language::reallocate(void *pointer, std::size_t old_size, std::size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return nullptr;
    }

    void *result = realloc(pointer, new_size);
    return result;
}