//
// Created by Jaran Chao on 11/17/21.
//

#ifndef INNOVATION_PL_MEMORY_H
#define INNOVATION_PL_MEMORY_H

#include "common.h"

#define ALLOCATE(type, count) \
    (type*)language::reallocate(nullptr, 0, sizeof(type) * (count))

namespace language {
    void *reallocate(void *pointer, std::size_t old_size, std::size_t new_size);
}

#endif //INNOVATION_PL_MEMORY_H
