//
// Created by Jaran Chao on 10/12/21.
//

#ifndef INNOVATION_PL_DEBUG_H
#define INNOVATION_PL_DEBUG_H

#include "chunk.h"

namespace debug {
    void disassemble_chunk(language::Chunk *chunk, const char *name);

    int disassemble_instruction(language::Chunk *chunk, int offset);
}
#endif //INNOVATION_PL_DEBUG_H
