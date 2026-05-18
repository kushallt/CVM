#pragma once
#include "Tokens.h"
#include "ASTNodes.h"
#include<vector>
#include<string>

class Parser{
    public:
        Parser(const std::vector<Token>& tokens);
        std::vector<std::unique_ptr<StmtAST>> parseProgram();
    private:
        std::vector<Token> tokens;
        std::vector<std::unique_ptr<StmtAST>>ast;
        int current = 0;

        std::unique_ptr<ExpressionAST> expression();
        std::unique_ptr<ExpressionAST> assignment();
        std::unique_ptr<ExpressionAST> equality();
        std::unique_ptr<ExpressionAST> comparision();
        std::unique_ptr<ExpressionAST> term();
        std::unique_ptr<ExpressionAST> factor();
        std::unique_ptr<ExpressionAST> unary();
        std::unique_ptr<ExpressionAST> primary();

        
        std::unique_ptr<StmtAST> ifStatement();
        std::unique_ptr<StmtAST> whileStatement();
        std::unique_ptr<StmtAST> printStatement();
        std::unique_ptr<StmtAST> inputStatement();
        std::unique_ptr<StmtAST> varDecl();
        std::unique_ptr<StmtAST> varAssign();

        std::unique_ptr<StmtAST> parseStatement();

        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType t);
        void advance();
        Token peek();
        Token previous();
        bool isAtEnd();
        void addNode(std::unique_ptr<StmtAST>);


};