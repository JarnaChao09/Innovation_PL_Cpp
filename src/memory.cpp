//
// Created by Jaran Chao on 11/17/21.
//

#include "memory.h"
#include "object.h"

void *language::reallocate(void *pointer, std::size_t old_size, std::size_t new_size) {
    if (new_size == 0) {
        free(pointer);
        return nullptr;
    }

    void *result = realloc(pointer, new_size);
    return result;
}

static void free_object(language::Obj* object) {
    switch (object->type) {
        case language::ObjType::String: {
            auto *string = (language::ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(language::ObjString, object);
            break;
        }
    }
}

void language::free_objects(language::VM *vm) {
    Obj *object = vm->objects;
    while (object != nullptr) {
        Obj *next = object->next;
        free_object(object);
        object = next;
    }
}