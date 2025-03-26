#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

enum class ASTNodeType {
    Program,
    Block,
    ConstDecl,
    VarDecl,
    TemplateDecl,
    ClassDecl,
    StatementBlock,
    Assignment,
    IfStatement,
    WhileStatement,
    ProcedureCall,
    Expression,
    Term,
    Factor,
    Unknown
};

struct ASTNode {
    ASTNodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(ASTNodeType type, const std::string &value = "")
            : type(type), value(value) {}

    void addChild(const std::shared_ptr<ASTNode> &child) {
        children.push_back(child);
    }




};

#endif
