//
// Created by Jaran Chao on 10/12/21.
//

#include "common.h"
#include "value.h"

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