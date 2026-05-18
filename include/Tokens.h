#pragma once
#include<string>
#include<unordered_map>
#include "error.h"

// All TokenTypes
enum class TokenType{
    Default, // Default
    Identifier, // Identifier
    If, Else, While, Let, Input, Print, // Keywords
    Number, Boolean, // Literals
    Plus, Minus, Mul, Div, Lesser, Greater, Equal, NotEqual, Assign, LogicalNot, LogicalAnd, LogicalOr,// Operator
    RightParenthesis, LeftParenthesis, // Parenthesis
    Quote,
    EndStmt,
    EndOfFile // EOF
};

// Token Struct
struct Token{
    TokenType type = TokenType::Default;
    std::string val;
    int line;
    Token(TokenType type, int line) : type(type), val(""), line(line){}
    Token(TokenType type, std::string val, int line) : type(type), val(val), line(line){}
};

inline std::string tokenTypeToString(TokenType type){
    switch(type){
        case TokenType::Default:          return "Default";
        case TokenType::If:               return "If";
        case TokenType::Else:             return "Else";
        case TokenType::While:            return "While";
        case TokenType::Let:              return "Let";
        case TokenType::Input:            return "Input";
        case TokenType::Print:            return "Print";
        case TokenType::Number:           return "Number";
        case TokenType::Boolean:          return "Boolean";
        case TokenType::Plus:             return "+";
        case TokenType::Minus:            return "-";
        case TokenType::Mul:              return "*";
        case TokenType::Div:              return "/";
        case TokenType::Lesser:           return "<";
        case TokenType::Greater:          return ">";
        case TokenType::Equal:            return "==";
        case TokenType::NotEqual:         return "!=";
        case TokenType::Assign:           return "=";
        case TokenType::LogicalNot:       return "!";
        case TokenType::LogicalAnd:       return "&&";
        case TokenType::LogicalOr:        return "||";
        case TokenType::RightParenthesis: return ")";
        case TokenType::LeftParenthesis:  return "(";
        case TokenType::Quote:            return "\"";
        case TokenType::EndStmt:          return ";";
        case TokenType::EndOfFile:        return "EOF";
        default:                          return "Unknown";
    }
}
// matching strings with tokenTypes for lexing
inline std::unordered_map<TokenType, std::string> singleCharTokens = {
    {TokenType::Plus, "+"},
    {TokenType::Minus, "-"},
    {TokenType::Mul, "*"},
    {TokenType::Div, "/"},
    {TokenType::Greater, ">"},
    {TokenType::Lesser, "<"},
    {TokenType::Assign, "="},
    {TokenType::LeftParenthesis, "("},
    {TokenType::RightParenthesis, ")"},
    {TokenType::LogicalNot, "!"},
    {TokenType::LogicalOr, "|"},
    {TokenType::LogicalAnd, "&"}
};

inline std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"while", TokenType::While},
    {"let", TokenType::Let},
    {"input", TokenType::Input},
    {"print", TokenType::Print},
    {"true", TokenType::Boolean},
    {"false", TokenType::Boolean},
};