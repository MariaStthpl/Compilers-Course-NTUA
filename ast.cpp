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
static Function *TheWriteString;
static Function *TheReadInteger;
// static Function *TheReadByte;
// static Function *TheReadString;

// Useful LLVM types.
static Type *i8 = IntegerType::get(TheContext, 8);
static Type *i16 = IntegerType::get(TheContext, 16);
static Type *i32 = IntegerType::get(TheContext, 32);
// static Type *i64 = IntegerType::get(TheContext, 64);

// Useful LLVM helper functions.
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
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{i8}, false);
  TheWriteByte =
      Function::Create(writeByte_type, Function::ExternalLinkage,
                       "writeChar", TheModule.get());
  // declare void @writeString(i8*)
  FunctionType *writeString_type =
      FunctionType::get(Type::getVoidTy(TheContext),
                        std::vector<Type *>{PointerType::get(i8, 0)}, false);
  TheWriteString =
      Function::Create(writeString_type, Function::ExternalLinkage,
                       "writeString", TheModule.get());

  FunctionType *readInteger_type =
      FunctionType::get(Type::getInt8Ty(TheContext),
                        std::vector<Type *>(), false);
  TheReadInteger =
      Function::Create(readInteger_type, Function::ExternalLinkage,
                       "readInteger", TheModule.get());
  // FunctionType *readByte_type =
  //   FunctionType::get(Type::getInt8Ty(TheContext),
  //                       std::vector<Type *>(), false);
  // TheReadByte =
  //     Function::Create(readByte_type, Function::ExternalLinkage,
  //                      "readInteger", TheModule.get());

  // Define and start the main function.
  t->codegen();

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
  // myfile << "<int-const>: ";
  return c16(Val);
}

// IR for <char-const>
Value *CharConst_ExprAST::codegen()
{
  // myfile << "<char-const>: " << Val;
  return c8(Val);
}

// IR for <id>
Value *Id_ExprAST::codegen()
{
  // myfile << "<id>: " << Name;
  // Look this variable up in the function.
  CodeGenBlock *block = context.getTop();
  AllocaInst *V;
  do
  {
    V = block->getLocals()[Name];
    if (!V)
      block = context.getPrev(block);
    else
    {
      if (PointerType::classof(V->getAllocatedType()))
      {
        // id in function -> using pointer
        std::vector<Value *> indexList;
        indexList.push_back(c16(0));
        LoadInst *ldinst = Builder.CreateLoad(V, Name);
        GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((V->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), Name, context.currentBlock());
        return Builder.CreateLoad(gepInst, Name);
      }
      else
        return Builder.CreateLoad(V, Name.c_str());
    }
  } while (block != nullptr);

  if (!V)
    LogErrorV("Unknown variable name");
  return V;
}

// IR for <id>[<expr>]
Value *ArrayElement_ExprAST::codegen()
{
  // myfile << "<id>[<expr>]: " << Name << "[" << dyn_cast<ConstantInt>(getExpr())->getSExtValue() << "]" << std::endl;

  AllocaInst *arr = context.locals()[Name];
  Value *index = expr->codegen();

  if (PointerType::classof(arr->getAllocatedType()))
  {
    // array element in function -> using pointer
    std::vector<Value *> indexList;
    indexList.push_back(index);
    LoadInst *ldinst = Builder.CreateLoad(arr, Name);
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds((arr->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), Name, context.currentBlock());
    return Builder.CreateLoad(gepInst, Name);
    // Builder.CreateLoad(gepInst, Name);
    // return arr;
  }
  else
  {
    // array element in main
    std::vector<Value *> indexList;
    indexList.push_back(c16(0));
    indexList.push_back(index);
    Builder.CreateLoad(arr, Name); // needed?
    GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(arr->getAllocatedType(), arr, ArrayRef<Value *>(indexList), Name, context.currentBlock());
    return Builder.CreateSExtOrBitCast(Builder.CreateLoad(gepInst, Name), i16);
    // Builder.CreateSExtOrBitCast(Builder.CreateLoad(gepInst, Name), i16);
    // return arr;
  }
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

// IR for ⟨expr⟩ ( '+' | '-' | '*' | '/' | '%' ) ⟨expr⟩
Value *ArithmeticOp_ExprAST::codegen()
{
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;
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
    return LogErrorV("invalid binary operator");
  }
}

/* ------------------------------------------- IR for CondAST ------------------------------------------- */

// IR for ⟨expr⟩( '==' | '!=' | '<' | '>' | '<=' | '>=' )⟨expr⟩
Value *ComparisonOp_CondAST::codegen()
{
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l || !r)
    return nullptr;
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
    return LogErrorV("invalid binary operator");
  }
}

// IR for ⟨cond⟩ ( '&' | '|' ) ⟨cond⟩
Value *LogicalOp_CondAST::codegen()
{
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l)
    return nullptr;
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
  // myfile << "⟨l-value⟩ '=' ⟨expr⟩: \t";
  Value *Val = RHS->codegen();
  if (!Val)
    return nullptr;

  ArrayElement_ExprAST *LHSA = dynamic_cast<ArrayElement_ExprAST *>(LHS);
  if (!LHSA)
  {
    /* assignment in identifier */
    Id_ExprAST *LHSE = dynamic_cast<Id_ExprAST *>(LHS);
    if (!LHSE)
      return LogErrorV("destination of '=' is not a variable");

    CodeGenBlock *block = context.getTop();
    AllocaInst *Variable;
    do
    {
      Variable = block->getLocals()[LHSE->getName()];
      if (!Variable)
      {
        LogErrorV("Unknown variable name in current block");
        printf("searching in block: %d\n", block->getId());
        block = context.getPrev(block);
      }
      else
      {
        if (PointerType::classof(Variable->getAllocatedType()))
        {
          Value *index = c16(0);
          Value *indexList[] = {ConstantInt::get(index->getType(), 0)};
          LoadInst *ldinst = Builder.CreateLoad(Variable, LHSE->getName());
          GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(ldinst, ArrayRef<Value *>(indexList), LHSE->getName(), context.currentBlock());
          Builder.CreateStore(Val, gepInst);
        }
        else
          Builder.CreateStore(Val, Variable);
        break;
      }
    } while (block != nullptr);
  }
  else
  {
    /* assignment in array element */
    CodeGenBlock *block = context.getTop();
    AllocaInst *arr;
    do
    {
      arr = block->getLocals()[LHSA->getName()];
      if (!arr)
      {
        LogErrorV("Unknown array name in current block");
        block = context.getPrev(block);
      }
      else
      {
        Value *t = LHSA->getExpr();
        Value *index = Builder.CreateSExt(t, i32, "ext");
        std::vector<Value *> indexList;

        if (PointerType::classof(arr->getAllocatedType()))
        {
          indexList.push_back(index);
          LoadInst *ldinst = Builder.CreateLoad(arr, LHSA->getName());
          GetElementPtrInst *gepInst = GetElementPtrInst::Create((arr->getAllocatedType())->getPointerElementType(), ldinst, ArrayRef<Value *>(indexList), LHSA->getName(), context.currentBlock());
          Builder.CreateStore(Val, gepInst);
        }
        else
        {
          indexList.push_back(c16(0));
          indexList.push_back(index);
          GetElementPtrInst *gepInst = GetElementPtrInst::Create(arr->getAllocatedType(), arr, ArrayRef<Value *>(indexList), LHSA->getName(), context.currentBlock());
          Builder.CreateStore(Val, gepInst);
        }
        break;
      }
    } while (block != NULL);
  }
  return Val;
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

        CodeGenBlock *block = context.getTop();
        AllocaInst *V;
        do
        {
          V = block->getLocals()[temp->getName()];
          if (!V)
            block = context.getPrev(block);
          else
          {
            if (ArrayType::classof(V->getAllocatedType()))
            {
              myfile << "\t\t--func call array" << std::endl;
              std::vector<Value *> indexList;
              indexList.push_back(c16(0));
              indexList.push_back(c16(0));
              GetElementPtrInst *gepInst = GetElementPtrInst::CreateInBounds(V->getAllocatedType(), V, ArrayRef<Value *>(indexList), temp->getName(), context.currentBlock());
              ArgsV.push_back(gepInst);
            }
            else
            {
              std::vector<Value *> indexList;
              indexList.push_back(c16(0));
              ArgsV.push_back(GetElementPtrInst::CreateInBounds(V->getAllocatedType(), V, ArrayRef<Value *>(indexList), temp->getName(), context.currentBlock()));
            }
            break;
          }
        } while (block != nullptr);
        if (!V)
          LogErrorV("Unknown variable name");
        return V;
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

// IR for “if” '(' ⟨cond⟩ ')' ⟨stmt⟩ [ “else” ⟨stmt⟩ ]
Value *If_StmtAST::codegen()
{
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  CondV = Builder.CreateICmpNE(CondV, c32(0), "ifcond");

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  context.pushBlock(ThenBB, nullptr);
  BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);

  // Builder.SetInsertPoint(ThenBB);
  Builder.SetInsertPoint(context.currentBlock());
  Value *ThenV = Then->codegen();

  if (!ThenV)
    return nullptr;

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateBr(MergeBB);
  ThenBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(ElseBB);
  context.pushBlock(ElseBB, nullptr);
  // Builder.SetInsertPoint(ElseBB);
  Builder.SetInsertPoint(context.currentBlock());

  Value *ElseV = nullptr;
  if (Else != nullptr)
  {
    ElseV = Else->codegen();
    if (!ElseV)
      return nullptr;
  }

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  context.pushBlock(MergeBB, nullptr);
  // Builder.SetInsertPoint(MergeBB);
  Builder.SetInsertPoint(context.currentBlock());
  return c32(0);
}

// IR for “while” '(' ⟨cond⟩ ')' ⟨stmt⟩
Value *While_StmtAST::codegen()
{
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;
  CondV = Builder.CreateICmpNE(CondV, c32(0), "while_entry");
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
  WhileV = Builder.CreateICmpNE(WhileV, c32(0), "while_entry");

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateCondBr(WhileV, WhileBB, MergeBB);
  WhileBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);

  return c32(0);
}

// IR for “return” [ ⟨expr⟩ ]
Value *Return_Stmt::codegen()
{
  if (expr == nullptr)
  {
    context.setCurrentReturnValue(nullptr);
    return c32(0);
  }
  Value *returnValue = expr->codegen();
  context.setCurrentReturnValue(returnValue);
  // std::cout << " return  " <<  context.id << std::endl;
  return returnValue;
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
    }
    // <func-def>
    else if (dynamic_cast<FunctionAST *>(VarNames[i]) != nullptr)
    {
      FunctionAST *temp = dynamic_cast<FunctionAST *>(VarNames[i]);
      Builder.SetInsertPoint(context.currentBlock());
      /* save var defs so far */
      for (std::map<std::string, AllocaInst *>::iterator it = (context.getTop()->getLocals()).begin(); it != (context.getTop()->getLocals()).end(); ++it)
      {
        context.inherited()[(temp->Proto->getName()).c_str()].push_back(std::pair<std::string, Type *>(it->first, PointerType::getUnqual(it->second->getAllocatedType())));
      }
      temp->codegen();
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
    context.locals() = prev->getLocals();

  // Record the function arguments in the NamedValues map.
  for (auto &Arg : TheFunction->args())
  {
    // Create an alloca for this variable
    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), Arg.getType());
    // Store the initial value into alloca
    Builder.CreateStore(&Arg, Alloca);
    // Add arguments to variable symbol table
    context.locals()[Arg.getName()] = Alloca;
  }

  if (Body->codegen())
  {
    myfile << "\n";
    for (int i = 0; i < context.id; i++)
      myfile << "\t";
    myfile << "Inherited: " << context.getTop()->getInherited().size() << std::endl;
    for (std::map<std::string, std::vector<std::pair<std::string, Type *>>>::iterator i = (context.inherited()).begin(); i != (context.inherited()).end(); ++i)
    {
      for (int i = 0; i < context.id; i++)
        myfile << "\t";
      myfile << i->first << std::endl;
      for (std::vector<std::pair<std::string, Type *>>::iterator it = i->second.begin(); it != i->second.end(); ++it)
      {
        for (int i = 0; i < context.id; i++)
          myfile << "\t";
        myfile << (it->first) << "   " << (it->second) << std::endl;
      }
    }
    myfile << "\n";
    for (int i = 0; i < context.id; i++)
      myfile << "\t";
    myfile << "Locals: " << context.getTop()->getLocals().size() << std::endl;

    for (std::map<std::string, AllocaInst *>::iterator it = (context.getTop()->getLocals()).begin(); it != (context.getTop()->getLocals()).end(); ++it)
    {
      for (int i = 0; i < context.id; i++)
        myfile << "\t";
      myfile << (it->first) << "   " << (it->second) << std::endl;
    }

    // Finish off the function.
    Builder.SetInsertPoint(context.currentBlock());
    ReturnInst::Create(TheContext, context.getCurrentReturnValue(), context.currentBlock());
    context.popBlock();

    // Validate the generated code, checking for consistency.
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
  Value *n16 = Builder.CreateZExt(n, i16, "ext");
  Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n16});
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  return n;
}

Value *WriteByte::codegen()
{
  Id_ExprAST *idexpr = dynamic_cast<Id_ExprAST *>(p);
  if (idexpr)
  {
    CodeGenBlock *block = context.getTop();
    AllocaInst *V;
    do
    {
      V = block->getLocals()[idexpr->getName()];
      if (!V)
        block = context.getPrev(block);
      else
      {
        if (PointerType::classof(V->getAllocatedType()))
        {
          // id in function -> using pointer
          Type *temp = (V->getAllocatedType());
          if (temp->getPointerElementType()->getIntegerBitWidth() == 8)
          {
            // myfile << "it is 8 bits" << std::endl;
            Value *n = p->codegen();
            Value *n16 = Builder.CreateZExtOrTrunc(n, i16, "ext");
            Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n16});
            Value *idxList[] = {c32(0), c32(0)};
            Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
          }
          else if (temp->getPointerElementType()->getIntegerBitWidth() == 16)
          {
            LogErrorV("Variable is wrong type(Integer instead of Byte)");
          }
        }
        else
        {
          Type *temp = (V->getAllocatedType());
          if (temp->getIntegerBitWidth() == 8)
          {
            Value *n = p->codegen();
            Value *n16 = Builder.CreateZExtOrTrunc(n, i16, "ext");
            Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n16});
            Value *idxList[] = {c32(0), c32(0)};
            Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
          }
          else if (temp->getIntegerBitWidth() == 16)
          {
            LogErrorV("Variable is wrong type(Integer instead of Byte)");
          }
        }
        break;
      }

    } while (block != nullptr);

    if (!V)
      LogErrorV("Unknown variable name");
  }
  else
  {
    Value *n = p->codegen();
    Value *n16 = Builder.CreateZExtOrTrunc(n, i16, "ext");
    Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n16});
    Value *idxList[] = {c32(0), c32(0)};
    Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
    Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  }

  return c8(0);
}

Value *WriteChar::codegen()
{
  Id_ExprAST *idexpr = dynamic_cast<Id_ExprAST *>(p);
  if (idexpr)
  {
    CodeGenBlock *block = context.getTop();
    AllocaInst *V;
    do
    {
      V = block->getLocals()[idexpr->getName()];
      if (!V)
        block = context.getPrev(block);
      else
      {
        if (PointerType::classof(V->getAllocatedType()))
        {
          // id in function -> using pointer
          Type *temp = (V->getAllocatedType());
          if (temp->getPointerElementType()->getIntegerBitWidth() == 8)
          {
            Value *n = p->codegen();
            Value *n8 = Builder.CreateZExtOrTrunc(n, i8, "ext");
            Builder.CreateCall(TheWriteByte, std::vector<Value *>{n8});
            Value *idxList[] = {c32(0), c32(0)};
            Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
          }
          else if (temp->getPointerElementType()->getIntegerBitWidth() == 16)
          {
            LogErrorV("Variable is wrong type(Integer instead of Byte)");
          }
        }
        else
        {
          Type *temp = (V->getAllocatedType());
          if (temp->getIntegerBitWidth() == 8)
          {
            Value *n = p->codegen();
            Value *n8 = Builder.CreateZExtOrTrunc(n, i8, "ext");
            Builder.CreateCall(TheWriteByte, std::vector<Value *>{n8});
            Value *idxList[] = {c32(0), c32(0)};
            Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
          }
          else if (temp->getIntegerBitWidth() == 16)
          {
            LogErrorV("Variable is wrong type(Integer instead of Byte)");
          }
        }
        break;
      }

    } while (block != nullptr);

    if (!V)
      LogErrorV("Unknown variable name");
  }
  else
  {
    Value *n = p->codegen();
    Value *n8 = Builder.CreateZExtOrTrunc(n, i8, "ext");
    Builder.CreateCall(TheWriteByte, std::vector<Value *>{n8});
    Value *idxList[] = {c32(0), c32(0)};
    Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
    Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  }
  return c8(0);
}

Value *WriteString::codegen()
{
  Id_ExprAST *idexpr = dynamic_cast<Id_ExprAST *>(str);
  if (idexpr)
  {
    CodeGenBlock *block = context.getTop();
    AllocaInst *V;
    do
    {
      V = block->getLocals()[idexpr->getName()];
      if (!V)
        block = context.getPrev(block);
      else
      {
        if (PointerType::classof(V->getAllocatedType()))
        {
          // id in function -> using pointer
          Type *temp = (V->getAllocatedType());
          if (temp->getPointerElementType()->getIntegerBitWidth() == 8)
          {
            // Value *n = p->codegen();
            // Value *n8 = Builder.CreateZExtOrTrunc(n, i8, "ext");
            // Builder.CreateCall(TheWriteByte, std::vector<Value *>{n8});
            // Value *idxList[] = {c32(0), c32(0)};
            // Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            // Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});

            Value *idxList[] = {c32(0), c32(0)};
            Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            Value *st = str->codegen();
            Builder.CreateCall(TheWriteString, std::vector<Value *>{st});
            Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
          }
          else if (temp->getPointerElementType()->getIntegerBitWidth() == 16)
          {
            LogErrorV("Variable is wrong type(Integer instead of Byte)");
          }
        }
        else
        {
          Type *temp = (V->getAllocatedType());
          if (temp->getIntegerBitWidth() == 8)
          {
            // Value *n = p->codegen();
            // Value *n8 = Builder.CreateZExtOrTrunc(n, i8, "ext");
            // Builder.CreateCall(TheWriteByte, std::vector<Value *>{n8});
            // Value *idxList[] = {c32(0), c32(0)};
            // Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            // Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});

            Value *idxList[] = {c32(0), c32(0)};
            Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
            Value *st = str->codegen();
            Builder.CreateCall(TheWriteString, std::vector<Value *>{st});
            Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
          }
          else if (temp->getIntegerBitWidth() == 16)
          {
            LogErrorV("Variable is wrong type(Integer instead of Byte)");
          }
        }
        break;
      }

    } while (block != nullptr);

    if (!V)
      LogErrorV("Unknown variable name");
  }
  else
  {
    Value *idxList[] = {c32(0), c32(0)};
    Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
    Value *st = str->codegen();
    Builder.CreateCall(TheWriteString, std::vector<Value *>{st});
    Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  }
  return c16(0);
}

Value *ReadInteger::codegen()
{
  Value *r = Builder.CreateCall(TheReadInteger, std::vector<Value *>());
  return Builder.CreateIntCast(r, i16, false, "ext");
}

Value *ReadByte::codegen()
{
  return Builder.CreateCall(TheReadInteger, std::vector<Value *>());
}

// Value *Shrink::codegen()
// {
//   // return Builder.CreateZExt(n, i8, "ext");
// }