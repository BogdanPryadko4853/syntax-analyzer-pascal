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

Token Lexer::getNextToken() {
    skipWhitespace();

    size_t line = currentLine;

    if (pos >= length) {
        return Token(TokenType::END_OF_FILE, "", 0.0, line);
    }

    char c = currentChar();

    if (c == '\'') {
        advance();
        size_t start = pos;
        while (currentChar() != '\'' && currentChar() != '\0') {
            advance();
        }
        std::string strVal = input.substr(start, pos - start);
        if (currentChar() == '\'') {
            advance();
        }
        return Token(TokenType::STRING_LITERAL, strVal, 0.0, line);
    }

    if (std::isdigit(c)) {
        return number();
    }

    if (std::isalpha(c) || c == '_') {
        return identifier();
    }

    if (c == '<') {
        advance();
        if (currentChar() == '=') {
            advance();
            return Token(TokenType::LE, "<=", 0.0, line);
        } else if (currentChar() == '>') {
            advance();
            return Token(TokenType::NE, "<>", 0.0, line);
        } else {
            return Token(TokenType::LT, "<", 0.0, line);
        }
    }
    if (c == '>') {
        advance();
        if (currentChar() == '=') {
            advance();
            return Token(TokenType::GE, ">=", 0.0, line);
        } else {
            return Token(TokenType::GT, ">", 0.0, line);
        }
    }
    if (c == ':') {
        advance();
        if (currentChar() == '=') {
            advance();
            return Token(TokenType::ASSIGN, ":=", 0.0, line);
        } else {
            return Token(TokenType::COLON, ":", 0.0, line);
        }
    }

    switch(c) {
        case '+': advance(); return Token(TokenType::PLUS, "+", 0.0, line);
        case '-': advance(); return Token(TokenType::MINUS, "-", 0.0, line);
        case '*': advance(); return Token(TokenType::TIMES, "*", 0.0, line);
        case '/': advance(); return Token(TokenType::DIVIDE, "/", 0.0, line);
        case '=': advance(); return Token(TokenType::EQ, "=", 0.0, line);
        case '(': advance(); return Token(TokenType::LPAREN, "(", 0.0, line);
        case ')': advance(); return Token(TokenType::RPAREN, ")", 0.0, line);
        case '{': advance(); return Token(TokenType::LBRACE, "{", 0.0, line);
        case '}': advance(); return Token(TokenType::RBRACE, "}", 0.0, line);
        case ',': advance(); return Token(TokenType::COMMA, ",", 0.0, line);
        case ';': advance(); return Token(TokenType::SEMI, ";", 0.0, line);
        case '.': advance(); return Token(TokenType::DOT, ".", 0.0, line);
        default:
            advance();
            return Token(TokenType::UNKNOWN, std::string(1, c), 0.0, line);
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = getNextToken();
    while (token.type != TokenType::END_OF_FILE) {
        tokens.push_back(token);
        token = getNextToken();
    }
    tokens.push_back(token);
    return tokens;
}