#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include <vector>
#include <memory>
#include <string>
#include "Lexer.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parse();

private:
    std::vector<Token> tokens;
    size_t current;

    Token currentToken();
    Token consume(TokenType expected, const std::string& errorMessage);
    bool match(TokenType type);

    std::shared_ptr<ASTNode> parseProgram();
    std::shared_ptr<ASTNode> parseBlock();
    std::shared_ptr<ASTNode> parseConstDecl();
    std::shared_ptr<ASTNode> parseVarDecl();
    std::shared_ptr<ASTNode> parseLocalVarDecl();
    std::shared_ptr<ASTNode> parseTemplateDecl();
    std::shared_ptr<ASTNode> parseStatementBlock();
    std::shared_ptr<ASTNode> parseStatement();
    std::shared_ptr<ASTNode> parseAssignment();
    std::shared_ptr<ASTNode> parseIfStatement();
    std::shared_ptr<ASTNode> parseWhileStatement();
    std::shared_ptr<ASTNode> parseProcedureCall();
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseTerm();
    std::shared_ptr<ASTNode> parseFactor();
    std::shared_ptr<ASTNode> parseAdditive();
};

#endif
