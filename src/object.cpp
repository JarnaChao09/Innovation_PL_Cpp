//
// Created by Jaran Chao on 11/17/21.
//

#include <string>

#include "memory.h"
#include "object.h"
#include "value.h"

#define ALLOCATE_OBJ(type, objectType, vm) \
    (type*)allocate_object((vm), sizeof(type), objectType)

#define ALLOCATE_OBJ_AST(type, objectType) \
    (type*)allocate_object_AST(sizeof(type), objectType)

static language::Obj* allocate_object(language::VM *vm, std::size_t size, language::ObjType type) {
    auto* object = (language::Obj*)language::reallocate(nullptr, 0, size);
    object->type = type;
    object->next = vm->objects;
    vm->objects = object;
    return object;
}

static language::Obj* allocate_object_AST(std::size_t size, language::ObjType type) {
    auto* object = (language::Obj*)language::reallocate(nullptr, 0, size);
    object->type = type;
    return object;
}

static language::ObjString* allocate_string(language::VM *vm, char* chars, std::size_t length) {
    auto* string = ALLOCATE_OBJ(language::ObjString, language::ObjType::String, vm);
    string->length = length;
    string->chars = chars;
    return string;
}

static language::ObjString* allocate_string_AST(char* chars, std::size_t length) {
    auto* string = ALLOCATE_OBJ_AST(language::ObjString, language::ObjType::String);
    string->length = length;
    string->chars = chars;
    return string;
}

language::ObjString* language::copy_string(language::VM *vm, const char* chars, std::size_t length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocate_string(vm, heapChars, length);
}

language::ObjString* language::copy_string_AST(const char* chars, std::size_t length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocate_string_AST(heapChars, length);
}

void language::print_object(language::value_t value) {
    switch (value.as.obj->type) {
        case ObjType::String:
            std::cout << AS_CSTRING(value);
            break;
    }
}

language::ObjString* language::take_string(language::VM *vm, char* chars, std::size_t length) {
    return allocate_string(vm, chars, length);
}