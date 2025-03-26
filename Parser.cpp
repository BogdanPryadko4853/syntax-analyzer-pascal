#include "Parser.h"
#include <sstream>


Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

Token Parser::currentToken() {
    if (current < tokens.size())
        return tokens[current];
    return Token(TokenType::END_OF_FILE, "");
}

Token Parser::consume(TokenType expected, const std::string& errorMessage) {
    if (currentToken().type == expected) {
        return tokens[current++];
    }

    Token token = currentToken();

    std::ostringstream oss;
    oss << "Ошибка в строке " << token.line
        <<  "\n"
        << "Найден токен: '" << token.lexeme << "'";

    throw std::runtime_error(oss.str());
}

bool Parser::match(TokenType type) {
    if (currentToken().type == type) {
        current++;
        return true;
    }
    return false;
}

