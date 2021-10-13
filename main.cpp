#include "src/chunk.h"
#include "src/debug.h"
#include "src/vm.h"

int main() {
    language::VM vm = language::VM();
    language::Chunk chunk = language::Chunk();

    int constant = chunk.add_constant(1.2);

    chunk.write(language::OpCode::Constant, 1);
    chunk.write(constant, 1);

    constant = chunk.add_constant(3.4);

    chunk.write(language::OpCode::Constant, 1);
    chunk.write(constant, 1);

    chunk.write(language::OpCode::Add, 1);

    constant = chunk.add_constant(5.6);
    chunk.write(language::OpCode::Constant, 1);
    chunk.write(constant, 1);

    chunk.write(language::OpCode::Divide, 1);

    chunk.write(language::OpCode::Negate, 1);

    chunk.write(language::OpCode::Return, 1);

    debug::disassemble_chunk(&chunk, "test chunk");

    vm.interpret(&chunk);
    return 0;
}
