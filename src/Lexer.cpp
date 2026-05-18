#include "lexer.h"
#include<iostream>
// constructor
Lexer::Lexer(const std::string& source) : source(source) {};

// tokenisation starts after this function is called
std::vector<Token> Lexer::lex(){
    set_length();
    while(! Lexer::endOfFile()){
        Lexer::scan();
    }
    addToken(TokenType::EndOfFile);
    return tokens;
}

// scan the source for tokens
void Lexer::scan(){
    char c = advance();
    switch (c)
    {
    case '(':
        addToken(TokenType::LeftParenthesis);
        break;
    case ')':
        addToken(TokenType::RightParenthesis);
        break;
    case '"':
        addToken(TokenType::Quote);
        break;
    case '+':
        addToken(TokenType::Plus);
        break;
    case '-':
        addToken(TokenType::Minus);
        break;
    case '*':
        addToken(TokenType::Mul);
        break;
    case '/':
        if(match('/')){
            while(!endOfFile() && peek() != '\n') current++;
        }
        else addToken(TokenType::Div);
        break;
    case '=':
        addToken(match('=')? TokenType::Equal : TokenType::Assign);
        break;
    case '<':
        addToken(TokenType::Lesser);
        break;
    case '>':
        addToken(TokenType::Greater);
        break;
    case '|':
        addToken(TokenType::LogicalOr);
        break;
    case '&':
        addToken(TokenType::LogicalAnd);
        break;
    case '!':
        if(match('=')){
            addToken(TokenType::NotEqual);
        }
        else{
            addToken(TokenType::LogicalNot);
        }
        break;
    case ';':
        addToken(TokenType::EndStmt);
        break;
    case ' ':
        break; 
    case '\n':
        line ++;
        break;
    default:
        if(isDigit(c)) addNumber(c);
        else if(isalpha(c) || c == '_' || c == '\\') ident(c);
        else throwError("Unexpected character");
        break;
    }
}
void Lexer::set_length(){
    source_length = static_cast<int> (source.size());
}

// Check if we reached end of file
bool Lexer::endOfFile(){
    return (current >= source_length); 
}

bool Lexer::match(char c){
    if(endOfFile()) return false;
    else if(peek() != c) return false;
    current ++;
    return true;
}

// read the current character and move cursor ahead
char Lexer::advance(){
    return source[current++];
}

//read the current character and keep cursor where it is
char Lexer::peek(){
    if(endOfFile()){
        return '\0';
    }
    return source[current];
}

// add a token to our vector of tokens
void Lexer::addToken(TokenType t){
    tokens.push_back(Token(t, line));
}

// add a token to our vector of tokens
void Lexer::addToken(TokenType t, std::string val){
    tokens.push_back(Token(t, val, line));
}
bool Lexer::isDigit(char c){
    return c >= '0' && c <= '9';
}
bool Lexer::isAlpha(char c){
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'));
}
bool Lexer::isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c);
}
void Lexer::addNumber(char startdigit){
    std::string num = "";
    num += startdigit;
    while(isDigit(peek())){
        num += peek();
        current++;
    }
    addToken(TokenType::Number, num);
}
void Lexer::ident(char startchar){
    std::string s = "";
    s += startchar;
    while(isAlphaNumeric(peek())){
        s += peek();
        current++;
    }
    auto it = keywords.find(s);
    if(it == keywords.end()){
        addToken(TokenType::Identifier, s);
    }
    else{
        TokenType t = keywords[s];
        addToken(t, s);
    }
}