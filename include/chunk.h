#pragma once
#include "commons.h"
#include "memory.h"
#include <iostream>
#include "ValueArray.h"
#include "Tokens.h"
#include<string>
enum class Opcode : uint8_t{
    // Constant
    Op_Constant,

    // Print
    Op_Return,

    // Variables
    Op_SetGlobal,
    Op_GetGlobal,

    // Binary Operators
    Op_Add,
    Op_Sub,
    Op_Mul,
    Op_Div,
    Op_Equality,
    Op_Greater,
    Op_Lesser,
    // Unary Operators
    Op_UnaryAdd,

    // Unary Operators
    Op_Not,

    Op_Print,
    Op_Input,

    Op_JumpIfFalse,
    Op_Jump,
};

inline std::unordered_map<TokenType, Opcode> tokens_to_op = {
    {TokenType::Plus, Opcode::Op_Add},
    {TokenType::Minus, Opcode::Op_Sub},
    {TokenType::Mul, Opcode::Op_Mul},
    {TokenType::Div, Opcode::Op_Div},
    {TokenType::Equal, Opcode::Op_Equality},
    {TokenType::Lesser, Opcode::Op_Lesser},
    {TokenType::Greater, Opcode::Op_Greater},
    {TokenType::LogicalNot, Opcode::Op_Not},

};

class Chunk{
public:
    int capacity;
    int count;
    int *lines;
    uint8_t *code;
    ValueArray constants;
    void initChunk(){
        capacity = 0;
        count = 0;
        code = NULL;
        lines = NULL;
        constants.initArr();
    }
    void writeChunk(uint8_t byte, int line){
        if(capacity < count + 1){
            int oldcapacity = capacity;
            growCapacity();
            code = GROW_ARRAY(uint8_t, code, oldcapacity, capacity);
            lines = GROW_ARRAY(int, lines, oldcapacity, capacity);
        }
        lines[count] = line;
        code[count++] = byte;
    }
    void freeChunk(){
    
    FREE_ARRAY(uint8_t, code, capacity);
    
    FREE_ARRAY(int, lines, capacity);
    constants.freeArr();
    initChunk();
}
    void growCapacity(){
        if(capacity > 0)capacity = capacity*2;
        else capacity = 9;
    }

    // Debugging
    void disassembleChunk(std::string name) {
        std::cout<<"=="<< name<< "==\n";  
        for (int offset = 0; offset < count;) {
        offset = disassembleInstruction(offset);
        }
    }
    int disassembleInstruction(int offset){
        uint8_t inst = code[offset];
        switch ((Opcode)inst)
        {
        case(Opcode::Op_Return):
            offset = simpleInstruction("Op_Return", offset);
            break;
        case(Opcode::Op_Add):
            offset = simpleInstruction("Add", offset);
            break;
        case(Opcode::Op_Sub):
            offset = simpleInstruction("Sub", offset);
            break;
        case(Opcode::Op_Mul):
            offset = simpleInstruction("Mul", offset);
            break;
        case(Opcode::Op_Div):
            offset = simpleInstruction("Div", offset);
            break;
        case(Opcode::Op_Equality):
            offset = simpleInstruction("Equality", offset);
            break;
        case(Opcode::Op_Lesser):
            offset = simpleInstruction("Lesser", offset);
            break;
        case(Opcode::Op_Greater):
            offset = simpleInstruction("Greater", offset);
            break;
        case(Opcode::Op_Print):
            offset = simpleInstruction("Print", offset);
            break;
        case(Opcode::Op_Input):
            offset = simpleInstruction("Input", offset);
            break;
        case(Opcode::Op_Constant):
            offset = constantInstruction("Op_Constant", offset);
            break;
        case(Opcode::Op_SetGlobal):
            offset = constantInstruction("SetGlobal", offset);
            break;
        case(Opcode::Op_GetGlobal):
            offset = constantInstruction("GetGlobal", offset);
            break;
        case(Opcode::Op_JumpIfFalse):
            offset = jumpInstruction("JumpIfFalse", offset);
            break;
        case(Opcode::Op_Jump):
            offset = jumpInstruction("Jump", offset);
            break;
        default:
            std::cout<<"Unknown Instruction"<<std::endl;
            offset++;
            break;
        }
        return offset;
    }
    static int simpleInstruction(std::string name, int offset){
        std::cout<<name<<std::endl;
        return ++offset;
    }
    int constantInstruction(std::string name, int offset){
        uint8_t constant = code[++offset];
        std::cout<<name<<" "<<(int)constant<<std::endl;
        constants.printValueArr((int)constant);
        return ++offset;
    }
    int jumpInstruction(std::string name, int offset){
        std::cout<<name<<std::endl;
        return offset + 3;
    }
    int addConstant(Value v){
        constants.writeArr(v);
        return constants.count - 1;
    }
};

