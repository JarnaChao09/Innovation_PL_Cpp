//
// Created by Jaran Chao on 11/17/21.
//

#include <string>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocate_object(sizeof(type), objectType)

static language::Obj* allocate_object(std::size_t size, language::ObjType type) {
    auto* object = (language::Obj*)language::reallocate(nullptr, 0, size);
    object->type = type;
    return object;
}

static language::ObjString* allocate_string(char* chars, std::size_t length) {
    auto* string = ALLOCATE_OBJ(language::ObjString, language::ObjType::String);
    string->length = length;
    string->chars = chars;
    return string;
}

language::ObjString* language::copy_string(const char* chars, std::size_t length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocate_string(heapChars, length);
}

void language::print_object(language::value_t value) {
    switch (value.as.obj->type) {
        case ObjType::String:
            std::cout << AS_CSTRING(value);
            break;
    }
}