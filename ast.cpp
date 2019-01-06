#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "ast.hpp"

using namespace llvm;

std::ofstream myfile;

// Global LLVM variables related to the LLVM suite.
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
// static std::map<std::string, Value *> NamedValues;
// static std::map<std::string, AllocaInst *> NamedValues;
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

// Global LLVM variables related to the generated code.
static GlobalVariable *TheVars;
static GlobalVariable *TheNL;

static Function *TheWriteInteger;
static Function *TheWriteByte;
static Function *TheWriteChar;
static Function *TheWriteString;
static Function *TheReadInteger;
static Function *TheReadChar;
static Function *TheReadString;

static Function *TheStrlen;
static Function *TheStrcmp;
static Function *TheStrcpy;
static Function *TheStrcat;

// Useful LLVM types.
static Type *i1 = IntegerType::get(TheContext, 1);
static Type *i8 = IntegerType::get(TheContext, 8);
static Type *i16 = IntegerType::get(TheContext, 16);
static Type *i32 = IntegerType::get(TheContext, 32);
// static Type *i64 = IntegerType::get(TheContext, 64);

// Useful LLVM helper functions.
inline ConstantInt *c1(char c)
{
  return ConstantInt::get(TheContext, APInt(1, c, true));
}

inline ConstantInt *c8(char c)
{
  return ConstantInt::get(TheContext, APInt(8, c, true));
}
inline ConstantInt *c32(int n)
{
  return ConstantInt::get(TheContext, APInt(32, n, true));
}
inline ConstantInt *c16(int n)
{
  return ConstantInt::get(TheContext, APInt(16, n, true));
}

CodeGenContext context;

void llvm_compile_and_dump(FunctionAST *t)
{
  myfile.open("ast.txt");

  // Initialize the module and the optimization passes.
  TheModule = make_unique<Module>("alan program", TheContext);
  TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
  TheFPM->add(createPromoteMemoryToRegisterPass());
  // TODO optimizations
  // TheFPM->add(createInstructionCombiningPass());
  // TheFPM->add(createReassociatePass());
  // TheFPM->add(createGVNPass());
  // TheFPM->add(createCFGSimplificationPass());
  TheFPM->doInitialization();
  // Define and initialize global symbols.
  // @vars = global [26 x i32] zeroinitializer, align 16
  ArrayType *vars_type = ArrayType::get(i32, 26);
  TheVars = new GlobalVariable(
      *TheModule, vars_type, false, GlobalValue::PrivateLinkage,
      ConstantAggregateZero::get(vars_type), "vars");
  TheVars->setAlignment(16);
  // @nl = private constant [2 x i8] c"\0A\00", align 1
  ArrayType *nl_type = ArrayType::get(i8, 2);
  TheNL = new GlobalVariable(
      *TheModule, nl_type, true, GlobalValue::PrivateLinkage,
      ConstantArray::get(nl_type,
                         std::vector<Constant *>{c8('\n'), c8('\0')}),
      "nl");
  TheNL->setAlignment(1);
  /* ------------------ declare library of functions --------------------------*/
  // declare void @writeInteger(i64)
  FunctionType *writeInteger_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i16}, false);
  TheWriteInteger =
      Function::Create(writeInteger_type, Function::ExternalLinkage,
                       "writeInteger", TheModule.get());
  // declare void @writeByte(i8)
  FunctionType *writeByte_type =
      FunctionType::get(Type::getInt8Ty(TheContext),
                        std::vector<Type *>{i8}, false);
  TheWriteByte =
      Function::Create(writeByte_type, Function::ExternalLinkage,
                       "ord", TheModule.get());
  // declare void @writeChar(i8)
  FunctionType *writeChar_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i8}, false);
  TheWriteChar =
      Function::Create(writeChar_type, Function::ExternalLinkage,
                       "writeChar", TheModule.get());
  // declare void @writeString(i8*)
  FunctionType *writeString_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::get(i8, 0)}, false);
  TheWriteString =
      Function::Create(writeString_type, Function::ExternalLinkage,
                       "writeString", TheModule.get());
  /*----------------------------READS----------------------------*/
  FunctionType *readInteger_type =
      FunctionType::get(Type::getInt16Ty(TheContext),
                        std::vector<Type *>(), false);
  TheReadInteger =
      Function::Create(readInteger_type, Function::ExternalLinkage,
                       "readInteger", TheModule.get());

  FunctionType *readChar_type =
      FunctionType::get(Type::getInt8Ty(TheContext),
                        std::vector<Type *>(), false);
  TheReadChar =
      Function::Create(readChar_type, Function::ExternalLinkage,
                       "readChar", TheModule.get());

  FunctionType *readString_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i16, PointerType::getUnqual(i8)}, false);
  TheReadString =
      Function::Create(readString_type, Function::ExternalLinkage,
                       "readString", TheModule.get());

  /*-----------------------------------------------------------------------------------
  --------------------------      Functions for strings      --------------------------
  -----------------------------------------------------------------------------------*/
  // strlen (s : reference byte []) : int
  FunctionType *strlen_type =
      FunctionType::get(Type::getInt16Ty(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8)}, false);
  TheStrlen =
      Function::Create(strlen_type, Function::ExternalLinkage,
                       "strlen", TheModule.get());
  //strcmp (s1 : reference byte [], s2 : reference byte []) : int
  FunctionType *strcmp_type =
      FunctionType::get(Type::getInt16Ty(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8), PointerType::getUnqual(i8)}, false);
  TheStrcmp =
      Function::Create(strcmp_type, Function::ExternalLinkage,
                       "strcmp", TheModule.get());
  //strcpy (trg : reference byte [], src : reference byte []) : proc
  FunctionType *strcpy_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8), PointerType::getUnqual(i8)}, false);
  TheStrcpy =
      Function::Create(strcpy_type, Function::ExternalLinkage,
                       "strcpy", TheModule.get());
  //strcat (trg : reference byte [], src : reference byte []) : proc
  FunctionType *strcat_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8), PointerType::getUnqual(i8)}, false);
  TheStrcat =
      Function::Create(strcat_type, Function::ExternalLinkage,
                       "strcat", TheModule.get());

  // Define and start the main function.
  t->codegen();
  if (t->Proto->getName() != "main")
  {
    auto fun = TheModule->getFunction("main");
    auto main_function = TheModule->getFunction(t->Proto->getName());
    if (fun != NULL)
      fun->setName("_oldmain");
    main_function->setName("main");
  }

  // Verify and optimize the main function.
  bool bad = verifyModule(*TheModule, &errs());
  if (bad)
  {
    fprintf(stderr, "The faulty IR is:\n");
    fprintf(stderr, "------------------------------------------------\n\n");
    TheModule->print(outs(), nullptr);
    return;
  }
  // TheFPM->run(*main);
  // Print out the IR.
  TheModule->print(outs(), nullptr);
}

std::unique_ptr<ExprAST> LogError(const char *Str)
{
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
  LogError(Str);
  return nullptr;
}

Value *LogErrorV(const char *Str)
{
  LogError(Str);
  return nullptr;
}

Value *loadValue(Value *p)
{
  if (PointerType::classof(p->getType()))
   {
    LogErrorV("POINTER FOUND MMMMMMMMM");
    // return Builder.CreateAlignedLoad(p, 4);
    return Builder.CreateLoad(p, "var");}
  else
    return p;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, Type *t)
{
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(t, 0, VarName.c_str());
}

/* ------------------------------------------- IR for ExprAST ------------------------------------------- */

// IR for <int-const>
Value *IntConst_ExprAST::codegen()
{
  // Function *TheFunction = Builder.GetInsertBlock()->getParent();
  AllocaInst *V = context.locals()["CONST"];

  Builder.CreateStore(c16(Val), V);
  return V;

  // std::vector<Value *> indexList;
  // indexList.push_back(c16(0));
  // LoadInst *ldinst = Builder.CreateLoad(V, "int_const");
  // GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(V->getAllocatedType()->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), "int_const", Builder.GetInsertBlock());
  // Builder.CreateStore(c16(Val), gepInst);
  // return gepInst;
  // myfile << "<int-const>: ";
  // return c16(Val);
}

Type *IntConst_ExprAST::getT()
{
  return IntegerType::get(TheContext, 16);
}

// IR for <char-const>
Value *CharConst_ExprAST::codegen()
{
  // myfile << "<char-const>: " << Val;
  return c8(Val);
}

Type *CharConst_ExprAST::getT()
{
  return i8;
}

// IR for <id>
Value *Id_ExprAST::codegen()
{
  // Look this variable up in the function.
  AllocaInst *V = context.locals()[Name];
  if (!V)
  {
    LogErrorV("Unknown variable name");
    exit(1);
  }

  if (PointerType::classof(V->getAllocatedType()) && ArrayType::classof(V->getAllocatedType()->getPointerElementType()))
  {
    // id of array (in nested function) with pointer of ArrayType
    std::vector<Value *> indexList;
    indexList.push_back(c16(0));
    indexList.push_back(c16(0));
    LoadInst *ldinst = Builder.CreateLoad(V, Name);
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((V->getAllocatedType()->getPointerElementType()), ldinst, ArrayRef<Value *>(indexList), Name, Builder.GetInsertBlock());
    return gepInst;
  }
  else if (PointerType::classof(V->getAllocatedType()))
  {
    // id (in nested function) with pointer of IntegerType
    std::vector<Value *> indexList;
    indexList.push_back(c16(0));
    LoadInst *ldinst = Builder.CreateLoad(V, Name);
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((V->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), Name, Builder.GetInsertBlock());
    // return Builder.CreateLoad(gepInst, Name);
    return gepInst;
  }
  else if (ArrayType::classof(V->getAllocatedType()))
  {
    // id of array of ArrayType
    std::vector<Value *> indexList;
    indexList.push_back(c8(0));
    indexList.push_back(c8(0));
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(V->getAllocatedType(), V, ArrayRef<Value *>(indexList), Name, Builder.GetInsertBlock());
    return gepInst;
  }
  else
  {
    // id of IntegerType
    Builder.CreateLoad(V, Name.c_str());
    return V;
  }

  return V;
}

Type *Id_ExprAST::getT()
{
  return context.locals_type()[Name];
}

// IR for <id>[<expr>]
Value *ArrayElement_ExprAST::codegen()
{

  AllocaInst *arr = context.locals()[Name];
  Value *index = expr->codegen();

  if (PointerType::classof(arr->getAllocatedType()))
  {
    // array element (in nested function) with pointer of ArrayType
    std::vector<Value *> indexList;
    indexList.push_back(c32(0));
    indexList.push_back(index);
    LoadInst *ldinst = Builder.CreateLoad(arr, Name);
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((arr->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), Name, context.currentBlock());
    // return Builder.CreateLoad(gepInst, Name);
    return gepInst;
  }
  else
  {
    // array element of ArrayType
    std::vector<Value *> indexList;
    indexList.push_back(c32(0));
    indexList.push_back(index);
    // Builder.CreateLoad(arr, Name); // needed?
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(arr->getAllocatedType(), arr, ArrayRef<Value *>(indexList), Name, context.currentBlock());
    // return Builder.CreateLoad(gepInst, Name);
    return gepInst;
  }
}

Type *ArrayElement_ExprAST::getT()
{
  // TypeCheck();
  return context.locals_type()[Name];
}

void ArrayElement_ExprAST::TypeCheck()
{
  Value *index = expr->codegen();
  if (!(index->getType()->isIntegerTy(16)))
  {
    LogErrorV("\nIndex of array must be of int type\n");
    exit(1);
  }
  return;
}

// IR for string-literal
Value *StringLiteral_ExprAST::codegen()
{
  AllocaInst *alloca = Builder.CreateAlloca(ArrayType::get(IntegerType::get(TheContext, 8), string_literal.length() - 1), 0, "string");
  std::vector<Value *> indexList;
  indexList.push_back(c8(0));
  size_t i;
  for (i = 1; i < string_literal.length() - 1; i++)
  {
    indexList.push_back(c8(i - 1));
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(alloca->getAllocatedType(), alloca, ArrayRef<Value *>(indexList), "str", context.currentBlock());
    Builder.CreateStore(c8(string_literal[i]), gepInst);
    indexList.pop_back();
  }
  indexList.push_back(c8(i - 1));
  GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(alloca->getAllocatedType(), alloca, ArrayRef<Value *>(indexList), "str", context.currentBlock());
  Builder.CreateStore(c8(0), gepInst);
  indexList.pop_back();

  indexList.push_back(c8(0));
  gepInst = GetElementPtrInst::CreateInBounds(alloca->getAllocatedType(), alloca, ArrayRef<Value *>(indexList), "str_ptr", context.currentBlock());
  return gepInst;
}

Type *StringLiteral_ExprAST::getT()
{
  return PointerType::getUnqual(i8);
}

// IR for ⟨expr⟩ ( '+' | '-' | '*' | '/' | '%' ) ⟨expr⟩
Value *ArithmeticOp_ExprAST::codegen()
{
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  L = loadValue(L);
  R = loadValue(R);

  switch (Op)
  {
  case PLUS:
    return Builder.CreateAdd(L, R, "addtmp");
  case MINUS:
    return Builder.CreateSub(L, R, "subtmp");
  case TIMES:
    // if (L->getType()->isIntegerTy(16))
    // {
    //   LogErrorV("L iS 16!!");
    // }
    // if (R->getType()->isIntegerTy(16))
    // {
    //   LogErrorV("R iS 16!!");
    // }
    return Builder.CreateMul(L, R, "multmp");
  case DIV:
    return Builder.CreateUDiv(L, R, "modtmp");
  case MOD:
    return Builder.CreateURem(L, R, "modtmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

Type *ArithmeticOp_ExprAST::getT()
{
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  L = loadValue(L);
  R = loadValue(R);
  if (!Unary)
  {
    if ((L->getType()) != (R->getType()))
    {
      LogErrorV("\noperands of arithmetic operation must be of same type\n");
      exit(1);
    }
  }
  return R->getType();
}

/* ------------------------------------------- IR for CondAST ------------------------------------------- */

// IR for ⟨expr⟩( '==' | '!=' | '<' | '>' | '<=' | '>=' )⟨expr⟩
Value *ComparisonOp_CondAST::codegen()
{
  // TypeCheck();

  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l || !r)
    return nullptr;

  l = loadValue(l);
  r = loadValue(r);

  switch (Op)
  {
  case L:
    return Builder.CreateICmpSLT(c1(1), c1(0), "ltmp");
  case G:
    return Builder.CreateICmpSGT(l, r, "gtmp");
  case LE:
    return Builder.CreateICmpSLE(l, r, "letmp");
  case GE:
    return Builder.CreateICmpSGE(l, r, "getmp");
  case EQ:
    return Builder.CreateICmpEQ(l, r, "eqtmp");
  case NE:
    return Builder.CreateICmpNE(l, r, "netmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

void ComparisonOp_CondAST::TypeCheck()
{
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  L = loadValue(L);
  R = loadValue(R);
  // if ((L->getType()) != (R->getType()))
  // {
  //   LogErrorV("\noperands of comparison operation must be of same type\n");
  //   exit(1);
  // }
  return;
}

// IR for ⟨cond⟩ ( '&' | '|' ) ⟨cond⟩
Value *LogicalOp_CondAST::codegen()
{
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l)
    return nullptr;

  l = loadValue(l);
  r = loadValue(r);

  switch (Op)
  {
  case AND:
    if (!r)
      return nullptr;
    return Builder.CreateAnd(l, r, "andtmp");
  case OR:
    if (!r)
      return nullptr;
    return Builder.CreateOr(l, r, "ortmp");
  case NOT:
    return Builder.CreateNot(l, "nottmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

/* ------------------------------------------- IR for StmtAST ------------------------------------------- */

// IR for ⟨l-value⟩ '=' ⟨expr⟩
Value *Assignment_StmtAST::codegen()
{
  // TypeCheck();

  // evaluate r-value
  Value *Val = RHS->codegen();
  if (!Val)
    return nullptr;

  // if r-value is pointer, load its value
  Val = loadValue(Val);

  // l-value is always a pointer
  Value *V = LHS->codegen();

  return Builder.CreateStore(Val, V);
}

void Assignment_StmtAST::TypeCheck()
{
  // Value *L = LHS->codegen();
  // Value *R = RHS->codegen();

  // Type *LT = L->getType();
  // Type *RT = R->getType();

  // if (LT->isPointerTy())
  //   LT = LT->getPointerElementType();

  // if (RT->isPointerTy())
  //   RT = RT->getPointerElementType();

  // if (!(LT->isIntegerTy(16) && (RT->isIntegerTy(16))) && !(LT->isIntegerTy(8) && (RT->isIntegerTy(8))))
  // {
  //   LogErrorV("\noperands of assignement operation must be of same type\n");
  //   exit(1);
  // }
  return;
}

// IR for ⟨compound-stmt⟩
Value *CompoundStmt_StmtAST::codegen()
{
  StatementList::const_iterator it;
  Value *last = NULL;
  for (it = statements.begin(); it != statements.end(); it++)
  {
    // std::cout << "Generating code for " << typeid(**it).name() << std::endl;
    last = (**it).codegen();
  }
  // std::cout << "Creating block" << std::endl;
  return last;
}

// IR for ⟨func-call⟩
Value *FuncCall::codegen()
{
  myfile << "\nCalling function: " << Callee << std::endl;
  // Look up the name in the global module table.
  Function *CalleeF = TheModule->getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  // If argument mismatch error. Except the real parameters, we take into consideration the # pointers of previous variables' declaration
  if (CalleeF->arg_size() != (Args.size() + (context.getTop()->inherited[Callee]).size()))
    return LogErrorV("Incorrect # arguments passed");

  std::vector<Value *> ArgsV;
  FunctionType *FTy = CalleeF->getFunctionType();
  for (unsigned i = 0, e = Args.size(); i != e; ++i)
  {
    // check argument pass by reference
    if (PointerType::classof(FTy->getParamType(i)))
    {
      if (dynamic_cast<StringLiteral_ExprAST *>(Args[i]) != nullptr)
      {
        StringLiteral_ExprAST *str = dynamic_cast<StringLiteral_ExprAST *>(Args[i]);
        ArgsV.push_back(str->codegen());
      }
      else if (dynamic_cast<Id_ExprAST *>(Args[i]) != nullptr)
      {
        myfile << std::endl
               << "\t----func call with pointer" << std::endl;

        Id_ExprAST *temp = dynamic_cast<Id_ExprAST *>(Args[i]);
        Value *V = context.locals()[temp->getName()];
        V = loadValue(V);
        if (ArrayType::classof(V->getType()))
        {
          myfile << "\t\t--func call array" << std::endl;
          std::vector<Value *> indexList;
          indexList.push_back(c16(0));
          indexList.push_back(c16(0));
          GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(V->getType(), V, ArrayRef<Value *>(indexList), temp->getName(), context.currentBlock());
          ArgsV.push_back(gepInst);
        }
        else
        {
          std::vector<Value *> indexList;
          indexList.push_back(c16(0));
          ArgsV.push_back(GetElementPtrInst::CreateInBounds(V->getType(), V, ArrayRef<Value *>(indexList), temp->getName(), context.currentBlock()));
        }
        if (!V)
          LogErrorV("Unknown variable name");
      }
    }
    else
      ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  for (std::vector<std::pair<std::string, Type *>>::iterator it = (context.inherited()[Callee]).begin(); it != (context.inherited()[Callee]).end(); ++it)
  {
    if (PointerType::classof((context.locals()[it->first])->getAllocatedType()))
    {
      std::vector<Value *> indexList;
      indexList.push_back(c16(0));
      GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((context.locals()[it->first])->getAllocatedType(), (context.locals()[it->first]), ArrayRef<Value *>(indexList), it->first, context.currentBlock());
      LoadInst *ldinst = Builder.CreateLoad(gepInst, it->first);
      ArgsV.push_back(ldinst);
    }
    else
    {
      std::vector<Value *> indexList;
      indexList.push_back(c16(0));
      GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((context.locals()[it->first])->getAllocatedType(), (context.locals()[it->first]), ArrayRef<Value *>(indexList), it->first, context.currentBlock());
      ArgsV.push_back(gepInst);
    }
  }
  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Type *FuncCall::getT()
{

  return context.getTop()->getFunction()->getFunctionType()->getReturnType();
}

// IR for “if” '(' ⟨cond⟩ ')' ⟨stmt⟩ [ “else” ⟨stmt⟩ ]
Value *If_StmtAST::codegen()
{
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  CondV = Builder.CreateICmpNE(CondV, c32(0), "ifcond");

  Function *TheFunction = Builder.GetInsertBlock()->getParent(); //must fix

  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  Builder.SetInsertPoint(ThenBB);
  Value *ThenV = Then->codegen();
  if (!ThenV)
    return nullptr;

  // Builder.SetInsertPoint(ThenBB);
  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateBr(MergeBB);

  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);

  Value *ElseV = nullptr;
  if (Else != nullptr)
  {
    ElseV = Else->codegen();
    if (!ElseV)
      return nullptr;
  }

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateBr(MergeBB);

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);

  return c32(0);
}

// IR for “while” '(' ⟨cond⟩ ')' ⟨stmt⟩
Value *While_StmtAST::codegen()
{
  Value *CondV = Cond->codegen();
  if (CondV->getType()->isIntegerTy(1))
  {
    LogErrorV("CONDV IS 1");
  }
  if (!CondV)
    return nullptr;
  CondV = Builder.CreateICmpNE(CondV, c1(0), "while_entry");
  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  BasicBlock *WhileBB = BasicBlock::Create(TheContext, "while", TheFunction);
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "while_end");

  Builder.CreateCondBr(CondV, WhileBB, MergeBB);

  Builder.SetInsertPoint(WhileBB);

  Value *StmtV = Stmt->codegen();
  if (!StmtV)
    return nullptr;

  Value *WhileV = Cond->codegen();
  if (CondV->getType()->isIntegerTy(1))
  {
    LogErrorV("WHILEV IS I1");
  }

  if (!WhileV)
    return nullptr;
  WhileV = Builder.CreateICmpNE(WhileV, c1(0), "while_entry");

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateCondBr(WhileV, WhileBB, MergeBB);
  // WhileBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);

  return c32(0);
}

// IR for “return” [ ⟨expr⟩ ]
Value *Return_Stmt::codegen()
{
  // TypeCheck();
  if (expr == nullptr)
  {
    context.setCurrentReturnValue(nullptr);
    ReturnInst::Create(TheContext, context.getCurrentReturnValue(), Builder.GetInsertBlock());
    return c32(0);
  }

  Value *returnValue = expr->codegen();
  returnValue = loadValue(returnValue);
  context.setCurrentReturnValue(returnValue);
  ReturnInst::Create(TheContext, context.getCurrentReturnValue(), Builder.GetInsertBlock());
  // std::cout << " return  " <<  context.id << std::endl;
  return returnValue;
}

void Return_Stmt::TypeCheck()
{
  Type *ret = context.getTop()->getFunction()->getFunctionType()->getReturnType();

  if (!expr)
  {
    if (!ret->isVoidTy())
    {
      LogErrorV("Return type of function doesn't match definition type which is void");
      exit(1);
    }
  }
  else if (!(ret->isIntegerTy(16) && expr->getT()->isIntegerTy(16)) && !(ret->isIntegerTy(8) && expr->getT()->isIntegerTy(8)))
  {
    LogErrorV("Return type of function doesn't match definition type ");
    exit(1);
  }
  return;
}

/* ------------------------------------------- IR for Function ------------------------------------------- */

Function *PrototypeAST::codegen()
{
  /* pass previous variables as pointers */
  if (context.id > 0)
    Args.insert(Args.end(), (context.getTop())->inherited[Name].begin(), (context.getTop())->inherited[Name].end());

  std::vector<Type *> argTypes = getArgsTypes();

  FunctionType *FT =
      FunctionType::get(getType(), makeArrayRef(argTypes), false);

  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++].first);
  return F;
}

Function *VarDef::codegen()
{
  return nullptr;
}

// IR for ( ⟨local-def⟩ )* ⟨compound-stmt⟩
Value *FuncBody_AST::codegen()
{

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  AllocaInst *V = CreateEntryBlockAlloca(TheFunction, "CONST", i16);
  context.locals()["CONST"] = V;

  // <local-def>
  for (unsigned i = 0, e = VarNames.size(); i != e; ++i)
  {
    // <var-def>
    if (dynamic_cast<VarDef *>(VarNames[i]) != nullptr)
    {
      VarDef *temp = dynamic_cast<VarDef *>(VarNames[i]);

      const std::string &VarName = temp->vdef.first;
      Type *t = temp->vdef.second;

      AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName, t);

      context.locals()[VarName] = Alloca;
      context.locals_type()[VarName] = t;
    }
    // <func-def>
    else if (dynamic_cast<FunctionAST *>(VarNames[i]) != nullptr)
    {
      FunctionAST *temp = dynamic_cast<FunctionAST *>(VarNames[i]);
      /* save var defs so far */
      Builder.SetInsertPoint(context.currentBlock());

      for (std::map<std::string, AllocaInst *>::iterator it = (context.getTop()->getLocals()).begin(); it != (context.getTop()->getLocals()).end(); ++it)
      {
        context.inherited()[(temp->Proto->getName()).c_str()].push_back(std::pair<std::string, Type *>(it->first, PointerType::getUnqual(it->second->getAllocatedType())));
      }
      temp->codegen();
      Builder.SetInsertPoint(context.currentBlock());
    }
  }

  Builder.SetInsertPoint(context.currentBlock());
  Value *BodyVal = Body->codegen();
  if (!BodyVal)
    return nullptr;

  return BodyVal;
}

/* IR for ⟨func-def⟩ */
Function *FunctionAST::codegen()
{
  // First, check for an existing function from a previous 'extern' declaration.
  Function *TheFunction = TheModule->getFunction(Proto->getName());

  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;

  if (!TheFunction->empty())
    return (Function *)LogErrorV("Function cannot be redefined.");

  for (int i = 0; i < context.id; i++)
    myfile << "\t";
  myfile << "New function: " << Proto->getName() << std::endl;

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  CodeGenBlock *prev = nullptr;
  if (context.id > 0)
    prev = context.getTop();
  context.pushBlock(BB, TheFunction);
  context.setPrev(prev);
  Builder.SetInsertPoint(context.currentBlock());

  if (context.id > 1)
  {
    context.locals() = prev->getLocals();
    context.locals_type() = prev->getLocals_Types();
  }

  // Record the function arguments in the NamedValues map.
  for (auto &Arg : TheFunction->args())
  {
    // Create an alloca for this variable
    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), Arg.getType());
    // Store the initial value into alloca
    Builder.CreateStore(&Arg, Alloca);
    // Add arguments to variable symbol table
    context.locals()[Arg.getName()] = Alloca;
    context.locals_type()[Arg.getName()] = Arg.getType();
  }

  if (Value *Val = Body->codegen())
  {
    // myfile << "\n";
    // for (int i = 0; i < context.id; i++)
    //   myfile << "\t";
    // myfile << "Inherited: " << context.getTop()->getInherited().size() << std::endl;
    // for (std::map<std::string, std::vector<std::pair<std::string, Type *>>>::iterator i = (context.inherited()).begin(); i != (context.inherited()).end(); ++i)
    // {
    //   for (int i = 0; i < context.id; i++)
    //     myfile << "\t";
    //   myfile << i->first << std::endl;
    //   for (std::vector<std::pair<std::string, Type *>>::iterator it = i->second.begin(); it != i->second.end(); ++it)
    //   {
    //     for (int i = 0; i < context.id; i++)
    //       myfile << "\t";
    //     myfile << (it->first) << "   " << (it->second) << std::endl;
    //   }
    // }
    // myfile << "\n";
    // for (int i = 0; i < context.id; i++)
    //   myfile << "\t";
    // myfile << "Locals: " << context.getTop()->getLocals().size() << std::endl;

    // for (std::map<std::string, AllocaInst *>::iterator it = (context.getTop()->getLocals()).begin(); it != (context.getTop()->getLocals()).end(); ++it)
    // {
    //   for (int i = 0; i < context.id; i++)
    //     myfile << "\t";
    //   myfile << (it->first) << "   " << (it->second) << std::endl;
    // }

    // Finish off the function.
    // ReturnInst::Create(TheContext, Val, Builder.GetInsertBlock());
    context.popBlock();

    // Validate the generated code, checking for consistency.
    verifyFunction(*TheFunction);
    return TheFunction;
  }
  else
  {
    ReturnInst::Create(TheContext, context.getCurrentReturnValue(), Builder.GetInsertBlock());
    context.popBlock();
    verifyFunction(*TheFunction);
    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();

  return nullptr;
}

/* ------------------------------------------- IR for Custom Functions ------------------------------------------- */

Value *WriteInteger::codegen()
{
  Value *n = p->codegen();
  n = loadValue(n);
  if (!((n->getType()->isIntegerTy(16))))
    LogErrorV("Variable is wrong type(Integer expected)");
  Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n});
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  return c8(0);
}

Value *WriteByte::codegen()
{
  Value *n = p->codegen();
  n = loadValue(n);
  if (!((n->getType()->isIntegerTy(8))))
    LogErrorV("Variable is wrong type(Byte expected)");
  Value *ch = Builder.CreateCall(TheWriteByte, std::vector<Value *>{n});
  Value *n16 = Builder.CreateZExtOrTrunc(ch, i16, "ext");
  Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n16});
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  return c8(0);
}

Value *WriteChar::codegen()
{
  Value *n = p->codegen();
  n = loadValue(n);
  if (!((n->getType()->isIntegerTy(8))))
    LogErrorV("Variable is wrong type(Byte expected)");
  Builder.CreateCall(TheWriteChar, std::vector<Value *>{n});
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  return c8(0);
}

Value *WriteString::codegen()
{
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Value *n = str->codegen();
  if (n->getType()->isPointerTy() && n->getType()->getPointerElementType()->isIntegerTy(8))
  {
    Builder.CreateCall(TheWriteString, std::vector<Value *>{n});
    Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  }
  else
  {
    LogErrorV("Parameter is wrong type (String expected)");
  }
  return c8(0);
}

Value *ReadInteger::codegen()
{
  Value *r = Builder.CreateCall(TheReadInteger, std::vector<Value *>());
  if (!(r->getType()->isIntegerTy(16)))
    LogErrorV("Wrong type of input. ReadInteger expects Integer type");
  return r;
}

Value *ReadByte::codegen()
{
  Value *r = Builder.CreateCall(TheReadInteger, std::vector<Value *>());
  Value *n8 = Builder.CreateZExtOrTrunc(r, i8, "ext");
  // if (!(r->getType()->isIntegerTy(8)))
  //   LogErrorV("Wrong type of input. ReadByte expects Byte type");
  return n8;
}

Value *ReadChar::codegen()
{
  Value *r = Builder.CreateCall(TheReadChar, std::vector<Value *>());
  if (!(r->getType()->isIntegerTy(8)))
    LogErrorV("Wrong type of input. ReadChar expects Byte type");
  return r;
}

Value *ReadString::codegen()
{
  Value *n = expr->codegen();
  Value *arr = array->codegen();
  Builder.CreateCall(TheReadString, std::vector<Value *>{n, arr});
  return c16(0);
}

Value *Extend::codegen()
{
  Value *n = expr->codegen();
  n = loadValue(n);
  if (!(n->getType()->isIntegerTy(8)))
    LogErrorV("Wrong type of input. Extend expects Byte type");
  return Builder.CreateZExtOrTrunc(n, i16, "ext");
}

Value *Shrink::codegen()
{
  Value *n = expr->codegen();
  n = loadValue(n);
  if (!(n->getType()->isIntegerTy(16)))
    LogErrorV("Wrong type of input. Shrink expects int type");
  return Builder.CreateZExtOrTrunc(n, i8, "ext");
}

Value *Strlen::codegen()
{
  Value *n = Arr->codegen();
  return Builder.CreateCall(TheStrlen, std::vector<Value *>{n}, "strlen");
}

Value *Strcmp::codegen()
{
  Value *arr1 = LArr->codegen();
  Value *arr2 = RArr->codegen();
  return Builder.CreateCall(TheStrcmp, std::vector<Value *>{arr1, arr2});
}

Value *Strcpy::codegen()
{
  Value *arr1 = LArr->codegen();
  Value *arr2 = RArr->codegen();
  return Builder.CreateCall(TheStrcpy, std::vector<Value *>{arr1, arr2});
}

Value *Strcat::codegen()
{
  Value *arr1 = LArr->codegen();
  Value *arr2 = RArr->codegen();
  return Builder.CreateCall(TheStrcat, std::vector<Value *>{arr1, arr2});
}