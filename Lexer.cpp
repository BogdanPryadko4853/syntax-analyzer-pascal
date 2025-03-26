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

Token Lexer::identifier() {
    size_t line = currentLine;
    size_t start = pos;

    while (std::isalnum(currentChar()) || currentChar() == '_') {
        advance();
    }

    std::string idStr = input.substr(start, pos - start);

    if (idStr == "const") return Token(TokenType::CONST, idStr, 0.0, line);
    if (idStr == "var") return Token(TokenType::VAR, idStr, 0.0, line);
    if (idStr == "begin") return Token(TokenType::BEGIN, idStr, 0.0, line);
    if (idStr == "end") return Token(TokenType::END, idStr, 0.0, line);
    if (idStr == "template") return Token(TokenType::TEMPLATE, idStr, 0.0, line);
    if (idStr == "class") return Token(TokenType::CLASS, idStr, 0.0, line);
    if (idStr == "typename") return Token(TokenType::TYPENAME, idStr, 0.0, line);
    if (idStr == "assert") return Token(TokenType::ASSERT, idStr, 0.0, line);
    if (idStr == "while") return Token(TokenType::WHILE, idStr, 0.0, line);
    if (idStr == "if") return Token(TokenType::IF, idStr, 0.0, line);
    if (idStr == "else") return Token(TokenType::ELSE, idStr, 0.0, line);
    if (idStr == "do") return Token(TokenType::DO, idStr, 0.0, line);
    if (idStr == "readln") return Token(TokenType::READLN, idStr, 0.0, line);
    if (idStr == "writeln") return Token(TokenType::WRITELN, idStr, 0.0, line);
    if (idStr == "write") return Token(TokenType::WRITE, idStr, 0.0, line);
    if (idStr == "then") return Token(TokenType::THEN, idStr, 0.0, line);
    if (idStr == "real") return Token(TokenType::IDENT, idStr, 0.0, line);

    return Token(TokenType::IDENT, idStr, 0.0, line);
}
