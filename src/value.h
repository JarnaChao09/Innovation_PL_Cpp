//
// Created by Jaran Chao on 10/12/21.
//

#ifndef INNOVATION_PL_VALUE_H
#define INNOVATION_PL_VALUE_H

#include "common.h"
#include <vector>

namespace language {
    using value_type = double;

    void print_value(value_type value);

    struct ValueArray {
        using value_buffer = std::vector<value_type>;
        using size_type = typename value_buffer::size_type;
        std::vector<value_type> values;

        ValueArray() : values() {}

        ~ValueArray() {
            std::destroy(values.begin(), values.end());
        }

        [[nodiscard]] size_type get_count() const;

        [[nodiscard]] size_type get_capacity() const;

        void write(value_type value);
    };
}

#endif //INNOVATION_PL_VALUE_H
