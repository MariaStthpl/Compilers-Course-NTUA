#!/bin/sh
if [ "$1" != "" ]; then
    echo "Compiling $1"
    make
    ./alan < $1 > a.imm
    llc a.imm -o a.s
    clang a.s lib.a -o a.asm
    # opt -dot-cfg a.ll
    ./a.asm
fi