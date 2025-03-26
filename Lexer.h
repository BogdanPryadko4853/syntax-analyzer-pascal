#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <stdexcept>
#include "BoundedDeque.h"

constexpr size_t LOOKAHEAD_BUFFER_SIZE = 5;

enum class TokenType {

    CONST, VAR, BEGIN, END, TEMPLATE, CLASS, TYPENAME,
    ASSERT, WHILE, IF, ELSE, DO, READLN, WRITELN, WRITE, THEN,

    PLUS, MINUS, TIMES, DIVIDE,
    EQ, NE, LT, GT, LE, GE,
    ASSIGN, COLON,

    LPAREN, RPAREN, LBRACE, RBRACE,
    COMMA, SEMI, DOT,

    NUMBER, STRING_LITERAL, IDENT,

    END_OF_FILE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string lexeme;
    double value;
    size_t line;

    Token(TokenType type, const std::string& lexeme, double value = 0.0,
          size_t line = 1)
            : type(type), lexeme(lexeme), value(value), line(line) {}
};

class Lexer {
public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;
    size_t length;
    size_t currentLine;
    BoundedDeque<char, LOOKAHEAD_BUFFER_SIZE> lookaheadBuffer;

    char currentChar();
    char peekAhead(size_t n = 1);
    void advance();
    void skipWhitespace();
    Token number();
    Token identifier();
    Token getNextToken();
    void fillLookahead(size_t n);
};

#endif