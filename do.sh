#!/bin/sh
if [ "$1" != "" ]; then
    echo "Compiling $1"
    # make
    # ./alan $1 > a.imm
    # llc a.imm -o a.asm
    # clang a.asm lib.a -o a.out
    # # opt -dot-cfg a.ll
    # ./a.out

    make
    ./alan $1
    llc test.imm -o test.asm
    clang test.asm lib.a -o a.out
    # opt -dot-cfg a.ll
    ./a.out
fi