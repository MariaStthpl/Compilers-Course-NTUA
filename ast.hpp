#ifndef __AST_H__
#define __AST_H__

extern "C"{
    #include "symbol.h"
}

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR >= 4
#include <llvm/Transforms/Scalar/GVN.h>
#endif

using namespace llvm;

class CodeGenBlock {
  public:
    BasicBlock *block;
    Value *returnValue;
};

class CodeGenContext {
  std::stack<CodeGenBlock *> blocks;
  Function *mainFunction;
  public:
    int id = 0;
    Module *module;
    CodeGenContext() { }
    BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(BasicBlock *block) { 
        blocks.push(new CodeGenBlock()); blocks.top()->returnValue = NULL;
        blocks.top()->block = block;
        id++;
        }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; id--; }
    void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
    Value *getCurrentReturnValue() { return blocks.top()->returnValue; }
};

extern CodeGenContext context;

typedef enum {
  PRINT, LET, WHILE, IF, SEQ, FUN,
  ID, ARRAY_ELEMENT, CONST, CHAR, BOOL,
  DECL, BLOCK
} kind;

typedef enum {
  L, G, LE, GE, EQ, NE
} comparison_ops;

typedef enum {
  AND, OR, NOT
} logical_ops;

typedef enum {
  PLUS, MINUS, TIMES, DIV, MOD
} arithmetic_ops;

  /* ExprAST - Base class for all expression nodes. */
class ExprAST {
  public:
  virtual ~ExprAST() {}
  virtual Value *codegen() = 0;
};

/* StmtAST - Base class for all statement nodes. */
class StmtAST {
  public:
    virtual ~StmtAST() {}
    virtual Value *codegen() = 0;
};

/* ------------------------------------------- ExprAST ------------------------------------------- */

typedef std::vector<ExprAST*> ExpressionList;
// typedef std::vector<StmtAST*> StatementList;

class Var_ExprAST: public ExprAST {
  std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames;
  std::unique_ptr<ExprAST> Body;
  public:
    Var_ExprAST(std::vector<std::pair<std::string, std::unique_ptr<ExprAST>>> VarNames,
              std::unique_ptr<ExprAST> Body)
      : VarNames(std::move(VarNames)), Body(std::move(Body)) {}

    Value *codegen() override;
};

class IdExprAST : public ExprAST {
  std::string Name;
  public:
    IdExprAST(const std::string &Name) : Name(Name) {}
    virtual Value *codegen() override;
    const std::string &getName() const { return Name; }
};

class IntConst_ExprAST : public ExprAST {
  int Val;
  public:
    IntConst_ExprAST(double Val): Val(Val){};
    virtual Value *codegen() override;
};

class CharConst_ExprAST : public ExprAST {
  int Val;
  public:
    CharConst_ExprAST(double Val): Val(Val){};
    virtual Value *codegen() override;
};

class ArithmeticOp_ExprAST: public ExprAST {
  ExprAST *LHS;
  arithmetic_ops Op;
  ExprAST *RHS;
  public:
    ArithmeticOp_ExprAST(ExprAST *LHS, arithmetic_ops op, ExprAST *RHS) : 
    LHS(std::move(LHS)), Op(op), RHS(std::move(RHS)){}
    virtual Value *codegen() override;
};

class ComparisonOp_ExprAST: public ExprAST {
  ExprAST *LHS;
  comparison_ops Op;
  ExprAST *RHS;
  public:
    ComparisonOp_ExprAST(ExprAST *LHS, comparison_ops op, ExprAST *RHS) : LHS(std::move(LHS)), Op(op), RHS(std::move(RHS)) { }
    virtual Value *codegen() override;
};

class LogicalOp_ExprAST: public ExprAST {
  ExprAST *LHS;
  logical_ops Op;
  ExprAST *RHS;
  public:
    LogicalOp_ExprAST(ExprAST *LHS, logical_ops op, ExprAST *RHS) : LHS(std::move(LHS)), Op(op), RHS(std::move(RHS)) { }
    virtual Value *codegen() override;
};

/* ------------------------------------------- StmtAST ------------------------------------------- */

typedef std::vector<StmtAST*> StatementList;

class Block: public StmtAST {
  public:
    StatementList statements;
    Block() { }
    Block(StatementList& statements): statements(std::move(statements)) { }
    virtual Value *codegen() override;
};

class Statement: public StmtAST {
  public:
    ExprAST& expr;
    Statement(ExprAST& expr): expr(expr) { }
    virtual Value *codegen() override;
};

class Return: public StmtAST {
  public:
    ExprAST *expr;
    Return(ExprAST *expr): expr(expr) { }
    virtual Value *codegen() override;
};

class If_ExprAST: public StmtAST {
  ExprAST *Cond;
  StmtAST *Then, *Else;
  public:
    If_ExprAST(ExprAST *Cond, StmtAST *Then, StmtAST *Else) : Cond(Cond), Then(Then), Else(Else) { }
    virtual Value *codegen() override;
};

class While_ExprAST: public StmtAST {
  ExprAST *Cond;
  StmtAST *Stmt;
  public:
    While_ExprAST(ExprAST *Cond, StmtAST *Stmt) : Cond(Cond), Stmt(Stmt) { }
    virtual Value *codegen() override;
};

class Assignment_StmtAST: public StmtAST {
  ExprAST *LHS, *RHS;
  public:
    Assignment_StmtAST(ExprAST *LHS, ExprAST *RHS): LHS(std::move(LHS)), RHS(std::move(RHS)) { }
    Value *codegen() override;
};

class PRINTAST: public StmtAST {
  ExprAST *p;
  public:
    PRINTAST(ExprAST *p): p(std::move(p)) {}
    virtual Value *codegen() override;
};

class CallExprAST : public StmtAST {
  std::string Callee;
  std::vector<ExprAST*> Args;
  public:
    CallExprAST(const std::string &Callee,
                std::vector<ExprAST*> Args)
      : Callee(Callee), Args(std::move(Args)) {}
    virtual Value *codegen() override;
};

// PrototypeAST - This class represents the "prototype" for a function, which captures its name,
// and its argument names (thus implicitly the number of arguments the function takes).
class PrototypeAST {
  Type *t;
  std::string Name;
  // std::vector<std::string> Args;
  std::vector<std::pair<std::string, Type*>> Args;
  public:
    PrototypeAST(Type *t, const std::string &name, std::vector<std::pair<std::string, Type*>> Args)
      : t(t), Name(name), Args(std::move(Args)) {}
    Function *codegen();
    const std::string &getName() const { return Name; }
    Type *getType() { return t; }
    std::vector<Type *> getArgsTypes() { 
          std::vector<Type *> argTypes;
          std::vector<std::pair<std::string, Type *>>::const_iterator it;
          for (it = Args.begin(); it != Args.end(); it++)
            argTypes.push_back((*it).second);
            return argTypes;
     };
};

class FunctionAST;

class LocalDef_AST { 
  public:
    virtual Function *codegen() = 0;
};

class VarDef: public LocalDef_AST, public StmtAST {
  public:
  std::pair<std::string, Type *> vdef;
    VarDef(std::pair<std::string, Type *> vdef): vdef(std::move(vdef)) { }
    virtual Function *codegen() override;
};

class FuncBody_AST {
  std::vector<LocalDef_AST *> VarNames;
  Block *Body;
  public:
    FuncBody_AST(std::vector<LocalDef_AST *> VarNames, Block *Body) : VarNames(std::move(VarNames)), Body(std::move(Body)) { }
    Value *codegen();
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST: public LocalDef_AST {
  public:
    PrototypeAST *Proto;
    FuncBody_AST *Body;
    FunctionAST(PrototypeAST *Proto, FuncBody_AST *Body)
      : Proto(std::move(Proto)), Body(Body) {}
    Function *codegen() override;
};

void llvm_compile_and_dump (FunctionAST *t);

#endif