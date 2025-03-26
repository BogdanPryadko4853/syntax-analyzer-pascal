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

    void print(int indent = 0) const {
        for (int i = 0; i < indent; i++) std::cout << "  ";
        std::cout << toString() << std::endl;
        for (const auto &child: children) {
            child->print(indent + 1);
        }
    }

    std::string toString() const {
        switch (type) {
            case ASTNodeType::Program:
                return "Program";
            case ASTNodeType::Block:
                return "Block";
            case ASTNodeType::ConstDecl:
                return "ConstDecl: " + value;
            case ASTNodeType::VarDecl:
                return "VarDecl: " + value;
            case ASTNodeType::TemplateDecl:
                return "TemplateDecl: " + value;
            case ASTNodeType::ClassDecl:
                return "ClassDecl: " + value;
            case ASTNodeType::StatementBlock:
                return "StatementBlock";
            case ASTNodeType::Assignment:
                return "Assignment: " + value;
            case ASTNodeType::IfStatement:
                return "IfStatement";
            case ASTNodeType::WhileStatement:
                return "WhileStatement";
            case ASTNodeType::ProcedureCall:
                return "ProcedureCall: " + value;
            case ASTNodeType::Expression:
                return "Expression: " + value;
            case ASTNodeType::Term:
                return "Term: " + value;
            case ASTNodeType::Factor:
                return "Factor: " + value;
            default:
                return "Unknown";
        }
    }
};

#endif
