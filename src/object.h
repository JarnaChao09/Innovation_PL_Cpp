//
// Created by Jaran Chao on 11/17/21.
//

#ifndef INNOVATION_PL_OBJECT_H
#define INNOVATION_PL_OBJECT_H

#include "common.h"
#include "value.h"

#define AS_OBJ(value)     ((value).as.obj)

#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->chars)

namespace language {
    enum ObjType {
        String,
    };
    struct Obj {
        ObjType type;
    };
    struct ObjString : Obj {
        std::size_t length;
        char *chars;
    };

    ObjString *copy_string(const char *chars, std::size_t length);

    static inline bool is_obj_type(value_t value, ObjType type) {
        return value.type == ValueType::Object && value.as.obj->type == type;
    }

    void print_object(language::value_t value);
}

#endif //INNOVATION_PL_OBJECT_H
