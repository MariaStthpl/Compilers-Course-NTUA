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
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

static Function *TheWriteInteger;
static Function *TheWriteByte;
static Function *TheWriteChar;
static Function *TheWriteString;

static Function *TheReadInteger;
static Function *TheReadByte;
static Function *TheReadChar;
static Function *TheReadString;

static Function *TheStrlen;
static Function *TheStrcmp;
static Function *TheStrcpy;
static Function *TheStrcat;

// Useful LLVM types.
static Type *proc = Type::getVoidTy(TheContext);
static Type *i8 = IntegerType::get(TheContext, 8);
static Type *i16 = IntegerType::get(TheContext, 16);

// Useful LLVM helper functions.
inline ConstantInt *c1(char c)
{
  return ConstantInt::get(TheContext, APInt(1, c, true));
}

inline ConstantInt *c8(char c)
{
  return ConstantInt::get(TheContext, APInt(8, c, true));
}

inline ConstantInt *c16(int n)
{
  return ConstantInt::get(TheContext, APInt(16, n, true));
}

inline ConstantInt *c32(int n)
{
  return ConstantInt::get(TheContext, APInt(32, n, true));
}

Scopes context;

void llvm_compile_and_dump(FunctionAST *t)
{
  myfile.open("ast.txt");

  // Initialize the module and the optimization passes.
  TheModule = make_unique<Module>("alan program", TheContext);
  TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
  // TheFPM->add(createPromoteMemoryToRegisterPass());
  // TODO optimizations
  // TheFPM->add(createGVNPass());
  // TheFPM->add(createCFGSimplificationPass());
  // TheFPM->add(createDeadCodeEliminationPass());
  // TheFPM->add(createDeadInstEliminationPass());
  // for (int i = 0; i < 5; ++i)
  //   TheFPM->add(createDeadStoreEliminationPass());
  // TheFPM->add(createInstructionCombiningPass());
  // TheFPM->add(createReassociatePass());
  // TheFPM->add(createGVNPass());
  // TheFPM->add(createCFGSimplificationPass());
  TheFPM->doInitialization();

  /* ---------------------------------------------------------------------------*/
  /*                    declare functions of runtime library                    */
  /* ---------------------------------------------------------------------------*/

  /*----------------------------   WRITE functions  ----------------------------*/

  // writeInteger (n : int) : proc
  FunctionType *writeInteger_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i16}, false);
  TheWriteInteger =
      Function::Create(writeInteger_type, Function::ExternalLinkage,
                       "writeInteger", TheModule.get());

  //  writeByte (b : byte) : proc
  FunctionType *writeByte_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i8}, false);
  TheWriteByte =
      Function::Create(writeByte_type, Function::ExternalLinkage,
                       "writeByte", TheModule.get());
  Function::arg_iterator arg = TheWriteByte->arg_begin();
  Value *byte = &(*arg);
  BasicBlock *b = BasicBlock::Create(TheContext, "", TheWriteByte, 0);
  Builder.SetInsertPoint(b);
  Value *retint = Builder.CreateSExt(byte, i16);
  Builder.CreateCall(TheWriteInteger, {retint});
  Builder.CreateRet(nullptr);

  //  writeChar (b : byte) : proc
  FunctionType *writeChar_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i8}, false);
  TheWriteChar =
      Function::Create(writeChar_type, Function::ExternalLinkage,
                       "writeChar", TheModule.get());

  // writeString (s : reference byte []) : proc
  FunctionType *writeString_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::get(i8, 0)}, false);
  TheWriteString =
      Function::Create(writeString_type, Function::ExternalLinkage,
                       "writeString", TheModule.get());

  /*----------------------------   READ functions  ----------------------------*/

  // readInteger () : int
  FunctionType *readInteger_type =
      FunctionType::get(Type::getInt16Ty(TheContext),
                        std::vector<Type *>(), false);
  TheReadInteger =
      Function::Create(readInteger_type, Function::ExternalLinkage,
                       "readInteger", TheModule.get());

  // readByte () : byte
  FunctionType *readByte_type =
      FunctionType::get(Type::getInt8Ty(TheContext),
                        std::vector<Type *>(), false);
  TheReadByte =
      Function::Create(readByte_type, Function::ExternalLinkage,
                       "readByte", TheModule.get());

  // readChar () : byte
  FunctionType *readChar_type =
      FunctionType::get(Type::getInt8Ty(TheContext),
                        std::vector<Type *>(), false);
  TheReadChar =
      Function::Create(readChar_type, Function::ExternalLinkage,
                       "readChar", TheModule.get());

  // readString (n : int, s : reference byte []) : proc
  FunctionType *readString_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i16, PointerType::getUnqual(i8)}, false);
  TheReadString =
      Function::Create(readString_type, Function::ExternalLinkage,
                       "readString", TheModule.get());

  /*----------------------------   CONVERSION functions  ----------------------------*/

  // extend (b : byte) : int
  FunctionType *extend_type =
      FunctionType::get(Type::getInt16Ty(TheContext), std::vector<Type *>{i8}, false);
  Function *TheExtend =
      Function::Create(extend_type, Function::ExternalLinkage, "extend", TheModule.get());
  arg = TheExtend->arg_begin();
  Value *int8_c = &(*arg);
  BasicBlock *bl = BasicBlock::Create(TheContext, "", TheExtend, 0);
  Builder.SetInsertPoint(bl);
  Value *ret = Builder.CreateSExt(int8_c, i16);
  Builder.CreateRet(ret);

  // shrink (i : int) : byte
  FunctionType *shrink_type =
      FunctionType::get(i8, std::vector<Type *>{i16}, false);
  Function *TheShrink =
      Function::Create(shrink_type, Function::ExternalLinkage, "shrink", TheModule.get());
  arg = TheShrink->arg_begin();
  Value *int16_c = &(*arg);
  BasicBlock *bl2 = BasicBlock::Create(TheContext, "", TheShrink, 0);
  Builder.SetInsertPoint(bl2);
  ret = Builder.CreateTrunc(int16_c, i8);
  Builder.CreateRet(ret);

  /*----------------------------   STRING functions  ----------------------------*/

  // strlen (s : reference byte []) : int
  FunctionType *strlen_type =
      FunctionType::get(Type::getInt16Ty(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8)}, false);
  TheStrlen =
      Function::Create(strlen_type, Function::ExternalLinkage,
                       "strlen", TheModule.get());
  // strcmp (s1 : reference byte [], s2 : reference byte []) : int
  FunctionType *strcmp_type =
      FunctionType::get(Type::getInt16Ty(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8), PointerType::getUnqual(i8)}, false);
  TheStrcmp =
      Function::Create(strcmp_type, Function::ExternalLinkage,
                       "strcmp", TheModule.get());
  // strcpy (trg : reference byte [], src : reference byte []) : proc
  FunctionType *strcpy_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8), PointerType::getUnqual(i8)}, false);
  TheStrcpy =
      Function::Create(strcpy_type, Function::ExternalLinkage,
                       "strcpy", TheModule.get());
  // strcat (trg : reference byte [], src : reference byte []) : proc
  FunctionType *strcat_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::getUnqual(i8), PointerType::getUnqual(i8)}, false);
  TheStrcat =
      Function::Create(strcat_type, Function::ExternalLinkage,
                       "strcat", TheModule.get());

  // Define and start the main function.
  Function *main_function = t->codegen();
  if (t->Proto->getName() != "main")
  {
    auto fun = TheModule->getFunction("main");
    main_function = TheModule->getFunction(t->Proto->getName());
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
  TheFPM->run(*main_function);
  // Print out the IR.
  TheModule->print(outs(), nullptr);
}

Value *LogError(std::string Str)
{
  fprintf(stderr, "\nError: %s\n\n", Str.c_str());
  return nullptr;
}

Value *loadValue(Value *p)
{
  if (PointerType::classof(p->getType()))
    return Builder.CreateLoad(p, "var");
  else
    return p;
}

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of the function.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, Type *t)
{
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                   TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(t, 0, VarName.c_str());
}

Type *getDataType(int n)
{
  switch (n)
  {
  case 0:
    return proc;
  case 8:
    return i8;
  case 16:
    return i16;
  default:
    return proc;
  }
}

/* ------------------------------------------- IR for ExprAST ------------------------------------------- */

// IR for <int-const>
Value *IntConst_ExprAST::codegen()
{
  return c16(Val);
}

Type *IntConst_ExprAST::getT()
{
  return i16;
}

// IR for <char-const>
Value *CharConst_ExprAST::codegen()
{
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
  AllocaInst *V = cast<AllocaInst>(std::get<1>(context.symbol_table()[Name]));
  if (!V)
  {
    LogError("Unknown variable name: " + Name);
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
    return V;
  }

  return V;
}

Type *Id_ExprAST::getT()
{
  Value *V = std::get<1>(context.symbol_table()[Name]);
  if (!V)
  {
    LogError("Unknown variable name: " + Name);
    exit(1);
  }

  Type *idT = (std::get<1>(context.symbol_table()[Name]))->getType()->getPointerElementType();
  // return idT->getPointerElementType();
  if (PointerType::classof(idT))
  {
    if (ArrayType::classof(idT->getPointerElementType()))
      return idT->getPointerElementType();
    else
      return idT->getPointerElementType();
  }
  else if (ArrayType::classof(idT))
    return idT;
  else
    return idT;
}

// IR for <id>[<expr>]
Value *ArrayElement_ExprAST::codegen()
{

  AllocaInst *arr = cast<AllocaInst>(std::get<1>(context.symbol_table()[Name]));
  if (!arr)
  {
    LogError("WTF Unknown array name: " + Name);
    exit(1);
  }

  Value *index = expr->codegen();
  index = loadValue(index);

  if (PointerType::classof(arr->getAllocatedType()))
  {
    if (ArrayType::classof(arr->getAllocatedType()->getPointerElementType()))
    {
      // array element (in nested function) with pointer of ArrayType
      std::vector<Value *> indexList;
      indexList.push_back(c32(0));
      indexList.push_back(index);
      LoadInst *ldinst = Builder.CreateLoad(arr, Name);
      GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((arr->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), Name, Builder.GetInsertBlock());
      return gepInst;
    }
    else
    {
      // array element (in nested function) with pointer of IntegerType
      std::vector<Value *> indexList;
      indexList.push_back(index);
      LoadInst *ldinst = Builder.CreateLoad(arr, Name);
      GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((arr->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), Name, Builder.GetInsertBlock());
      return gepInst;
    }
  }
  else
  {
    // array element of ArrayType
    std::vector<Value *> indexList;
    indexList.push_back(c32(0));
    indexList.push_back(index);
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(arr->getAllocatedType(), arr, ArrayRef<Value *>(indexList), Name, Builder.GetInsertBlock());
    return gepInst;
  }
}

Type *ArrayElement_ExprAST::getT()
{
  TypeCheck();
  Type *idT = (std::get<1>(context.symbol_table()[Name]))->getType()->getPointerElementType();
  if (PointerType::classof(idT))
  {
    if (ArrayType::classof(idT->getPointerElementType()))
      return idT->getPointerElementType()->getArrayElementType();
    else
      return idT->getPointerElementType();
  }
  else if (ArrayType::classof(idT))
    return idT->getArrayElementType();
  else
    return idT;
}

void ArrayElement_ExprAST::TypeCheck()
{
  int isarray = std::get<3>(context.symbol_table()[Name]);
  if (isarray!=1)
  {
    LogError("Unknown array name: " + Name);
    exit(1);
  }
  Type *indexT = expr->getT();
  if (!(indexT->isIntegerTy(16)))
  {
    LogError("Index of array " + Name + "[] must be of int type");
    exit(1);
  }
  return;
}

// IR for string-literal
Value *StringLiteral_ExprAST::codegen()
{
  std::vector<llvm::Constant *> values;
  size_t i;
  for (i = 1; i < string_literal.length() - 1; i++)
  {
    values.push_back(c8(string_literal[i]));
  }
  values.push_back(c8('\0'));

  ArrayType *string_type = ArrayType::get(i8, string_literal.length() - 1);
  GlobalVariable *TheString = new GlobalVariable(
      *TheModule, string_type, false, GlobalValue::InternalLinkage,
      ConstantArray::get(string_type,
                         values),
      "string_literal");

  Value *idxList[] = {c32(0), c32(0)};
  return GetElementPtrInst::CreateInBounds(string_type, TheString, ArrayRef<Value *>(idxList), "str_ptr", Builder.GetInsertBlock());
}

Type *StringLiteral_ExprAST::getT()
{
  return PointerType::getUnqual(i8);
}

// IR for ⟨expr⟩ ( '+' | '-' | '*' | '/' | '%' ) ⟨expr⟩
Value *ArithmeticOp_ExprAST::codegen()
{
  TypeCheck();

  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  Value *temp1 = loadValue(L);
  Value *temp2 = loadValue(R);

  L = temp1;
  R = temp2;

  switch (Op)
  {
  case PLUS:
    return Builder.CreateAdd(L, R, "addtmp");
  case MINUS:
    return Builder.CreateSub(L, R, "subtmp");
  case TIMES:
    return Builder.CreateMul(L, R, "multmp");
  case DIV:
    return Builder.CreateUDiv(L, R, "modtmp");
  case MOD:
    return Builder.CreateURem(L, R, "modtmp");
  default:
    return LogError("invalid binary operator");
  }
}

Type *ArithmeticOp_ExprAST::getT()
{
  Type *LT = LHS->getT();
  Type *RT = RHS->getT();

  if (!Unary)
  {
    if (LT != RT)
    {
      LogError("\noperands of arithmetic operation must be of same type\n");
      exit(1);
    }
  }
  return RT;
}

void ArithmeticOp_ExprAST::TypeCheck()
{
  Type *LT = LHS->getT();
  Type *RT = RHS->getT();

  if (!Unary)
    if (LT != RT)
    {
      LogError("\noperands of arithmetic operation must be of same type\n");
      exit(1);
    }
  return;
}

/* ------------------------------------------- IR for CondAST ------------------------------------------- */

// IR for ⟨expr⟩( '==' | '!=' | '<' | '>' | '<=' | '>=' )⟨expr⟩
Value *ComparisonOp_CondAST::codegen()
{
  TypeCheck();

  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l || !r)
    return nullptr;

  l = loadValue(l);
  r = loadValue(r);

  switch (Op)
  {
  case L:
    return Builder.CreateICmpSLT(l, r, "ltmp");
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
    return LogError("invalid binary operator");
  }
}

void ComparisonOp_CondAST::TypeCheck()
{
  Type *LT = LHS->getT();
  Type *RT = RHS->getT();

  if (LT != RT)
  {
    LogError("\noperands of comparison operation must be of same type\n");
    exit(1);
  }
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
    return LogError("invalid binary operator");
  }
}

void LogicalOp_CondAST::TypeCheck()
{
  Type *LT = LHS->getT();
  Type *RT = RHS->getT();

  if (LT != RT)
  {
    LogError("\noperands of logical operation must be of same type\n");
    exit(1);
  }
  return;
}

/* ------------------------------------------- IR for StmtAST ------------------------------------------- */

// IR for ⟨l-value⟩ '=' ⟨expr⟩
Value *Assignment_StmtAST::codegen()
{
  TypeCheck();

  // l-value is always a pointer
  Value *V = LHS->codegen();

  // evaluate r-value
  Value *Val = RHS->codegen();
  if (!Val)
    return nullptr;

  // if r-value is pointer, load its value
  Val = loadValue(Val);

  // special case: assign string_literal to array
  // TODO assign array to array ???
  if (StringLiteral_ExprAST *str = dynamic_cast<StringLiteral_ExprAST *>(RHS))
  {
    Id_ExprAST *idexpr = dynamic_cast<Id_ExprAST *>(LHS);
    size_t i;
    for (i = 1; i < str->getString().length() - 1; i++)
    {
      Assignment_StmtAST *assignelement = new Assignment_StmtAST(
          new ArrayElement_ExprAST(idexpr->getName(), new IntConst_ExprAST(i - 1)),
          new CharConst_ExprAST(str->getString()[i]));
      assignelement->codegen();
    }
    Assignment_StmtAST *assignelement = new Assignment_StmtAST(
        new ArrayElement_ExprAST(idexpr->getName(), new IntConst_ExprAST(i - 1)),
        new CharConst_ExprAST('\0'));
    assignelement->codegen();
    return c8(0);
  }
  else
    return Builder.CreateStore(Val, V);
}

void Assignment_StmtAST::TypeCheck()
{
  Type *LT = LHS->getT();
  Type *RT = RHS->getT();

  if (LT->isFunctionTy() || LT->isFunctionTy())
  {
    LogError("operands of assignement operation cannot be functions");
    exit(1);
  }

  if (LT != RT)
  {
    LogError("operands of assignement operation must be of same type");
    exit(1);
  }
  return;
}

// IR for ⟨compound-stmt⟩
Value *CompoundStmt_StmtAST::codegen()
{
  StatementList::const_iterator it;
  Value *last = NULL;
  for (it = statements.begin(); it != statements.end(); it++)
  {
    last = (**it).codegen();
  }
  return last;
}

// IR for ⟨func-call⟩
Value *FuncCall::codegen()
{
  TypeCheck();

  // Look up the name in the global module table.
  Function *CalleeF = dyn_cast<Function>(std::get<1>(context.symbol_table()[Callee])); //TheModule->getFunction(((context.symbol_table()[Callee]).first));

  std::vector<Value *> ArgsV;
  FunctionType *FTy = CalleeF->getFunctionType();
  for (unsigned i = 0, e = Args.size(); i != e; ++i)
  {
    // check if argument pass by reference
    if (PointerType::classof(FTy->getParamType(i)))
      ArgsV.push_back(Args[i]->codegen());
    else
      ArgsV.push_back(loadValue(Args[i]->codegen()));
    if (!ArgsV.back())
      return nullptr;
  }

  for (std::map<std::string, std::pair<Type *, AllocaInst *>>::iterator it = (context.inherited()[Callee]).begin(); it != (context.inherited()[Callee]).end(); ++it)
  //Passing pointers to hidden arguments
  {
    if (PointerType::classof((std::get<1>(context.symbol_table()[it->first]))->getType()) && PointerType::classof((std::get<1>(context.symbol_table()[it->first]))->getType()->getPointerElementType()))
    {
      LoadInst *ldinst = Builder.CreateLoad(std::get<1>(context.symbol_table()[it->first]), it->first);
      ArgsV.push_back(ldinst);
      // indexList.push_back(c16(0));
      // GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((context.symbol_table()[it->first].second)->getType(), (context.symbol_table()[it->first].second), ArrayRef<Value *>(indexList),  it->first, Builder.GetInsertBlock());
      // LoadInst *ldinst = Builder.CreateLoad(gepInst, it->first);
      // ArgsV.push_back(ldinst);
    }
    else if (PointerType::classof((std::get<1>(context.symbol_table()[it->first]))->getType()))
    {
      std::vector<Value *> indexList;
      indexList.push_back(c16(0));
      GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(std::get<1>(context.symbol_table()[it->first])->getType()->getPointerElementType(), std::get<1>(context.symbol_table()[it->first]), ArrayRef<Value *>(indexList), it->first, Builder.GetInsertBlock());
      // LoadInst *ldinst = Builder.CreateLoad(gepInst, it->first);
      ArgsV.push_back(gepInst);
    }
    else
    {
      std::vector<Value *> indexList;
      indexList.push_back(c16(0));
      GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(std::get<1>(context.symbol_table()[it->first])->getType(), std::get<1>(context.symbol_table()[it->first]), ArrayRef<Value *>(indexList), it->first, Builder.GetInsertBlock());
      ArgsV.push_back(gepInst);
    }
  }
  return Builder.CreateCall(CalleeF, ArgsV);
}

Type *FuncCall::getT()
{
  Function *CalleeF = TheModule->getFunction(((std::get<0>(context.symbol_table()[Callee]))));
  return CalleeF->getFunctionType()->getReturnType();
}

void FuncCall::TypeCheck()
{
  // Look up the name in the global module table.
  Function *CalleeF = dyn_cast<Function>(std::get<1>(context.symbol_table()[Callee])); //TheModule->getFunction(((context.symbol_table()[Callee]).first));
  if (!CalleeF)
  {
    LogError("Unknown function referenced: " + Callee);
    exit(1);
  }

  // Check argument mismatch error: Except the real parameters, we take into consideration the #pointers of previous variables' declaration
  if (CalleeF->arg_size() != (Args.size() + (context.getTop()->getInherited()[Callee]).size()))
  {
    LogError("Incorrect # arguments passed in function: " + Callee);
    exit(1);
  }

  FunctionType *FTy = CalleeF->getFunctionType();
  for (unsigned i = 0, e = Args.size(); i != e; ++i)
  {
    if (PointerType::classof(FTy->getParamType(i)))
    {
      if (!dynamic_cast<Id_ExprAST *>(Args[i]) && !dynamic_cast<ArrayElement_ExprAST *>(Args[i]) && !dynamic_cast<StringLiteral_ExprAST *>(Args[i]))
      {
        LogError("Calling function with wrong argument type, arg no: " + std::to_string(i + 1));
        exit(1);
      }
    }
    else
    {
      if (Args[i]->getT() != FTy->getParamType(i))
      {
        LogError("Calling function with wrong argument type, arg no: " + std::to_string(i + 1));
        exit(1);
      }
    }
  }
  return;
}

// IR for “if” '(' ⟨cond⟩ ')' ⟨stmt⟩ [ “else” ⟨stmt⟩ ]
Value *If_StmtAST::codegen()
{
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  CondV = Builder.CreateICmpNE(CondV, c1(0), "ifcond");

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  Builder.SetInsertPoint(ThenBB);
  Value *ThenV = Then->codegen();
  if (!ThenV)
    return nullptr;

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

  if (!WhileV)
    return nullptr;

  WhileV = Builder.CreateICmpNE(WhileV, c1(0), "while_entry");

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateCondBr(WhileV, WhileBB, MergeBB);

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);

  return c32(0);
}

// IR for “return” [ ⟨expr⟩ ]
Value *Return_Stmt::codegen()
{
  TypeCheck();
  if (expr == nullptr)
  {
    context.setCurrentReturnValue(nullptr);
    if (!Builder.GetInsertBlock()->getTerminator())
      ReturnInst::Create(TheContext, context.getCurrentReturnValue(), Builder.GetInsertBlock());
    return c32(0);
  }

  Value *returnValue = expr->codegen();
  returnValue = loadValue(returnValue);
  context.setCurrentReturnValue(returnValue);
  if (!Builder.GetInsertBlock()->getTerminator())
    ReturnInst::Create(TheContext, context.getCurrentReturnValue(), Builder.GetInsertBlock());
  return returnValue;
}

void Return_Stmt::TypeCheck()
{
  Type *ret = context.getTop()->getFunction()->getFunctionType()->getReturnType();

  if (!expr)
  {
    if (!(ret->isVoidTy()))
    {
      LogError("Return type of function doesn't match definition type which is void");
      exit(1);
    }
  }
  else if (ret != expr->getT())
  {
    LogError("Return type of function " + std::string((context.getTop()->getFunction()->getName())) + " doesn't match definition type ");
    exit(1);
  }
  return;
}

/* ------------------------------------------- IR for Function ------------------------------------------- */

Function *PrototypeAST::codegen()
{
  argsize = Args.size();
  /* pass previous variables as pointers */
  if (context.id > 0)
    for (std::map<std::string, std::pair<Type *, AllocaInst *>>::iterator it = (context.getTop())->getInherited()[Name].begin(); it != (context.getTop())->getInherited()[Name].end(); ++it)
      Args.push_back(std::pair<std::string, Type *>(it->first, (it->second).first));

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

      // std::tuple<std::string, Value *, int> locals = context.symbol_table()[VarName];
      if ((context.symbol_table()).find(VarName) != (context.symbol_table()).end())
      {
        myfile << "TRYing to redefine variable " << VarName << std::endl;

        //check that function is defined only in higher scope and not the current one
        if (!(std::get<2>(context.symbol_table()[VarName]) < context.getScopeId()))
        {
          myfile << "WTF" << VarName << std::endl;
          LogError("Cannot redefine variable in scope: " + std::to_string(context.getScopeId()) + " Variable Name is: " + VarName);
          LogError("declared in scope: " + std::to_string(std::get<2>(context.symbol_table()[VarName])) + " Variable Name is: " + VarName);
          exit(1);
        }
      }

      if ( t->isArrayTy() )
      context.symbol_table()[VarName] = std::tuple<std::string, Value *, int, int>(VarName, Alloca, context.getScopeId(), 1); //std::pair<std::string, Value *>(VarName, Alloca);
      else 
      context.symbol_table()[VarName] = std::tuple<std::string, Value *, int, int>(VarName, Alloca, context.getScopeId(), 0); //std::pair<std::string, Value *>(VarName, Alloca);

    }
    // <func-def>
    else if (dynamic_cast<FunctionAST *>(VarNames[i]) != nullptr)
    {
      FunctionAST *temp = dynamic_cast<FunctionAST *>(VarNames[i]);
      Builder.SetInsertPoint(context.currentBlock());

      const std::string &FuncName = temp->Proto->getName();

      // std::tuple<std::string, Value *, int> locals = context.symbol_table()[FuncName];
      if ((context.symbol_table()).find(FuncName) != (context.symbol_table()).end())
      {
        myfile << "TRYing to redefine function " << FuncName << std::endl;

        //check that function is defined only in higher scope and not the current one
        if (!(std::get<2>(context.symbol_table()[FuncName]) < context.getScopeId()))
        {
          myfile << "WTF" << FuncName << std::endl;
          LogError("Cannot redefine function in same scope. Function Name is: " + FuncName);
          exit(1);
        }
      }

      /* save var defs so far */
      for (std::map<std::string, std::tuple<std::string, Value *, int, int>>::iterator it = (context.symbol_table()).begin(); it != (context.symbol_table()).end(); ++it)
      {
        if (dyn_cast<AllocaInst>(std::get<1>(it->second)))
          ((context.inherited()[FuncName])[it->first]) = (std::pair<Type *, AllocaInst *>(PointerType::getUnqual(cast<AllocaInst>(std::get<1>(it->second))->getAllocatedType()), cast<AllocaInst>(std::get<1>(it->second))));
      }

      Function *fun = temp->codegen();
      context.symbol_table()[FuncName] = std::tuple<std::string, Value *, int, int>(fun->getName(), fun, context.getScopeId(), 2); //std::pair<std::string, Value *>(fun->getName(), fun);

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

  TheFunction = Proto->codegen();

  for (int i = 0; i < context.id; i++)
    myfile << "\t";
  myfile << "New function: " << Proto->getName() << std::endl;

  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  Scope *prev = nullptr;
  if (context.id > 0)
    prev = context.getTop();
  context.pushBlock(BB, TheFunction);
  // if (context.id > 0) // main can call itself?
  context.setPrev(prev);
  Builder.SetInsertPoint(context.currentBlock());

  if (context.id == 1)
  {
    context.symbol_table()["writeInteger"] = std::tuple<std::string, Value *, int, int>("writeInteger", TheModule->getFunction("writeInteger"), 0, 2);
    context.symbol_table()["writeChar"] = std::tuple<std::string, Value *, int, int>("writeChar", TheModule->getFunction("writeChar"), 0, 2);
    context.symbol_table()["writeByte"] = std::tuple<std::string, Value *, int, int>("writeByte", TheModule->getFunction("writeByte"), 0, 2);
    context.symbol_table()["writeString"] = std::tuple<std::string, Value *, int, int>("writeString", TheModule->getFunction("writeString"), 0, 2);
    context.symbol_table()["readInteger"] = std::tuple<std::string, Value *, int, int>("readInteger", TheModule->getFunction("readInteger"), 0, 2);
    context.symbol_table()["readByte"] = std::tuple<std::string, Value *, int, int>("readByte", TheModule->getFunction("readByte"), 0, 2);
    context.symbol_table()["readChar"] = std::tuple<std::string, Value *, int, int>("readChar", TheModule->getFunction("readChar"), 0, 2);
    context.symbol_table()["readString"] = std::tuple<std::string, Value *, int, int>("readString", TheModule->getFunction("readString"), 0, 2);
    context.symbol_table()["extend"] = std::tuple<std::string, Value *, int, int>("extend", TheModule->getFunction("extend"), 0, 2);
    context.symbol_table()["shrink"] = std::tuple<std::string, Value *, int, int>("shrink", TheModule->getFunction("shrink"), 0, 2);
    context.symbol_table()["strlen"] = std::tuple<std::string, Value *, int, int>("strlen", TheModule->getFunction("strlen"), 0, 2);
    context.symbol_table()["strcmp"] = std::tuple<std::string, Value *, int, int>("strcmp", TheModule->getFunction("strcmp"), 0, 2);
    context.symbol_table()["strcpy"] = std::tuple<std::string, Value *, int, int>("strcpy", TheModule->getFunction("strcpy"), 0, 2);
    context.symbol_table()["strcat"] = std::tuple<std::string, Value *, int, int>("strcat", TheModule->getFunction("strcat"), 0, 2);
  }

  if (context.id > 1)
  {
    context.inherited() = prev->getInherited();
    context.symbol_table() = prev->getSymbolTable();
    context.symbol_table()[Proto->getName()] = std::tuple<std::string, Value *, int, int>(TheFunction->getName(), TheFunction, context.getScopeId() - 1, 2); //pushes itself on local definitions
  }

  int counter = 0;
  for (auto &Arg : TheFunction->args())
  {
    // Create an alloca for this variable
    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), Arg.getType());
    // Store the initial value into alloca
    Builder.CreateStore(&Arg, Alloca);
    // Add arguments to variable symbol table
    if (counter < Proto->getArgsSize()) {
      if ( PointerType::classof((Proto->getArgs()[counter]).second) && PointerType::classof(((Proto->getArgs()[counter]).second)->getPointerElementType()) )
      context.symbol_table()[Arg.getName()] = std::tuple<std::string, Value *, int, int>(Arg.getName(), Alloca, context.getScopeId(), 1);
      else
      context.symbol_table()[Arg.getName()] = std::tuple<std::string, Value *, int, int>(Arg.getName(), Alloca, context.getScopeId(), 0);
    }
    else {//Hidden arguments
      if ( PointerType::classof(Arg.getType()) && ArrayType::classof(Arg.getType()->getPointerElementType()))
      context.symbol_table()[Arg.getName()] = std::tuple<std::string, Value *, int, int>(Arg.getName(), Alloca, context.getScopeId() - 1, 1);
      else
      context.symbol_table()[Arg.getName()] = std::tuple<std::string, Value *, int, int>(Arg.getName(), Alloca, context.getScopeId() - 1, 0);
    }
    counter++;
  }



  Body->codegen();

  // debug print
  myfile << "\n";
  for (int i = 0; i < context.id; i++)
    myfile << "\t";
  myfile << "Childs: " << context.getTop()->getInherited().size() << std::endl;
  for (std::map<std::string, std::map<std::string, std::pair<Type *, AllocaInst *>>>::iterator i = (context.inherited()).begin(); i != (context.inherited()).end(); ++i)
  {
    for (int i = 0; i < context.id; i++)
      myfile << "\t";
    myfile << i->first << " - " << (i->second).size() << std::endl;
    for (std::map<std::string, std::pair<Type *, AllocaInst *>>::iterator it = i->second.begin(); it != i->second.end(); ++it)
    {
      for (int i = 0; i < context.id; i++)
        myfile << "\t";
      myfile << "\t";
      myfile << (it->first) << std::endl;
    }
  }
  myfile << "\n";
  for (int i = 0; i < context.id; i++)
    myfile << "\t";
  myfile << "symbol_table: " << context.symbol_table().size() << std::endl;

  for (std::map<std::string, std::tuple<std::string, Value *, int, int>>::iterator it = (context.symbol_table()).begin(); it != (context.symbol_table()).end(); ++it)
  {
    for (int i = 0; i < context.id; i++)
      myfile << "\t";
    myfile << (it->first) << "   " << std::get<1>(it->second) << " scope:" << std::to_string(std::get<2>(it->second)) << "   " << std::to_string(std::get<3>(it->second)) << std::endl;
  }
  myfile << "\n";

  if ((!context.getCurrentReturnValue() && !context.getTop()->getFunction()->getFunctionType()->getReturnType()->isVoidTy()) || (context.getCurrentReturnValue() && (context.getCurrentReturnValue()->getType() != context.getTop()->getFunction()->getFunctionType()->getReturnType())))
  {
    LogError("Return type of function doesn't match definition type");
    exit(1);
  }

  // Finish off the function.
  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateRet(context.getCurrentReturnValue());
  context.popBlock();

  // Validate the generated code, checking for consistency.
  verifyFunction(*TheFunction);
  TheFPM->run(*TheFunction);
  return TheFunction;

  // Error reading body, remove function.
  TheFunction->eraseFromParent();

  return nullptr;
}