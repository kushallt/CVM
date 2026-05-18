#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include "Tokens.h"
#include <string>
#include <algorithm>
// typedef double Value;

struct ObjString
{
  std::string chars;
};

enum class ValueType
{
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_STRING,
};

typedef struct
{
  ValueType type;
  union
  {
    bool boolean;
    double number;
    ObjString *str;
  } as;
} Value;

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_STRING(value) ((value).as.str)

#define BOOL_VAL(value) ((Value){ValueType::VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){ValueType::VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){ValueType::VAL_NUMBER, {.number = value}})
#define STRING_VAL(value) ((Value){ValueType::VAL_STRING, {.str = value}})

#define IS_BOOL(value) (value.type == ValueType::VAL_BOOL)
#define IS_NUMBER(value) (value.type == ValueType::VAL_NUMBER)
#define IS_STRING(value) (value.type == ValueType::VAL_STRING)

#define IS_FALSEY(value)                  \
   (IS_BOOL(value) && !AS_BOOL(value)) || \
   (IS_NUMBER(value) && AS_NUMBER(value) == 0)

inline std::unordered_map<TokenType, ValueType> token_to_dtype = {
    {TokenType::Number, ValueType::VAL_NUMBER},
    {TokenType::Boolean, ValueType::VAL_BOOL},
};

inline void printValue(Value v)
{
  if (IS_NUMBER(v))
    std::cout << AS_NUMBER(v);
  else if (IS_BOOL(v))
    std::cout << AS_BOOL(v);
  else if (IS_STRING(v))
    std::cout <<AS_STRING(v)->chars;
}