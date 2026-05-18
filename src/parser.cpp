
#include "parser.h"

Parser::Parser(const std::vector<Token> &tokens) : tokens(tokens) {};

std::unique_ptr<ExpressionAST> Parser::expression()
{
    return assignment();
}
std::unique_ptr<ExpressionAST> Parser::assignment()
{
    std::unique_ptr<ExpressionAST> expr = equality();
    Variable *vcheck = dynamic_cast<Variable *>(expr.get());
    if (vcheck != nullptr && match({TokenType::Assign}))
    {
        std::unique_ptr<StmtAST> val;
        if(match({TokenType::Input})){
            val = inputStatement();
        }
        else val = expression();
        expr = std::make_unique<Assignment>(vcheck->varName, std::move(val), vcheck->line);
    }
    return expr;
}
std::unique_ptr<ExpressionAST> Parser::equality()
{
    auto expr = comparision();

    while (match({TokenType::Equal, TokenType::NotEqual}))
    {
        Token op = previous();
        expr = std::make_unique<Binary>(op, std::move(expr), comparision());
    }
    return expr;
}
std::unique_ptr<ExpressionAST> Parser::comparision()
{
    auto expr = term();
    while (match({TokenType::Lesser, TokenType::Greater}))
    {
        Token op = previous();
        expr = std::make_unique<Binary>(op, std::move(expr), term());
    }
    return expr;
}
std::unique_ptr<ExpressionAST> Parser::term()
{
    auto expr = factor();
    while (match({TokenType::Plus, TokenType::Minus}))
    {
        Token op = previous();
        expr = std::make_unique<Binary>(op, std::move(expr), factor());
    }
    return expr;
}
std::unique_ptr<ExpressionAST> Parser::factor()
{
    auto expr = unary();
    while (match({TokenType::Mul, TokenType::Div}))
    {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(op, std::move(expr), std::move(right));
    }
    return expr;
}
std::unique_ptr<ExpressionAST> Parser::unary()
{
    std::unique_ptr<ExpressionAST> expr;
    if (match({TokenType::LogicalNot}))
    {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<Unary>(op, std::move(right));
    }
    else
    {
        expr = primary();
    }

    return expr;
}

std::unique_ptr<ExpressionAST> Parser::primary()
{
    if (match({TokenType::Number}))
        return std::make_unique<Literal>(TokenType::Number, previous().val, previous().line);
    else if (match({TokenType::Boolean}))
        return std::make_unique<Literal>(TokenType::Boolean, previous().val, previous().line);
    else if (match({TokenType::Identifier}))
        return std::make_unique<Variable>(previous().val, previous().line);
    else if (match({TokenType::LeftParenthesis}))
    {
        auto expr = expression();
        match({TokenType::RightParenthesis});
        return expr;
    }
    else if (match({TokenType::EndStmt}))
    {
        return std::make_unique<Empty>();
    }
    else
    {
        return std::make_unique<Empty>();
    }
}

std::unique_ptr<StmtAST> Parser::ifStatement()
{
    if (match({TokenType::LeftParenthesis}))
    {
        auto cond = expression();
        std::unique_ptr<StmtAST> stmt;
        std::unique_ptr<StmtAST> ifnode;
        if (match({TokenType::RightParenthesis}))
        {
            stmt = parseStatement();
            ifnode = std::make_unique<IfStmt>(std::move(cond), std::move(stmt));
            match({TokenType::EndStmt});
            return ifnode;
        }
    }
    return std::make_unique<Empty>();
}
std::unique_ptr<StmtAST> Parser::whileStatement()
{
    if (match({TokenType::LeftParenthesis}))
    {
        auto cond = expression();
        std::unique_ptr<StmtAST> stmt;
        std::unique_ptr<StmtAST> whilenode;
        if (match({TokenType::RightParenthesis}))
        {
            stmt = parseStatement();
            whilenode = std::make_unique<WhileStmt>(std::move(cond), std::move(stmt));
            match({TokenType::EndStmt});
            return whilenode;
        }
    }
    return std::make_unique<Empty>();
}

std::unique_ptr<StmtAST> Parser::printStatement()
{
    std::unique_ptr<StmtAST> printNode;
    if (match({TokenType::Quote}))
    {
        std::string pVal = "";
        while (!match({TokenType::Quote}))
        {
            if (pVal != "") pVal += " ";
            if (peek().type == TokenType::Identifier){
                if(peek().val == "\\n") pVal += '\n';
                else pVal += peek().val;
            }
            else{

                if(peek().val == "") pVal += tokenTypeToString(peek().type);
                else pVal += peek().val;
            }
            advance();
        }
        printNode = std::make_unique<PrintStmt>(pVal, nullptr, 0);
    }
    else
    {
        std::unique_ptr<StmtAST> pVar = expression();
        printNode = std::make_unique<PrintStmt>("", std::move(pVar), 1);
        advance();
    }
    match({TokenType::EndStmt});
    return printNode;
}
std::unique_ptr<StmtAST> Parser::inputStatement()
{
    std::unique_ptr<StmtAST> inputNode;
    if (match({TokenType::Quote}))
    {
        std::string inputVal = "";
        while (!match({TokenType::Quote}))
        {
            if (peek().type == TokenType::Identifier)
                inputVal += peek().val;
            else
                inputVal += tokenTypeToString(peek().type);
            advance();
            inputVal += " ";
        }
        inputNode = std::make_unique<InputStmt>(inputVal);
    }
    match({TokenType::EndStmt});
    return inputNode;
}
std::unique_ptr<StmtAST> Parser::parseStatement()
{
    std::unique_ptr<StmtAST> newnode;
    if (match({TokenType::If}))
        newnode = ifStatement();
    else if (match({TokenType::While}))
        newnode = whileStatement();
    else if (match({TokenType::Print}))
    {
        newnode = printStatement();
    }
    else if (match({TokenType::Input}))
        newnode = inputStatement();
    else newnode = expression();
    return newnode;
}
std::vector<std::unique_ptr<StmtAST>> Parser::parseProgram()

{
    std::unique_ptr<StmtAST> newnode;
    while (!isAtEnd())
    {
        newnode = parseStatement();
        addNode(std::move(newnode));
        // if(match({TokenType::Identifier})) ast.push_back(varAssign());
    }
    return std::move(ast);
}

bool Parser::match(std::initializer_list<TokenType> types)
{
    for (auto type : types)
    {
        if (check(type))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType t)
{
    if (!isAtEnd())
    {
        return peek().type == t;
    }
    return false;
}
void Parser::advance()
{
    current++;
}
Token Parser::peek()
{
    return tokens[current];
}
Token Parser::previous()
{
    if (current > 0)
        return tokens[current - 1];
    return Token(TokenType::Default, -1);
}
bool Parser::isAtEnd()
{
    return peek().type == TokenType::EndOfFile;
}
void Parser::addNode(std::unique_ptr<StmtAST> p)
{
    if (typeid(*p) != typeid(Empty))
    {
        ast.push_back(std::move(p));
    }
}