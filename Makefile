.PHONY: clean distclean default

export SHELL := /bin/bash

ifeq ($(shell uname -s),Darwin)
export PATH := /usr/local/opt/llvm/bin:$(PATH)
endif

CXXFLAGS=-Wall -g -std=c++11 `llvm-config --cxxflags`
LDFLAGS=`llvm-config --ldflags --system-libs --libs all`
CC=gcc
CFLAGS=-Wall -g

default: alan

libs :
	lib.a

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

parser.o: parser.cpp parser.hpp ast.hpp

lexer.o: lexer.cpp parser.hpp ast.hpp

ast.o: ast.cpp ast.hpp

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $<

alan: lexer.o parser.o ast.o
	$(CXX) $(CXXFLAGS) -o alan $^ $(LDFLAGS) -lfl

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *.imm *.asm *~ alan *.dot a.out

distclean: clean
	$(RM) alan ast.txt debug