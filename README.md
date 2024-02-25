# COMP2023 Compiler Project

## Overview
This project aims to develop a compiler for the COMP2023 language. The compiler consists of several components, including a file descriptor, scanner, parser, and symbol table. This readme provides an overview of the project structure and tasks.

## File Descriptor
The file descriptor module manages file operations and error handling. It opens files, reads lines, tracks line and character numbers, and reports errors related to file parsing.

### Tasks:
- Implement file descriptor functionalities.
- Test file descriptor operations.
- Ensure error reporting functionality.

## Scanner
The scanner reads a COMP2023 program file and produces a sequence of tokens representing lexemes in the program. It recognizes identifiers, reserved words, literals, operators, and comments.

### Tasks:
- Write regular expressions for lexemes.
- Test file descriptor integration with the scanner.
- Implement and test scanner functionalities.

## Parser
The parser analyzes the syntax of COMP2023 programs using LL(1) grammar. It produces abstract syntax trees (ASTs) for valid programs, detecting and reporting syntax and semantic errors.

### Tasks:
- Produce LL(1) grammar for COMP2023.
- Modify symbol table to accommodate scoped variables.
- Write a recursive-descent parser for COMP2023.
- Ensure error detection and reporting.

## Symbol Table
The symbol table maps identifiers to symbol table entries, recording information about variables, constants, and routines. It supports scoping rules similar to C's.

### Tasks:
- Implement scoped symbol table functionalities.
- Add functions for symbol table entry manipulation.
- Integrate symbol table with parser.

## Support Routines
Support routines provide functionality for AST manipulation, printing, and evaluation.

### Tasks:
- Modify support routines for compatibility with C++.
- Implement AST node creation, printing, and evaluation.

## Conclusion
This readme outlines the tasks and objectives of the COMP2023 compiler project. Each component plays a crucial role in analyzing and processing COMP2023 programs, ultimately contributing to the development of a functional compiler.
