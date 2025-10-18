Recursive Descent Parser / Arithmetic Interpreter

Project Overview
This project implements a single-pass Recursive Descent Parser for a custom arithmetic language, built entirely in C/C++. It demonstrates mastery of fundamental Computer Science concepts, including Lexical Analysis, Context-Free Grammars, Operator Precedence, and Recursion.

The interpreter successfully evaluates complex arithmetic expressions, correctly handling varying levels 
of operator precedence and right-associativity.

Key Features & Computer Science Concepts
Feature | Concept Demonstrated | Recursive Descent 

Top-down parsing structure where non-terminals map directly to C functions | (expr(), term(), power(), factor()).

Operator Precedence
correctly enforces standard arithmetic order (Multiplication/Division before Addition/Subtraction).

Right-Associativity 
Correctly implements right-associativity for the exponentiation operator (**), e.g., 2 \text{ ** } 3 \text{ ** } 2 = 2 \text{ ** } 9.

Unary Operators 
Handles prefix increment (++) and decrement (--) operators.

Lexical Analysis 
Implemented a basic Lexer (lex()) to tokenize the input stream.

Error Handling 
Basic syntax error reporting during the parsing process.

Usage and Testing

Compilation
The project consists of a single C source file (parser.c). Compile using GCC:
gcc parser.c -o parser

Running the Test Suite
The included run_tests.sh script tests the parser against various expressions to verify precedence, 
associativity, and unary operator handling.
1.	Ensure parser executable and run_tests.sh are in the same directory.
2.	Run the script:
3.	bash run_tests.sh

Example Test Output (Precedence Verification)
The parser correctly processes mixed expressions like 10 + 5 * 2, ensuring multiplication is performed first (Result: 20). 
The output clearly shows the Abstract Syntax Tree (AST) generated:

AST:
+ (op)
  10 (int)
  * (op)
    5 (int)
    2 (int)

Result = 20

Author: Samuel Afon
Course: CSCE 4430 (Programming Languages)

