#pragma once
#include "Tokens.h"
#include "chunk.h"
#include <memory>
#include <iostream>
#include <utility>
class Binary;
class Unary;
class Literal;
class IfStmt;
class WhileStmt;
class Variable;
class Assignment;
class PrintStmt;
class InputStmt;

// visitor abstract class for visitor pattern.
class Visitor
{
public:
    virtual void visitBinary(Binary &node) = 0;
    virtual void visitUnary(Unary &node) = 0;
    virtual void visitLiteral(Literal &node) = 0;
    virtual void visitIf(IfStmt &node) = 0;
    virtual void visitWhile(WhileStmt &node) = 0;
    virtual void visitVariable(Variable &node) = 0;
    virtual void visitAssignment(Assignment &node) = 0;
    virtual void visitPrint(PrintStmt &node) = 0;
    virtual void visitInput(InputStmt &node) = 0;
    virtual ~Visitor() = default;
};

class StmtAST
{
public:
    virtual void accept(Visitor &v) = 0; // the accept function for visitor pattern
    virtual ~StmtAST() = default;
};

class ExpressionAST : public StmtAST
{
public:
    virtual ~ExpressionAST() = default;
};

// -------------------- Expressions --------------------

class Binary : public ExpressionAST
{
public:
    Token Op;
    std::unique_ptr<ExpressionAST> LHS;
    std::unique_ptr<ExpressionAST> RHS;

    Binary(Token op,
           std::unique_ptr<ExpressionAST> lhs,
           std::unique_ptr<ExpressionAST> rhs)
        : Op(std::move(op)), LHS(std::move(lhs)), RHS(std::move(rhs)) {}

    void accept(Visitor &v) override { v.visitBinary(*this); }
};

class Unary : public ExpressionAST
{
public:
    Token Op;
    std::unique_ptr<ExpressionAST> OperandExp;

    Unary(Token op,
          std::unique_ptr<ExpressionAST> operand)
        : Op(std::move(op)), OperandExp(std::move(operand)) {}
    void accept(Visitor &v) override { v.visitUnary(*this); }
};

class Literal : public ExpressionAST
{
public:
    TokenType Dtype;
    std::string val;
    int line;
    Literal(TokenType dtype, std::string value, int line)
        : Dtype(dtype), val(std::move(value)), line(line) {}
    void accept(Visitor &v) override { v.visitLiteral(*this); }
};

class Variable : public ExpressionAST
{
public:
    std::string varName;
    int line;
    Variable(std::string name, int line)
        : varName(std::move(name)), line(line) {}
    void accept(Visitor &v) override {v.visitVariable(*this);}
};

class Assignment : public ExpressionAST
{
public:
    std::string var;
    std::unique_ptr<StmtAST> value;
    int line;
    Assignment(std::string variable, std::unique_ptr<StmtAST> assignvalue, int aline) 
        : var(std::move(variable)), value(std::move(assignvalue)), line(aline){}
    void accept(Visitor &v) override {v.visitAssignment(*this);}
};

class Empty : public ExpressionAST
{
    public:
    void accept(Visitor &v) override{(void)v;}
};

// -------------------- Statements --------------------

class IfStmt : public StmtAST
{
public:
    std::unique_ptr<ExpressionAST> Condition;
    std::unique_ptr<StmtAST> RunStmt;  
    IfStmt(std::unique_ptr<ExpressionAST> condition,
           std::unique_ptr<StmtAST> runStmt)
        : Condition(std::move(condition)), RunStmt(std::move(runStmt)) {}
    void accept(Visitor &v) override {v.visitIf(*this);}
};

class WhileStmt : public StmtAST
{
public:
    std::unique_ptr<ExpressionAST> Condition;
    std::unique_ptr<StmtAST> RunStmt;

    WhileStmt(std::unique_ptr<ExpressionAST> condition,
              std::unique_ptr<StmtAST> runStmt)
        : Condition(std::move(condition)), RunStmt(std::move(runStmt)) {}
    void accept(Visitor &v) override {v.visitWhile(*this);}
};
class InputStmt : public StmtAST{
public:
    std::string inputText = "";
    InputStmt(std::string itext)
        : inputText(itext) {}
    void accept(Visitor &v) override {v.visitInput(*this);}
};
class PrintStmt : public StmtAST
{
public:
    std::string printVal = "";
    std::unique_ptr<StmtAST> printVar = nullptr;
    int sel = 0;
    PrintStmt(std::string pVal, std::unique_ptr<StmtAST> pVar, int select)
        : printVal(pVal), printVar(std::move(pVar)), sel(select) {}  
    void accept(Visitor &v) override {v.visitPrint(*this);}
};

class DeclStmt : public StmtAST
{
public:
    std::string VarName;
    std::unique_ptr<ExpressionAST> VarVal;

    DeclStmt(std::string name,
             std::unique_ptr<ExpressionAST> val)
        : VarName(std::move(name)), VarVal(std::move(val)) {}
    void accept(Visitor &v) override {(void)v;}
};

class AssignStmt : public StmtAST
{
public:
    std::string VarName;
    std::unique_ptr<ExpressionAST> VarVal;

    AssignStmt(std::string name,
               std::unique_ptr<ExpressionAST> val)
        : VarName(std::move(name)), VarVal(std::move(val)) {}
    void accept(Visitor &v) override {(void)v;}
};





