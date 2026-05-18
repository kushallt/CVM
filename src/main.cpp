#include "Lexer.h"
#include "Tokens.h"
#include "Parser.h"
#include "chunk.h"
#include "VirtualMachine.h"
#include "Compiler.h"
#include "Printer.h"
#include<vector>
#include<string>
#include<fstream>
#include<iostream>

// reading through the actual source file.
std::string readFile(std::string fileName){
    std::ifstream file(fileName);
    if(!file.is_open()){
        std::cout<<"Unable to open file.";
        return "";
    }
    std::string content(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    return content;
}

// main function - all the stages are merged here.
int main(){

    // Source
    std::string source;
    std::string fileName = "programs/program_code.cvm";
    source = readFile(fileName);

    //Lexing(Tokenisation)
    Lexer L = Lexer(source);
    std::vector<Token> LexedTokens = L.lex();

    // for(auto &t:LexedTokens){
    //     std::cout << int(t.type)<<" "<<t.val<<std::endl;
    // }
    
    //Parsing (Making the AST using Recursive descent)
    Parser P(LexedTokens);
    std::vector<std::unique_ptr<StmtAST>> FinalAST = P.parseProgram();
    Printer prin;
    // for(auto &stmt:FinalAST){
    //     stmt->accept(prin);
    // }

    //CodeGeneration
    Chunk chunk;
    chunk.initChunk();
    Compiler compiler(&chunk);
    for(auto &stmt:FinalAST){
        stmt->accept(compiler);
    }
    // chunk.writeChunk(static_cast<uint8_t>(Opcode::Op_Return), -1);
    // chunk.disassembleChunk("test chunk");
    // for(int i=0; i<chunk.count; i++){
    //     std::cout<<(int)chunk.code[i]<<std::endl;
    // }
    VM vm;
    HashTable variables;
    vm.resetStack();
    vm.interpret(&chunk, &variables);
    chunk.freeChunk();
    ObjString str;
    // str.chars = "x";
    // int index = variables.search(&str);
    // std::cout<<index<<" "<<AS_NUMBER(variables.entries[index].value)<<std::endl; 
    return 0;
}
