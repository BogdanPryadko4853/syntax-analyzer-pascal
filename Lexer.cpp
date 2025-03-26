#include "Lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& input)
        : input(input), pos(0), length(input.size()), currentLine(1) {}

char Lexer::currentChar() {
    if (pos >= length) return '\0';
    return input[pos];
}

char Lexer::peekAhead(size_t n) {
    if (pos + n >= length) return '\0';
    return input[pos + n];
}

