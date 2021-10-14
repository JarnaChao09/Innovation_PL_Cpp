//
// Created by Jaran Chao on 10/12/21.
//

#include "chunk.h"

void language::Chunk::write(std::uint8_t byte, int32_t line) {
    this->code.push_back(byte);
    this->lines.push_back(line);
}

int language::Chunk::add_constant(language::value_type value) {
    this->constants.write(value);
    return static_cast<int>(this->constants.get_count() - 1);
}

std::size_t language::Chunk::count() const {
    return this->code.size();
}

std::size_t language::Chunk::capacity() const {
    return this->code.capacity();
}