#pragma once
#include "chunk.h"
#include "hashtable.h"
#define STACK_MAX 256
enum class InterpretResult
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VM
{
public:
    Chunk *chunk;
    uint8_t *ip;
    Value stack[STACK_MAX];
    Value *stackTop;
    HashTable *variables;
    InterpretResult interpret(Chunk *c, HashTable *t)
    {
        chunk = c;
        variables = t;
        ip = chunk->code;
        variables->init_table();
        return run();
    }
    InterpretResult run()
    {
#define READ_BYTE() (*(ip++))
#define READ_CONSTANT() ((chunk)->constants.values[READ_BYTE()])
#define BINARY_NUM_OP(op)                             \
    do                                                \
    {                                                 \
        Value b = pop();                              \
        Value a = pop();                              \
                                                      \
        push(NUMBER_VAL(a.as.number op b.as.number)); \
    } while (false)
#define BINARY_BOOL_OP(op)                          \
    do                                              \
    {                                               \
        Value b = pop();                            \
        Value a = pop();                            \
        push(BOOL_VAL(a.as.number op b.as.number)); \
    } while (false)
        for (;;)
        {
            Opcode instruction;
            switch (instruction = (Opcode)READ_BYTE())
            {
            case (Opcode::Op_Return):
            {
                return InterpretResult::INTERPRET_OK;
                break;
            }
            case Opcode::Op_Constant:
            {
                // std::cout<<"j"<<std::endl;
                Value rconstant = READ_CONSTANT();
                push(rconstant);
                break;
            }
            case Opcode::Op_SetGlobal:
            {
                Value varname = READ_CONSTANT();
                // std::cout<<"h"<<std::endl;
                Value setvalue = pop();
                // std::cout<<"g"<<std::endl;
                int index = variables->search(AS_STRING(varname));
                if (index != -1)
                {
                    // std::cout<<"d"<<std::endl;
                    variables->entries[index].value = setvalue;
                    // std::cout<<"e"<<std::endl;
                }
                else
                {
                    // std::cout<<"f"<<std::endl;
                    variables->insert(AS_STRING(varname), setvalue);
                    // std::cout<<"o"<<std::endl;
                }
                break;
            }
            case Opcode::Op_GetGlobal:
            {
                Value varname = READ_CONSTANT();
                int index = variables->search(AS_STRING(varname));
                if (index != -1)
                {
                    push(variables->entries[index].value);
                }
                break;
            }
            case Opcode::Op_Print:
            {
                Value rconstant = pop();
                printValue(rconstant);
                break;
            }
            case Opcode::Op_Input:
            {
                std::string s;
                std::cin >> s;
                Value pushvalue = NUMBER_VAL(std::stod(s));
                push(pushvalue);
                break;
            }
            case Opcode::Op_JumpIfFalse:
            {
                uint16_t high = READ_BYTE();
                uint16_t low = READ_BYTE();
                uint16_t offset = (high << 8) | low;
                Value cond = pop();
                if (IS_FALSEY(cond))
                {
                    ip += offset;
                }
                // std::cout<<"b"<<std::endl;
                break;
            }
            case Opcode::Op_Jump:
            {
                uint16_t high = READ_BYTE();
                uint16_t low = READ_BYTE();
                uint16_t offset = (high << 8) | low;
                ip -= offset;
                break;
            }
            case (Opcode::Op_UnaryAdd):
            {
                Value curr = pop();
                curr.as.number++;
                push(curr);
                break;
            }
            case (Opcode::Op_Add):
            {
                BINARY_NUM_OP(+);
                break;
            }
            case Opcode::Op_Sub:
            {
                BINARY_NUM_OP(-);
                break;
            }
            case Opcode::Op_Mul:
            {
                BINARY_NUM_OP(*);
                break;
            }
            case Opcode::Op_Div:
            {
                BINARY_NUM_OP(/);
                break;
            }
            case Opcode::Op_Equality:
            {
                BINARY_BOOL_OP(==);
                break;
            }
            case Opcode::Op_Lesser:
            {
                BINARY_BOOL_OP(<);
                break;
            }
            case Opcode::Op_Greater:
            {
                BINARY_BOOL_OP(>);

                break;
            }
            default:
            {
                break;
            }
            }
        }
#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
    }
    void resetStack()
    {
        stackTop = stack;
    }
    void push(Value value)
    {
        *stackTop = value;
        stackTop++;
    }
    Value pop()
    {
        stackTop--;
        return *(stackTop);
    }
    Value peek()
    {
        return *(stackTop-1);
    }
};