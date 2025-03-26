#include "Parser.h"
#include <sstream>


Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens), current(0) {}

Token Parser::currentToken() {
    if (current < tokens.size())
        return tokens[current];
    return Token(TokenType::END_OF_FILE, "");
}

Token Parser::consume(TokenType expected, const std::string &errorMessage) {
    if (currentToken().type == expected) {
        return tokens[current++];
    }

    Token token = currentToken();

    std::ostringstream oss;
    oss << "Ошибка в строке " << token.line
        << "\n"
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

std::shared_ptr<ASTNode> Parser::parse() {
    return parseProgram();
}

std::shared_ptr<ASTNode> Parser::parseProgram() {
    auto node = std::make_shared<ASTNode>(ASTNodeType::Program);
    node->addChild(parseBlock());
    consume(TokenType::DOT, "Ожидалась точка ('.') в конце программы.");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseBlock() {
    auto node = std::make_shared<ASTNode>(ASTNodeType::Block);
    if (currentToken().type == TokenType::CONST) {
        node->addChild(parseConstDecl());
    }
    if (currentToken().type == TokenType::VAR) {

        if (tokens[current + 1].type != TokenType::IDENT || tokens[current + 2].type != TokenType::ASSIGN)
            node->addChild(parseVarDecl());
        else
            node->addChild(parseLocalVarDecl());
    }
    if (currentToken().type == TokenType::TEMPLATE) {
        node->addChild(parseTemplateDecl());
    }
    node->addChild(parseStatementBlock());
    return node;
}


std::shared_ptr<ASTNode> Parser::parseConstDecl() {
    consume(TokenType::CONST, "Ожидалось 'const'");
    auto node = std::make_shared<ASTNode>(ASTNodeType::ConstDecl);
    do {
        Token id = consume(TokenType::IDENT, "Ожидался идентификатор в объявлении константы.");
        consume(TokenType::EQ, "Ожидался '=' в объявлении константы.");
        Token num = consume(TokenType::NUMBER, "Ожидалось число в объявлении константы.");
        consume(TokenType::SEMI, "Ожидалась ';' после объявления константы.");
        auto constNode = std::make_shared<ASTNode>(ASTNodeType::ConstDecl,
                                                   id.lexeme + " = " + std::to_string(num.value));
        node->addChild(constNode);
    } while (currentToken().type == TokenType::IDENT);
    return node;
}

std::shared_ptr<ASTNode> Parser::parseVarDecl() {
    consume(TokenType::VAR, "Ожидалось 'var'");
    auto node = std::make_shared<ASTNode>(ASTNodeType::VarDecl);
    Token id = consume(TokenType::IDENT, "Ожидался идентификатор в объявлении переменной.");
    node->value += id.lexeme;
    while (currentToken().type == TokenType::COMMA) {
        consume(TokenType::COMMA, "Ожидалась ',' между идентификаторами.");
        Token nextId = consume(TokenType::IDENT, "Ожидался идентификатор после ','.");
        node->value += ", " + nextId.lexeme;
    }

    if (currentToken().type == TokenType::COLON) {
        consume(TokenType::COLON, "Ожидался ':' для указания типа");
        Token typeId = consume(TokenType::IDENT, "Ожидался идентификатор типа");
        node->value += " : " + typeId.lexeme;
    }
    consume(TokenType::SEMI, "Ожидалась ';' после объявления переменных.");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseLocalVarDecl() {
    consume(TokenType::VAR, "Ожидалось 'var' для локальной переменной");
    auto node = std::make_shared<ASTNode>(ASTNodeType::VarDecl);
    Token id = consume(TokenType::IDENT, "Ожидался идентификатор локальной переменной.");
    node->value += id.lexeme;
    while (currentToken().type == TokenType::COMMA) {
        consume(TokenType::COMMA, "Ожидалась ',' между идентификаторами.");
        Token nextId = consume(TokenType::IDENT, "Ожидался идентификатор локальной переменной.");
        node->value += ", " + nextId.lexeme;
    }
    if (currentToken().type == TokenType::COLON) {
        consume(TokenType::COLON, "Ожидался ':' для указания типа");
        Token typeId = consume(TokenType::IDENT, "Ожидался идентификатор типа");
        node->value += " : " + typeId.lexeme;
    }
    consume(TokenType::ASSIGN, "Ожидалось ':=' для инициализации локальной переменной");
    auto expr = parseExpression();
    node->addChild(expr);
    consume(TokenType::SEMI, "Ожидалась ';' после локального объявления переменной");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseTemplateDecl() {
    consume(TokenType::TEMPLATE, "Ожидалось 'template'");
    consume(TokenType::LT, "Ожидался символ '<' после 'template'");
    consume(TokenType::TYPENAME, "Ожидалось 'typename' в шаблоне");
    Token param = consume(TokenType::IDENT, "Ожидался идентификатор параметра шаблона");
    consume(TokenType::GT, "Ожидался символ '>' после параметра шаблона");
    auto node = std::make_shared<ASTNode>(ASTNodeType::TemplateDecl, param.lexeme);
    return node;
}

std::shared_ptr<ASTNode> Parser::parseStatementBlock() {
    consume(TokenType::BEGIN, "Ожидалось 'begin'");
    auto node = std::make_shared<ASTNode>(ASTNodeType::StatementBlock);
    while (currentToken().type != TokenType::END) {
        node->addChild(parseStatement());
    }
    consume(TokenType::END, "Ожидалось 'end' для закрытия блока операторов.");
    return node;
}

std::shared_ptr<ASTNode> Parser::parseStatement() {
    TokenType t = currentToken().type;

    if (t == TokenType::SEMI) {
        consume(TokenType::SEMI, "");
        return std::make_shared<ASTNode>(ASTNodeType::Unknown, "EmptyStatement");
    }

    if (t == TokenType::BEGIN) {
        return parseStatementBlock();
    } else if (t == TokenType::VAR) {
        if (tokens[current + 1].type == TokenType::IDENT && tokens[current + 2].type == TokenType::ASSIGN)
            return parseLocalVarDecl();
        else
            return parseVarDecl();
    } else if (t == TokenType::IF) {
        return parseIfStatement();
    } else if (t == TokenType::WHILE) {
        return parseWhileStatement();
    } else if (t == TokenType::WRITE || t == TokenType::WRITELN ||
               t == TokenType::READLN || t == TokenType::ASSERT) {
        return parseProcedureCall();
    } else if (t == TokenType::IDENT) {
        if (tokens[current + 1].type == TokenType::ASSIGN)
            return parseAssignment();
        else
            return parseProcedureCall();
    } else {
        throw std::runtime_error("Неожиданный токен в операторе: " + currentToken().lexeme);
    }
}

std::shared_ptr<ASTNode> Parser::parseAssignment() {
    Token id = consume(TokenType::IDENT, "Ожидался идентификатор в операторе присваивания");
    consume(TokenType::ASSIGN, "Ожидалось ':=' в операторе присваивания");
    auto exprNode = parseExpression();
    consume(TokenType::SEMI, "Ожидалась ';' после оператора присваивания");
    auto node = std::make_shared<ASTNode>(ASTNodeType::Assignment, id.lexeme);
    node->addChild(exprNode);
    return node;
}

std::shared_ptr<ASTNode> Parser::parseIfStatement() {
    consume(TokenType::IF, "Ожидалось 'if'");
    auto exprNode = parseExpression();
    consume(TokenType::THEN, "Ожидалось 'then' в операторе if");
    auto thenStmt = parseStatement();
    auto ifNode = std::make_shared<ASTNode>(ASTNodeType::IfStatement);
    ifNode->addChild(exprNode);
    ifNode->addChild(thenStmt);
    if (currentToken().type == TokenType::ELSE) {
        consume(TokenType::ELSE, "Ожидалось 'else' в операторе if");
        auto elseStmt = parseStatement();
        ifNode->addChild(elseStmt);
    }
    return ifNode;
}

std::shared_ptr<ASTNode> Parser::parseWhileStatement() {
    consume(TokenType::WHILE, "Ожидалось 'while'");
    auto exprNode = parseExpression();
    consume(TokenType::DO, "Ожидалось 'do' в цикле while");
    auto stmt = parseStatement();
    auto node = std::make_shared<ASTNode>(ASTNodeType::WhileStatement);
    node->addChild(exprNode);
    node->addChild(stmt);
    return node;
}

std::shared_ptr<ASTNode> Parser::parseProcedureCall() {
    Token proc = consume(currentToken().type, "Ожидался идентификатор или ключевое слово процедуры");
    consume(TokenType::LPAREN, "Ожидалось '(' в вызове процедуры");
    // Разбираем аргументы вызова (используем полное выражение, включающее реляционные операторы)
    while (currentToken().type != TokenType::RPAREN) {
        parseExpression();
        if (currentToken().type == TokenType::COMMA)
            consume(TokenType::COMMA, "Ожидалась ',' между аргументами");
        else
            break;
    }
    consume(TokenType::RPAREN, "Ожидалось ')' в вызове процедуры");
    consume(TokenType::SEMI, "Ожидалась ';' после вызова процедуры");
    auto node = std::make_shared<ASTNode>(ASTNodeType::ProcedureCall, proc.lexeme);
    return node;
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    auto node = parseAdditive();
    while (currentToken().type == TokenType::LT ||
           currentToken().type == TokenType::GT ||
           currentToken().type == TokenType::LE ||
           currentToken().type == TokenType::GE ||
           currentToken().type == TokenType::EQ ||
           currentToken().type == TokenType::NE) {
        Token op = currentToken();
        consume(op.type, "Ожидался оператор сравнения");
        auto right = parseAdditive();
        auto cmpNode = std::make_shared<ASTNode>(ASTNodeType::Expression, op.lexeme);
        cmpNode->addChild(node);
        cmpNode->addChild(right);
        node = cmpNode;
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseAdditive() {
    auto node = parseTerm();
    while (currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS) {
        Token op = currentToken();
        consume(op.type, "Ожидался оператор '+' или '-'");
        auto right = parseTerm();
        auto exprNode = std::make_shared<ASTNode>(ASTNodeType::Expression, op.lexeme);
        exprNode->addChild(node);
        exprNode->addChild(right);
        node = exprNode;
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseTerm() {
    auto node = parseFactor();
    while (currentToken().type == TokenType::TIMES || currentToken().type == TokenType::DIVIDE) {
        Token op = currentToken();
        consume(op.type, "Ожидался оператор '*' или '/'");
        auto right = parseFactor();
        auto termNode = std::make_shared<ASTNode>(ASTNodeType::Term, op.lexeme);
        termNode->addChild(node);
        termNode->addChild(right);
        node = termNode;
    }
    return node;
}

std::shared_ptr<ASTNode> Parser::parseFactor() {
    Token token = currentToken();
    if (token.type == TokenType::NUMBER) {
        consume(TokenType::NUMBER, "Ожидалось число");
        return std::make_shared<ASTNode>(ASTNodeType::Factor, token.lexeme);
    } else if (token.type == TokenType::STRING_LITERAL) {
        consume(TokenType::STRING_LITERAL, "Ожидался строковый литерал");
        return std::make_shared<ASTNode>(ASTNodeType::Factor, token.lexeme);
    } else if (token.type == TokenType::IDENT) {
        consume(TokenType::IDENT, "Ожидался идентификатор");
        // Если после идентификатора идёт открывающая скобка – это вызов функции
        if (currentToken().type == TokenType::LPAREN) {
            auto funcNode = std::make_shared<ASTNode>(ASTNodeType::Factor, token.lexeme);
            consume(TokenType::LPAREN, "Ожидалось '(' после идентификатора");
            while (currentToken().type != TokenType::RPAREN) {
                auto arg = parseExpression();
                funcNode->addChild(arg);
                if (currentToken().type == TokenType::COMMA)
                    consume(TokenType::COMMA, "Ожидалась ',' между аргументами");
                else
                    break;
            }
            consume(TokenType::RPAREN, "Ожидалось ')' в вызове функции");
            return funcNode;
        }
        return std::make_shared<ASTNode>(ASTNodeType::Factor, token.lexeme);
    } else if (token.type == TokenType::LPAREN) {
        consume(TokenType::LPAREN, "Ожидалось '('");
        auto node = parseExpression();
        consume(TokenType::RPAREN, "Ожидалось ')'");
        return node;
    } else {
        throw std::runtime_error("Неожиданный токен в выражении: " + token.lexeme);
    }
}
