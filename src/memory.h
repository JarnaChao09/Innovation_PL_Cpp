//
// Created by Jaran Chao on 11/17/21.
//

#ifndef INNOVATION_PL_MEMORY_H
#define INNOVATION_PL_MEMORY_H

#include "common.h"
#include "vm.h"

#define ALLOCATE(type, count) \
    (type*)language::reallocate(nullptr, 0, sizeof(type) * (count))

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)language::reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

#define FREE(type, pointer) language::reallocate(pointer, sizeof(type), 0)

#define FREE_ARRAY(type, pointer, oldCount) \
    language::reallocate(pointer, sizeof(type) * (oldCount), 0)

namespace language {
    void *reallocate(void *pointer, std::size_t old_size, std::size_t new_size);

    void free_objects(language::VM *vm);
}

#endif //INNOVATION_PL_MEMORY_H
