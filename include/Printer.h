#pragma once
#include "ASTNodes.h"

class Printer : public Visitor
{
public:
    void visitLiteral(Literal &node) override { std::cout << node.val; }
    void visitBinary(Binary &node) override
    {
        std::cout << "Binary(";
        node.LHS->accept(*this);
        std::cout << singleCharTokens[node.Op.type];
        node.RHS->accept(*this);
        std::cout << ")";
    }
    void visitUnary(Unary &node) override
    {
        std::cout << "Unary(" << singleCharTokens[node.Op.type];
        node.OperandExp->accept(*this);
        std::cout << ")";
    }
    void visitIf(IfStmt &node) override
    {
        std::cout<< "If(";
        node.Condition->accept(*this);
        std::cout<<")";
        node.RunStmt->accept(*this);
        std::cout<<";";
    }
    void visitVariable(Variable &node) override
    {
        std::cout<<node.varName<<"=";
    }
    void visitAssignment(Assignment& node) override {
        std::cout << "Assignment("<<node.var;
        std::cout << "=";
        node.value->accept(*this);
        std::cout << ")";
    }
    void visitWhile(WhileStmt &node) override
    {
        std::cout<< "If(";
        node.Condition->accept(*this);
        std::cout<<")";
        node.RunStmt->accept(*this);
        std::cout<<";";
    }
    void visitPrint(PrintStmt &node) override
    {
        std::cout<<"print "<<node.printVal<<std::endl; 
    }
    void visitInput(InputStmt &node) override
    {
        std::cout<<"input"<<node.inputText<<std::endl;
    }
};