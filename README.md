# CVM++ — A Custom Interpreted Programming Language in C++

## 📌 Overview

**CVM++** is a lightweight interpreted programming language built from scratch in C++. It implements a full language execution pipeline including:

- Lexer (tokenizer)
- Recursive descent parser
- AST (Abstract Syntax Tree)
- Bytecode compiler
- Stack-based virtual machine

The project is designed as an educational implementation to understand how real-world interpreters (like Python or Lua) work internally.

---

## ⚙️ Features

### 🔹 Lexer / Tokenizer
- Converts raw source code into tokens
- Handles keywords, identifiers, literals, and operators  
📄 `Lexer.h`, `Lexer.cpp`

---

### 🔹 Recursive Descent Parser
- Builds an Abstract Syntax Tree (AST)
- Implements grammar rules manually  
📄 `Parser.h`, `parser.cpp`, `ASTNodes.h`

---
### 🔹 AST & Visitor Pattern

The AST is implemented using the Visitor design pattern.

Each node exposes an `accept(Visitor&)` method, allowing the compiler and other components to traverse the AST without modifying node classes.

This enables:
- Separation of AST structure and execution logic
- Easy addition of new operations (compiler, interpreter, debugger)
- Clean extensibility without modifying existing nodes

### 🔹 Bytecode Compiler
- Converts AST into bytecode instructions
- Stores constants in a constant pool (Chunk system)  
📄 `chunk.h`, `ValueArray.h`, `compiler.h`

---

### 🔹 Stack-Based Virtual Machine
- Executes bytecode instructions
- Uses a stack model similar to JVM / CPython VM  
📄 `VirtualMachine.h`

---

### 🔹 Hash Table System
- Stores global variables
- Supports fast lookup using hashing  
📄 `HashTable.h`

---

### 🔹 Memory Utilities
- Custom dynamic memory management system  
📄 `memory.h`

---

### 🔹 Supporting Components
- `commons.h` → Shared types and macros
- `Tokens.h` → Token definitions
- `error.h` → Error handling utilities (not complete)

---

## 🧠 Compilation Pipeline

```text
Source Code (.cvm)
        ↓
Lexer / Tokenizer
        ↓
Parser (AST Generation)
        ↓
Bytecode Compiler (Chunk)
        ↓
Virtual Machine Execution

================================================================
