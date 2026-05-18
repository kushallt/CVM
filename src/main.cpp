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

    // uncomment the below part to see the integer values of type of tokens generated
    // for(auto &t:LexedTokens){
    //     std::cout << int(t.type)<<" "<<t.val<<std::endl;
    // }
    
    //Parsing (Making the AST using Recursive descent)
    Parser P(LexedTokens);
    std::vector<std::unique_ptr<StmtAST>> FinalAST = P.parseProgram();

    // uncomment the below part to see the ast nodes.
    // Printer prin;
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
    chunk.writeChunk(static_cast<uint8_t>(Opcode::Op_Return), -1); // the last instruction. informs the vm that code is complete.

    //uncomment the below part to see the bytecode generated
    // chunk.disassembleChunk("test chunk");

    //uncomment the below part to see the integer value of opcodes and index values in the chunk
    // for(int i=0; i<chunk.count; i++){
    //     std::cout<<(int)chunk.code[i]<<std::endl;
    // }

    // the VM which actually runs the compiled bytecode.
    VM vm;
    HashTable variables; // this hash table stores the global variables
    vm.resetStack();
    vm.interpret(&chunk, &variables);
    chunk.freeChunk();  // after everything is done, free the memory.
    variables.free_table(); // free the memory.

    return 0;
}
