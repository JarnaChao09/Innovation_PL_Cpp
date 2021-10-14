#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "src/chunk.h"
#include "src/debug.h"
#include "src/vm.h"

void repl(language::VM &vm);

void run_file(language::VM &vm, const char *path);

int main(int argc, char *argv[]) {
    language::VM vm = language::VM();

    if (argc == 1) {
        repl(vm);
    } else if (argc == 2) {
        run_file(vm, argv[1]);
    } else {
        std::cerr << "Usage: .exe [path_to_file]\n";
    }
    return 0;
}


void repl(language::VM &vm) {
    std::cout << "Running Repl\n";

    std::string line;

    for (;;) {
        std::cout << "[> ";

        std::cin >> line;

        if (line.empty()) {
            std::cout << "\n";
            break;
        }

        if (line == "exit") {
            std::cout << "Exiting Repl\n";
            break;
        }

        std::cout << line << "\n";
//        vm.interpret(line.c_str());
    }
}

void run_file(language::VM &vm, const char *path) {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << "\n";
        exit(74);
    }

    std::stringstream source;
    source << file.rdbuf();
    file.close();

    std::string source_str = source.str();

    language::InterpreterResult result = vm.interpret(source_str);

    if (result == language::InterpreterResult::CompileError) exit(65);
    if (result == language::InterpreterResult::RuntimeError) exit(70);
}