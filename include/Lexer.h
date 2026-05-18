#pragma once
#include<vector>
#include<string>
#include "Tokens.h"

// Lexer class
class Lexer{
    public:
        Lexer(const std::string &source);
        std::vector<Token> lex();
    private:
        std::string source;
        std::vector<Token> tokens;
        int source_length;
        int current = 0;
        int line = 1;
        void scan();
        void set_length();
        bool endOfFile();
        char advance();
        char peek();
        bool match(char c);
        bool isDigit(char c);
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        void addToken(TokenType t);
        void addToken(TokenType t, std::string val);
        void addNumber(char startdigit);
        void ident(char startchar);
        
};