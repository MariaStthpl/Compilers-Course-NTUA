.PHONY: clean distclean default

export SHELL := /bin/bash

ifeq ($(shell uname -s),Darwin)
export PATH := /usr/local/opt/llvm/bin:$(PATH)
endif

CXXFLAGS=-Wall -g -std=c++11 `llvm-config --cxxflags`
LDFLAGS=`llvm-config --ldflags --system-libs --libs all`
CC=gcc
CFLAGS=-Wall -g

default: minibasic

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

parser.o: parser.cpp parser.hpp ast.hpp

lexer.o: lexer.cpp parser.hpp ast.hpp symbol.h

ast.o: ast.cpp ast.hpp symbol.h general.h error.h

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $<

minibasic: lexer.o parser.o ast.o general.o error.o symbol.o
	$(CXX) $(CXXFLAGS) -o minibasic $^ $(LDFLAGS) -lfl

clean:
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output a.* *.o *~

distclean: clean
	$(RM) minibasic