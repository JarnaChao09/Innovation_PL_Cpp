//
// Created by Jaran Chao on 10/12/21.
//

#include "common.h"
#include "value.h"
#include "object.h"

void language::print_value(value_t value) {
    switch (value.type) {
        case ValueType::Double:
            std::cout << value.as.double_value;
            break;
        case ValueType::Boolean:
            std::cout << std::boolalpha << value.as.boolean_value << std::noboolalpha;
            break;
        case ValueType::Null:
            std::cout << "null";
            break;
        case ValueType::Object:
            language::print_object(value);
            break;
    }
}

bool language::values_equal(language::value_t lhs, language::value_t rhs) {
    if (lhs.type != rhs.type) return false;

    switch (lhs.type) {
        case language::ValueType::Double:
            return lhs.as.double_value == rhs.as.double_value;
        case language::ValueType::Boolean:
            return lhs.as.boolean_value == rhs.as.boolean_value;
        case language::ValueType::Null:
            return true;
        case language::ValueType::Object:
            language::ObjString *lhs_string = AS_STRING(lhs);
            language::ObjString *rhs_string = AS_STRING(rhs);
            return lhs_string->length == rhs_string->length &&
                   memcmp(lhs_string->chars, rhs_string->chars, lhs_string->length) == 0;
    }
}

language::ValueArray::size_type language::ValueArray::get_count() const {
    return this->values.size();
}

language::ValueArray::size_type language::ValueArray::get_capacity() const {
    return this->values.capacity();
}

void language::ValueArray::write(value_t value) {
    this->values.push_back(value);
}