//
// Created by Jaran Chao on 10/12/21.
//

#ifndef INNOVATION_PL_VALUE_H
#define INNOVATION_PL_VALUE_H

#include "common.h"
#include <vector>

namespace language {
    enum class ValueType {
//        Byte,
//        Short,
//        Int,
//        Long,
//        Float,
        Double,
//        Char,
//        String,
        Boolean,
        Null,
    };
    struct value_t {
        ValueType type;
        union {
            bool boolean_value;
            double double_value;
        } as;

        value_t(ValueType type, bool value): type(type), as() {
            this->as.boolean_value = value;
        }

        value_t(ValueType type, double value): type(type), as() {
            this->as.double_value = value;
        }

        explicit value_t(ValueType type): type(type), as() {
            this->as.double_value = 0.0;
        }

        value_t(): type(), as() {}
    };


    void print_value(value_t value);

    struct ValueArray {
        using value_buffer = std::vector<value_t>;
        using size_type = typename value_buffer::size_type;
        std::vector<value_t> values;

        ValueArray() : values() {}

        ~ValueArray() {
            std::destroy(values.begin(), values.end());
        }

        [[nodiscard]] size_type get_count() const;

        [[nodiscard]] size_type get_capacity() const;

        void write(value_t value);
    };
}

#endif //INNOVATION_PL_VALUE_H
