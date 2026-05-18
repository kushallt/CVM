#pragma once
#include "ASTNodes.h"
class Compiler : public Visitor{
public:
    Chunk *chunk;
    Compiler(Chunk* chunk) : chunk(chunk) {}
    void visitLiteral(Literal &node) override {
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_Constant), node.line);
        Value addVal;
        if(node.Dtype == TokenType::Number){
            addVal = NUMBER_VAL(stod(node.val));
        }
        else if(node.Dtype == TokenType::Boolean){
            addVal = BOOL_VAL(node.val != "False");
        }
        int ind = chunk->addConstant(addVal);
        chunk->writeChunk(ind, node.line);
    }
    void visitBinary(Binary &node) override{
        node.LHS->accept(*this);
        node.RHS->accept(*this);
        chunk->writeChunk(static_cast<uint8_t>(tokens_to_op[node.Op.type]), node.Op.line);
    }
    void visitUnary(Unary &node) override{
        node.OperandExp->accept(*this);
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_UnaryAdd), node.Op.line);

    }
    void visitIf(IfStmt &node) override{
        node.Condition->accept(*this);
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_JumpIfFalse), -1);

        chunk->writeChunk(0xff, -1);
        chunk->writeChunk(0xff, -1);
        int patchstart = chunk->count - 2;
        int startoffset = chunk->count;
        node.RunStmt->accept(*this);
        int endoffset = chunk->count;

        int jump = endoffset-startoffset;

        chunk->code[patchstart] = (jump >> 8) & 0xff;
        chunk->code[patchstart+1] = jump & 0xff;
    }
    void visitWhile(WhileStmt &node) override{
        int loopStart = chunk->count;

        node.Condition->accept(*this);
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_JumpIfFalse), -1);
        chunk->writeChunk(0xff, -1);
        chunk->writeChunk(0xff, -1);
        int patchstart = chunk->count - 2;
        int startoffset = chunk->count;
        node.RunStmt->accept(*this);

        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_Jump), -1);
        chunk->writeChunk(0xff, -1);
        chunk->writeChunk(0xff, -1);
        int backpatchstart = chunk->count - 2;
        int backpatch = chunk->count - loopStart;
        int jump = chunk->count - startoffset;

        chunk->code[backpatchstart] = (backpatch >> 8) & 0xff;
        chunk->code[backpatchstart+1] = backpatch & 0xff;

        chunk->code[patchstart] = (jump >> 8) & 0xff;
        chunk->code[patchstart+1] = jump & 0xff;

    }
    void visitAssignment(Assignment &node) override{
        node.value->accept(*this);
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_SetGlobal), node.line);
        ObjString *strobj = new ObjString();
        strobj->chars = node.var;
        int index = chunk->addConstant(STRING_VAL(strobj));
        chunk->writeChunk(index, node.line);
    }
    void visitVariable(Variable &node) override{
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_GetGlobal), node.line);
        ObjString* strobj = new ObjString();
        strobj->chars = node.varName;
        int index = chunk->addConstant(STRING_VAL(strobj));
        chunk->writeChunk(index, node.line);
    }
    void visitPrint(PrintStmt &node) override{
        if(node.sel == 0){
        
            chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_Constant), -1);
            ObjString *strobj = new ObjString();
            strobj->chars = node.printVal;
            int index = chunk->addConstant(STRING_VAL(strobj));
            chunk->writeChunk(index, -1);
        }
        else if(node.sel == 1){
            node.printVar->accept(*this);
        }
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_Print), -1);
    }
    void visitInput(InputStmt &node) override{
        auto p = PrintStmt(node.inputText, nullptr, 0);
        p.accept(*this);
        chunk->writeChunk(static_cast<uint8_t>(Opcode::Op_Input), -1);

    }
};