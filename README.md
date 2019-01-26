# Compilers-Course-NTUA

Compiler implementation for the Pascal-based programming language "Alan"(in C/C++ for x64-GNU/Linux) for the course of Compilers in ECE, NTUA. Tools used: flex, bison and LLVM.

## Authors

* Dadamis Phivos
* Stathopoulou Maria

## Required Installations

* llvm apt-packages version 3.8
* clang-3.8


## Usage
In order to create the Alan Compiler executable, you just need to run the provided Makefile.
After having created this executable, you can compile files written in Alan language with a ".alan" extension just by running the alan executable with the file to be compiled as parameter. Four different options are also available and explained below.
The following flags are available:
* -o : enable optimizations over the parser generated ir-code
*  -i : the input must be given on stdin, the ir is printed in stdout
* -f : the input must be given on stdin, the final code is printed in stdout
* -h : prints usage

By default, if not -i or -f are used, the ir-code , the final code and the executable will be named after the `name` of the original alan file, as `name`.imm, `name`.asm, `name`.