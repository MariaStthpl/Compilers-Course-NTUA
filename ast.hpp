#ifndef __AST_H__
#define __AST_H__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>

#include <llvm/IR/IRBuilder.h>
#include <llvm-c/Types.h>
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

class CodeGenBlock
{
  int id;

public:
  Function *fun;
  CodeGenBlock *prev;
  BasicBlock *block;
  Value *returnValue = nullptr;
  std::map<std::string, AllocaInst *> locals;
  std::map<std::string, Type *> locals_type;
  std::map<std::string, std::vector<std::pair<std::string, Type *>>> inherited;
  std::map<std::string, AllocaInst *> &getLocals() { return locals; }
  std::map<std::string, Type *> &getLocals_Types() { return locals_type; }
  std::map<std::string, std::vector<std::pair<std::string, Type *>>> &getInherited() { return inherited; }
  void setId(int n) { id = n; }
  int getId() { return id; }
  Function *getFunction() { return fun; }
};

class CodeGenContext
{
  std::stack<CodeGenBlock *> blocks;
  Function *mainFunction;

public:
  int id = 0;
  Module *module;
  CodeGenContext() {}
  std::map<std::string, AllocaInst *> &locals() { return blocks.top()->locals; }
  std::map<std::string, Type *> &locals_type() { return blocks.top()->locals_type; }
  std::map<std::string, std::vector<std::pair<std::string, Type *>>> &inherited() { return blocks.top()->inherited; }
  BasicBlock *currentBlock() { return blocks.top()->block; }
  void pushBlock(BasicBlock *block, Function *f)
  {
    blocks.push(new CodeGenBlock());
    blocks.top()->returnValue = NULL;
    blocks.top()->block = block;
    blocks.top()->fun = f;
    id++;
    blocks.top()->setId(id);
  }
  void popBlock()
  {
    CodeGenBlock *top = blocks.top();
    blocks.pop();
    delete top;
    id--;
  }
  void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
  CodeGenBlock *getTop() { return blocks.top(); }
  void setPrev(CodeGenBlock *CB) { blocks.top()->prev = CB; }
  CodeGenBlock *getPrev(CodeGenBlock *CB) { return CB->prev; }
  Value *getCurrentReturnValue() { return blocks.top()->returnValue; }
};

extern CodeGenContext context;

typedef enum
{
  PRINT,
  LET,
  WHILE,
  IF,
  SEQ,
  FUN,
  ID,
  ARRAY_ELEMENT,
  CONST,
  CHAR,
  BOOL,
  DECL,
} kind;

typedef enum
{
  L,
  G,
  LE,
  GE,
  EQ,
  NE
} comparison_ops;

typedef enum
{
  AND,
  OR,
  NOT
} logical_ops;

typedef enum
{
  PLUS = '+',
  MINUS,
  TIMES,
  DIV,
  MOD
} arithmetic_ops;


/* ExprAST - Base class for all expression nodes. */
class ExprAST
{
public:
  virtual ~ExprAST() {}
  virtual Value *codegen() = 0;
  virtual Type *getT(){return IntegerType::get(getGlobalContext(), 1);};
};

/* StmtAST - Base class for all statement nodes. */
class StmtAST
{
public:
  virtual ~StmtAST() {}
  virtual Value *codegen() = 0;
  virtual Type *getT(){return IntegerType::get(getGlobalContext(), 1);};
  virtual void TypeCheck(){ return ; };
};

/* CondAST - Base class for all condition nodes. */
class CondAST
{
  // int Val;

public:
  // CondAST(double Val) : Val(Val){};
  virtual ~CondAST() {}
  virtual Value *codegen() = 0;
  virtual Type *getT(){return IntegerType::get(getGlobalContext(), 1);};
};

/* ------------------------------------------- ExprAST ------------------------------------------- */
typedef std::vector<ExprAST *> ExpressionList;

// <int-const>
class IntConst_ExprAST : public ExprAST
{
  int Val;

public:
  IntConst_ExprAST(int Val) : Val(Val){};
  virtual Value *codegen() override;
  virtual Type *getT() override;
};

// <char-const>
class CharConst_ExprAST : public ExprAST
{
  char Val;

public:
  CharConst_ExprAST(char Val) : Val(Val){};
  virtual Value *codegen() override;
  virtual Type *getT() override;
};

/*  lvalue: id, array element, string literal */

// <id>
class Id_ExprAST : public ExprAST
{
  std::string Name;

public:
  Id_ExprAST(const std::string &Name) : Name(Name) {}
  virtual Value *codegen() override;
  const std::string &getName() const { return Name; }
  virtual Type *getT() override;
};

// <id>[<expr>]
class ArrayElement_ExprAST : public ExprAST
{
  std::string Name;
  ExprAST *expr;

public:
  ArrayElement_ExprAST(const std::string &Name, ExprAST *expr) : Name(Name), expr(expr) {}
  virtual Value *codegen() override;
  const std::string &getName() const { return Name; }
  Value *getExpr() { return expr->codegen(); }
  void TypeCheck();
  virtual Type *getT() override;
};

//TODO: string literal
class StringLiteral_ExprAST : public ExprAST
{
  std::string string_literal;

public:
  StringLiteral_ExprAST(std::string &str_literal) : string_literal(str_literal) {}
  virtual Value *codegen() override;
  const std::string &getString() const { return string_literal; }
  virtual Type *getT() override;
};

// ⟨expr⟩ ( '+' | '-' | '*' | '/' | '%' ) ⟨expr⟩
class ArithmeticOp_ExprAST : public ExprAST
{
  ExprAST *LHS;
  arithmetic_ops Op;
  ExprAST *RHS;
  bool Unary;

public:
  ArithmeticOp_ExprAST(ExprAST *LHS, arithmetic_ops op, ExprAST *RHS, bool Unary) : LHS(std::move(LHS)), Op(op), RHS(std::move(RHS)), Unary(std::move(Unary)) {}
  virtual Value *codegen() override;
  virtual Type *getT() override;
};

/* ------------------------------------------- CondAST ------------------------------------------- */

// ⟨expr⟩( '==' | '!=' | '<' | '>' | '<=' | '>=' )⟨expr⟩
class ComparisonOp_CondAST : public CondAST
{
  ExprAST *LHS;
  comparison_ops Op;
  ExprAST *RHS;

public:
  ComparisonOp_CondAST(ExprAST *LHS, comparison_ops op, ExprAST *RHS) : LHS(std::move(LHS)), Op(op), RHS(std::move(RHS)) {}
  virtual Value *codegen() override;
  void TypeCheck();
};

// ⟨cond⟩ ( '&' | '|' ) ⟨cond⟩
class LogicalOp_CondAST : public CondAST
{
  CondAST *LHS;
  logical_ops Op;
  CondAST *RHS;

public:
  LogicalOp_CondAST(CondAST *LHS, logical_ops op, CondAST *RHS) : LHS(std::move(LHS)), Op(op), RHS(std::move(RHS)) {}
  virtual Value *codegen() override;
};

/* ------------------------------------------- StmtAST ------------------------------------------- */

typedef std::vector<StmtAST *> StatementList;

// ⟨l-value⟩ '=' ⟨expr⟩
class Assignment_StmtAST : public StmtAST
{
  ExprAST *LHS, *RHS;

public:
  Assignment_StmtAST(ExprAST *LHS, ExprAST *RHS) : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  virtual Value *codegen() override;
  virtual void TypeCheck() override;
};

// ⟨compound-stmt⟩
class CompoundStmt_StmtAST : public StmtAST
{
public:
  StatementList statements;
  CompoundStmt_StmtAST() {}
  CompoundStmt_StmtAST(StatementList &statements) : statements(std::move(statements)) {}
  virtual Value *codegen() override;
};

// ⟨func-call⟩
class FuncCall : public StmtAST, public ExprAST
{
  std::string Callee;
  std::vector<ExprAST *> Args;

public:
  FuncCall(const std::string &Callee,
           std::vector<ExprAST *> Args)
      : Callee(Callee), Args(std::move(Args)) {}
  virtual Value *codegen() override;
  virtual Type *getT() override;
};

// “if” '(' ⟨cond⟩ ')' ⟨stmt⟩ [ “else” ⟨stmt⟩ ]
class If_StmtAST : public StmtAST
{
  CondAST *Cond;
  StmtAST *Then, *Else;

public:
  If_StmtAST(CondAST *Cond, StmtAST *Then, StmtAST *Else) : Cond(Cond), Then(Then), Else(Else) {}
  virtual Value *codegen() override;
};

// “while” '(' ⟨cond⟩ ')' ⟨stmt⟩
class While_StmtAST : public StmtAST
{
  CondAST *Cond;
  StmtAST *Stmt;

public:
  While_StmtAST(CondAST *Cond, StmtAST *Stmt) : Cond(Cond), Stmt(Stmt) {}
  virtual Value *codegen() override;
};

// “return” [ ⟨expr⟩ ]
class Return_Stmt : public StmtAST
{
public:
  ExprAST *expr;
  Return_Stmt(ExprAST *expr) : expr(expr) {}
  virtual Value *codegen() override;
  virtual void TypeCheck() override;
};

/* ------------------------------------------- Function ------------------------------------------- */

// This class captures: name, argument names and return type of a function.
class PrototypeAST
{
  Type *t;
  std::string Name;
  std::vector<std::pair<std::string, Type *>> Args;
  // a.insert(std::end(a), std::begin(b), std::end(b));

public:
  PrototypeAST(Type *t, const std::string &name, std::vector<std::pair<std::string, Type *>> Args) : t(t), Name(name), Args(std::move(Args)) {}

  Function *codegen();
  const std::string &getName() const { return Name; }
  Type *getType() { return t; }
  std::vector<Type *> getArgsTypes()
  {
    std::vector<Type *> argTypes;
    std::vector<std::pair<std::string, Type *>>::const_iterator it;
    for (it = Args.begin(); it != Args.end(); it++)
    {
      // pass array by reference -> dont create pointer to pointer
      if (PointerType::classof((*it).second) && PointerType::classof(((*it).second)->getPointerElementType()))
        argTypes.push_back(((*it).second)->getPointerElementType());
      else
        argTypes.push_back((*it).second);
    }
    return argTypes;
  };
};

// ⟨func-def⟩ | ⟨var-def⟩
class LocalDef_AST
{
public:
  virtual Function *codegen() = 0;
};

// ⟨var-def⟩
class VarDef : public LocalDef_AST
{
public:
  std::pair<std::string, Type *> vdef;
  VarDef(std::pair<std::string, Type *> vdef) : vdef(std::move(vdef)) {}
  virtual Function *codegen() override;
};

// ( ⟨local-def⟩ )* ⟨compound-stmt⟩
class FuncBody_AST
{
  std::vector<LocalDef_AST *> VarNames;
  CompoundStmt_StmtAST *Body;

public:
  FuncBody_AST(std::vector<LocalDef_AST *> VarNames, CompoundStmt_StmtAST *Body) : VarNames(std::move(VarNames)), Body(std::move(Body)) {}
  Value *codegen();
};

// ⟨func-def⟩ - This class represents a function definition itself.
class FunctionAST : public LocalDef_AST
{
public:
  PrototypeAST *Proto;
  FuncBody_AST *Body;
  FunctionAST(PrototypeAST *Proto, FuncBody_AST *Body)
      : Proto(std::move(Proto)), Body(Body) {}
  Function *codegen() override;
};

/* ------------------------------------------- Custom Functions ------------------------------------------- */

class WriteInteger : public StmtAST
{
  ExprAST *p;

public:
  WriteInteger(ExprAST *p) : p(std::move(p)) {}
  virtual Value *codegen() override;
  // virtual Type *getT() override;
};

class WriteByte : public StmtAST
{
  ExprAST *p;

public:
  WriteByte(ExprAST *p) : p(std::move(p)) {}
  virtual Value *codegen() override;
  // virtual Type* getT() override;
};

class WriteChar : public StmtAST
{
  ExprAST *p;

public:
  WriteChar(ExprAST *p) : p(std::move(p)) {}
  virtual Value *codegen() override;
  // virtual Type* getT() override;
};

class WriteString : public StmtAST
{
  ExprAST *str;

public:
  WriteString(ExprAST *str) : str(std::move(str)) {}
  virtual Value *codegen() override;
  // virtual Type* getT() override;
};

class ReadInteger : public ExprAST
{
public:
  ReadInteger() {}
  virtual Value *codegen() override;
};

class ReadByte : public ExprAST
{
public:
  ReadByte() {}
  virtual Value *codegen() override;
};

class ReadChar : public ExprAST
{
public:
  ReadChar() {}
  virtual Value *codegen() override;
};

class ReadString : public StmtAST
{
  ExprAST *expr;
  Id_ExprAST *array;

public:
  ReadString(ExprAST *expr, Id_ExprAST *array) : expr(std::move(expr)), array(std::move(array)) {}
  virtual Value *codegen() override;
};

class Extend : public ExprAST
{
  ExprAST *expr;

public:
  Extend(ExprAST *expr) : expr(std::move(expr)) {}
  virtual Value *codegen() override;
  // void getT();
};

class Shrink : public ExprAST
{
  ExprAST *expr;

public:
  Shrink(ExprAST *expr) : expr(std::move(expr)) {}
  virtual Value *codegen() override;
  // void getT();
};

class Strlen : public ExprAST
{
  ExprAST *Arr;

public:
  Strlen(ExprAST *Arr) : Arr(std::move(Arr)) {}
  virtual Value *codegen() override;
  // void getT();
};

class Strcpy : public StmtAST
{
  ExprAST *LArr;
  ExprAST *RArr;

public:
  Strcpy(ExprAST *LArr, ExprAST *RArr) : LArr(std::move(LArr)), RArr(std::move(RArr)) {}
  virtual Value *codegen() override;
  // void getT();
};

class Strcmp : public ExprAST
{
  ExprAST *LArr;
  ExprAST *RArr;

public:
  Strcmp(ExprAST *LArr, ExprAST *RArr) : LArr(std::move(LArr)), RArr(std::move(RArr)) {}
  virtual Value *codegen() override;
  // void getT();
};

class Strcat : public StmtAST
{
  ExprAST *LArr;
  ExprAST *RArr;

public:
  Strcat(ExprAST *LArr, ExprAST *RArr) : LArr(std::move(LArr)), RArr(std::move(RArr)) {}
  virtual Value *codegen() override;
  // void getT();
};

void llvm_compile_and_dump(FunctionAST *t);

#endif