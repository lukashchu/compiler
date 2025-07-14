# J Language Compiler

This is a custom compiler for the **J programming language**, written in C. The compiler reads `.j` source files and generates equivalent assembly code in **LC4 assembly**, a custom assembly language used in the [CIS 2400 - Computer Systems](https://www.seas.upenn.edu/~cis2400/22fa/) course at the University of Pennsylvania.

While LC4 is specific to Penn, its syntax and semantics are similar to traditional RISC-based assembly languages such as ARM.

---

## 🛠️ Project Structure

```
.
├── jc.c         # Main compiler logic and control flow  
├── jc.h         # Header for compiler structs and function declarations  
├── token.c      # Tokenizer implementation (lexical analyzer)  
├── token.h      # Token structure and tokenizer interface  
├── Makefile     # Build script  
└── README.md    # Project documentation  
```

---

## ⚙️ Compilation Instructions

To build the compiler, ensure you are in the project directory and run:

```
make
```

This will generate an executable named `jc`.

To clean all compiled objects and the binary:

```
make clean
```

---

## 🧾 Usage

```
./jc input.j > output.asm
```

- `input.j`: Source file written in the J language  
- `output.asm`: Assembly output in LC4 format  

---

## 🧠 Language Overview

The **J language** is a stack-based, imperative language with control flow structures such as:

- Arithmetic operations: `+`, `-`, `*`, `/`, `%`  
- Logical operations: `AND`, `OR`, `NOT`  
- Comparison operations: `=`, `<`, `<=`, `>=`, `>`  
- Stack manipulation: `DROP`, `DUP`, `SWAP`, `ROT`  
- Control flow: `IF`, `ELSE`, `ENDIF`, `WHILE`, `ENDWHILE`  
- Function definitions: `DEFUN`, `RETURN`  
- Literals and function arguments: `LITERAL`, `ARGn`  

The tokenizer (`token.c`) reads these tokens from the input and provides structured token data for the compiler (`jc.c`) to convert to LC4 assembly.

---

## 📦 Example Workflow

1. Write a J program, e.g., `program.j`  
2. Compile it using:

```
./jc program.j > program.asm
```

3. Load and run `program.asm` using an LC4 emulator or interpreter.

---

## 🧩 Code Highlights

- **Modular Tokenization**: Clean separation of lexical analysis in `token.c`  
- **Nested Control Structures**: Implemented using a linked list stack (`NestedNode`) to support nested `IF` and `WHILE` constructs  
- **Error Handling**: Line numbers are tracked to improve syntax error reporting  

---

## 📜 Note

This project is intended for educational use. Please do not use or distribute this code without proper attribution.
