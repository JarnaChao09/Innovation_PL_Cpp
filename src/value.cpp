//
// Created by Jaran Chao on 10/12/21.
//

#include "common.h"
#include "value.h"

void language::print_value(value_type value) {
    printf("%g", value);
}

language::ValueArray::size_type language::ValueArray::get_count() const {
    return this->values.size();
}

language::ValueArray::size_type language::ValueArray::get_capacity() const {
    return this->values.capacity();
}

void language::ValueArray::write(value_type value) {
    this->values.push_back(value);
}