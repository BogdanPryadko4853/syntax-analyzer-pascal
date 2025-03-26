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

void Lexer::advance() {
    if (currentChar() == '\n') {
        currentLine++;
    }
    if (!lookaheadBuffer.empty()) {
        lookaheadBuffer.pop_front();
    }
    pos++;
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar())) {
        advance();
    }
}

Token Lexer::number() {
    size_t line = currentLine;
    size_t start = pos;
    bool hasDot = false;

    while (std::isdigit(currentChar()) || currentChar() == '.') {
        if (currentChar() == '.') {
            if (hasDot) break;
            hasDot = true;
        }
        advance();
    }

    std::string numStr = input.substr(start, pos - start);
    double value = std::stod(numStr);
    return Token(TokenType::NUMBER, numStr, value, line);
}
