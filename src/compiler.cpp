//
// Created by Jaran Chao on 10/13/21.
//

#include "compiler.h"
#include "scanner.h"

void language::compile(std::string &source) {
    auto scanner = language::Scanner(source);

    int line = -1;

    for (;;) {
        language::Token token = scanner.scan_token();
        if (token.line != line) {
            printf("%d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }

//        printf("%2d '%.*s'\n", token.type, static_cast<int>(token.length), token.start);

        std::cout << token.type << '\n' << std::string(token.start) << '\n';

        if (token.type == TokenType::EOF_T) break;

        break;
    }
}