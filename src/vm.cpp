//
// Created by Jaran Chao on 10/12/21.
//

#include "debug.h"
#include "compiler.h"
#include "value.h"
#include "vm.h"
#include "memory.h"

//language::InterpreterResult language::VM::interpret(language::Chunk *_chunk) {
//    this->chunk = _chunk;
//    this->ip = &this->chunk->code[0];
//    return this->run();
//}

//language::InterpreterResult language::VM::interpret(const std::string &source) {
//    language::compile(source);
//    return InterpreterResult::Ok;
//}

void language::VM::runtime_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fputs("\n", stdout);

    std::size_t instruction = this->ip - this->chunk->code.data() - 1;
    int line = this->chunk->lines[instruction];
    fprintf(stdout, "[line %d] in script\n", line);
    this->reset_stack();
}

bool is_falsey(language::value_t value) {
    return value.type == language::ValueType::Null ||
           (value.type == language::ValueType::Boolean && !value.as.boolean_value);
}

void language::VM::concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    std::size_t length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = language::take_string(this, chars, length);
    push(language::value_t(ValueType::Object, (Obj*)result));
}

language::InterpreterResult language::VM::interpret(const std::string &source) {
    auto compiler = new language::Compiler(source);

    bool error = false;

    language::Expr *current_expr = compiler->compile(this, source, error);

    if (error) {
        return language::InterpreterResult::CompileError;
    }

    if (current_expr) {
        std::cout << current_expr->to_string() << "\n";
    } else {
        std::cout << "null\n";
    }

    auto *compiled_chunk = new Chunk();

    language::CodeGenerator code_generator(compiled_chunk, current_expr);

    code_generator.generate();

    if (code_generator.had_error) {
        return language::InterpreterResult::CompileError;
    }

    std::cout << "\n";

    debug::disassemble_chunk(compiled_chunk, "REPL");

    std::cout << "\n";

    this->chunk = compiled_chunk;
    this->ip = this->chunk->code.data();

    language::InterpreterResult res = this->run();

    return res;
}

language::InterpreterResult language::VM::run() {
#define READ_BYTE() (*this->ip++)
#define READ_CONSTANT() (this->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(value_type, op) \
    do { \
      double b = this->pop().as.double_value; \
      double a = this->pop().as.double_value; \
      this->push(language::value_t(value_type, a op b)); \
    } while (false)

    for (;;) {
        std::uint8_t instruction;
        switch (instruction = READ_BYTE()) {
#ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (value_t *slot = this->stack; slot < this->stack_top; slot++) {
                printf("[ ");
                language::print_value(*slot);
                printf(" ]");
            }
            printf("\n");
            debug::disassemble_instruction(this->chunk, (int) (std::size_t) (this->ip - this->chunk->code[0]));
#endif
            case static_cast<std::uint8_t>(language::OpCode::Constant): {
                language::value_t constant = READ_CONSTANT();
                this->push(constant);
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Identity): {
                this->push(language::value_t(ValueType::Double, +this->pop().as.double_value));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Negate): {
                this->push(language::value_t(ValueType::Double, -this->pop().as.double_value));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Not): {
                this->push(language::value_t(ValueType::Boolean, is_falsey(this->pop())));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::True): {
                this->push(language::value_t(ValueType::Boolean, true));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::False): {
                this->push(language::value_t(ValueType::Boolean, false));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Ne): {
                language::value_t b = this->pop();
                language::value_t a = this->pop();
                this->push(language::value_t(ValueType::Boolean, !language::values_equal(a, b)));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Eq): {
                language::value_t b = this->pop();
                language::value_t a = this->pop();
                this->push(language::value_t(ValueType::Boolean, language::values_equal(a, b)));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Gt): {
                BINARY_OP(language::ValueType::Boolean, >);
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Ge): {
                BINARY_OP(language::ValueType::Boolean, >=);
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Lt): {
                BINARY_OP(language::ValueType::Boolean, <);
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Le): {
                BINARY_OP(language::ValueType::Boolean, <=);
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Null): {
                this->push(language::value_t(ValueType::Null));
                break;
            }
            case static_cast<std::uint8_t>(language::OpCode::Add):
                if (this->peek(1).type == language::ValueType::Double &&
                    this->peek(0).type == language::ValueType::Double) {
                    BINARY_OP(language::ValueType::Double, +);
                } else if (this->peek(1).type == language::ValueType::Object &&
                           this->peek(0).type == language::ValueType::Object &&
                           this->peek(1).as.obj->type == language::ObjType::String &&
                           this->peek(0).as.obj->type == language::ObjType::String) {
                    this->concatenate();
                }
                break;
            case static_cast<std::uint8_t>(language::OpCode::Subtract):
                BINARY_OP(language::ValueType::Double, -);
                break;
            case static_cast<std::uint8_t>(language::OpCode::Multiply):
                BINARY_OP(language::ValueType::Double, *);
                break;
            case static_cast<std::uint8_t>(language::OpCode::Divide):
                BINARY_OP(language::ValueType::Double, /);
                break;
            case static_cast<std::uint8_t>(language::OpCode::Return): {
                language::print_value(this->pop());
                printf("\n");
                return InterpreterResult::Ok;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

void language::VM::reset_stack() {
    this->stack_top = this->stack;
}

void language::VM::push(value_t value) {
    *this->stack_top++ = value;
}

language::value_t language::VM::pop() {
    return *--this->stack_top;
}

language::value_t language::VM::peek(int distance) {
    return this->stack_top[-1 - distance];
}

void language::VM::free_objects() {
    language::Obj *current = this->objects;
//    int i = 0;
//    while (current != nullptr) {
//        language::Obj *next = current->next;
//        std::cout << "objects[" << i++ << "] " << current << "\n";
//        current = next;
//    }
    language::free_objects(this);
}