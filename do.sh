#!/bin/sh

if [ "$1" != "" ]; then
    echo "Compiling $1"
    make
    ./minibasic < $1 > a.ll
    llc a.ll -o a.s
    clang a.s lib.a -o a.out
    opt -dot-cfg a.ll
    ./a.out
fi