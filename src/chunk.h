//
// Created by Jaran Chao on 10/12/21.
//

#ifndef INNOVATION_PL_CHUNK_H
#define INNOVATION_PL_CHUNK_H

#include "common.h"
#include "value.h"
#include <vector>

namespace language {
    enum OpCode {
        Constant,
        Negate,
        Add,
        Subtract,
        Multiply,
        Divide,
//        Constant_Long,
        Return,
    };

    struct Chunk {
        using byte_buffer = std::vector<std::uint8_t>;
        using line_buffer = std::vector<int32_t>;

        byte_buffer code;
        line_buffer lines;
        language::ValueArray constants;

        Chunk() : code(), lines(), constants() {}

        ~Chunk() {
            std::destroy(code.begin(), code.end());
            std::destroy(lines.begin(), lines.end());
        }

        void write(std::uint8_t byte, int32_t line);

        int add_constant(language::value_type value);

        [[nodiscard]] std::size_t count() const;

        [[nodiscard]] std::size_t capacity() const;
    };
}

#endif //INNOVATION_PL_CHUNK_H
