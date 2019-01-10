#!/bin/sh
if [ "$1" != "" ]; then
    echo "Compiling $1"
    rm minibasic
    rm a.out
    rm a.ll
    rm a.s
    make
    ./minibasic < $1 > a.ll
    llc a.ll -o a.s
    clang a.s lib.a -o a.out
    # opt -dot-cfg a.ll
    ./a.out

    # clang++
    # clang-3.8
    # clang++-3.8 
    # clang-5.0
    # clang++-5.0
fi