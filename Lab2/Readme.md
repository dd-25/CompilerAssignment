# README

## Lex and GCC Compilation Guide

This guide explains how to compile and run a Lex program using `lex` and `gcc`.

### Prerequisites
Ensure you have the following installed:
- **Lex/Flex** (Lexical Analyzer Generator)
- **GCC** (GNU Compiler Collection)

### Steps to Compile and Run

1. **Generate the Lex source file**
   ```sh
   lex file_name.l
   ```
   This will generate a file named `lex.yy.c`.

2. **Compile the generated C file**
   ```sh
   gcc lex.yy.c -o output
   ```
   This compiles the `lex.yy.c` file into an executable named `output`.
   or you can simply use
   ```sh
   gcc lex.yy.c
   ```
   and use the command ./a.out in the next step
   

4. **Run the executable with an input file**
   ```sh
   ./output < test_file_name.c
   ```
   Replace `test_file_name.c` with the actual test file you want to analyze.

### Example
For a Lex file named `lexer.l` and a test file named `input.c`:
```sh
lex lexer.l
gcc lex.yy.c -o lexer
./lexer < input.c
```

### Notes
- Ensure `test_file_name.c` contains valid input for the lexer.
- The output of the program depends on the Lex rules defined in `file_name.l`.

### Troubleshooting
- If `lex` is not installed, install it using:
  ```sh
  sudo apt-get install flex  # Debian-based systems
  sudo yum install flex      # RHEL-based systems
  ```
- If `gcc` is not installed, install it using:
  ```sh
  sudo apt-get install gcc   # Debian-based systems
  sudo yum install gcc       # RHEL-based systems
  ```


