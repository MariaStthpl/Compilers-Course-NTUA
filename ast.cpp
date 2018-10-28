#include <stdio.h>
#include <stdlib.h>

#include "ast.hpp"

extern "C"
{
#include "symbol.h"
}

using namespace llvm;

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
// static Function *TheWriteByte;
static Function *TheWriteString;
static Function *TheReadInteger;
// static Function *TheReadByte;
static Function *TheReadString;

// Useful LLVM types.
static Type *i8 = IntegerType::get(TheContext, 8);
static Type *i16 = IntegerType::get(TheContext, 16);
static Type *i32 = IntegerType::get(TheContext, 32);
static Type *i64 = IntegerType::get(TheContext, 64);

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

void llvm_compile_and_dump(FunctionAST *t) {
  // Initialize the module and the optimization passes.
  TheModule = make_unique<Module>("minibasic program", TheContext);
  TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
  TheFPM->add(createPromoteMemoryToRegisterPass());
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
  // FunctionType *writeByte_type =
  //     FunctionType::get(Type::getVoidTy(TheContext),
  //                       std::vector<Type *>{i8}, false);
  // TheWriteByte =
  //     Function::Create(writeByte_type, Function::ExternalLinkage,
  //                      "writeInteger", TheModule.get());
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

std::unique_ptr<ExprAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

/* ---------------------------- ExprAST ---------------------- */

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, Type *t) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), 
                    TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(t, 0, VarName.c_str());
}

/* IR for ID */
Value *IdExprAST::codegen() {
  // Look this variable up in the function.
  AllocaInst *V = context.locals()[Name];
  if (!V)
    LogErrorV("Unknown variable name");
  if ( ArrayType::classof(V->getAllocatedType()) ) {
    AllocaInst* arr = context.locals()[Name];
    Value* index = c16(0);
    Value* indexList[2] = {ConstantInt::get(index->getType(), 0), index};
    return GetElementPtrInst::CreateInBounds(V->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), Name, context.currentBlock());
  }
  // if ( PointerType::classof(V->getAllocatedType()) ) {
  //   dyn_cast<ArrayType>(V->getAllocatedType());
  //   AllocaInst* arr = context.locals()[Name];
  //   Value* index = c16(0);
  //   Value* indexList[2] = {ConstantInt::get(index->getType(), 0), index};
  //   return Builder.CreateGEP(arr, ArrayRef<Value*>(indexList, 2), Name);
  //   // return GetElementPtrInst::CreateInBounds(V->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), Name, context.currentBlock());
  // }
  return Builder.CreateLoad(V, Name.c_str());
}

Value *ArrayElementExprAST::codegen() {
  AllocaInst* arr = context.locals()[Name];
  Value* index = expr->codegen();
  Value* indexList[2] = {ConstantInt::get(index->getType(), 0), index};
  if ( PointerType::classof(arr->getAllocatedType()) ) {
    // return Builder.CreateLoad(arr, Name.c_str());
    GetElementPtrInst* gepInst = GetElementPtrInst::CreateInBounds(arr->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), Name, context.currentBlock());
    return Builder.CreateLoad(gepInst, Name);
  }
  GetElementPtrInst* gepInst = GetElementPtrInst::CreateInBounds(arr->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), Name, context.currentBlock());
  // Value *gepInst = Builder.CreateGEP(arr, ArrayRef<Value*>(indexList, 2), Name);
  return Builder.CreateLoad(gepInst, Name);
  // Value* gepInst = Builder.CreateInBoundsGEP(arr, ArrayRef<Value*>(indexList, 2), Name);
  // return Builder.CreateLoad(gepInst, Name);


  // AllocaInst* arr = context.locals()[Name];
  // Value* index = expr->codegen();
  // Value* indexList[2] = {ConstantInt::get(index->getType(), 0), index};
  // return Builder.CreateExtractValue(arr,ArrayRef<unsigned int>(0));
  // GetElementPtrInst* gepInst = GetElementPtrInst::Create(arr->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), Name, context.currentBlock());
  // return Builder.CreateLoad(gepInst, Name);
}

/* IR for int */
Value *IntConst_ExprAST::codegen() {
  // std::cout << "Creating integer: " << Val << " in " <<  context.id << std::endl;
  return c16(Val);
}

/* IR for Char */
Value *CharConst_ExprAST::codegen() {
  // std::cout << "Creating char: " << Val << std::endl;
  return c8(Val);
}

/* IR for Binary Expressions */
Value *ArithmeticOp_ExprAST::codegen() {
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;
  switch (Op) {
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

Value *ComparisonOp_ExprAST::codegen() {
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l || !r)
    return nullptr;
  switch (Op) {
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

Value *LogicalOp_ExprAST::codegen() {
  Value *l = LHS->codegen();
  Value *r = RHS->codegen();
  if (!l)
    return nullptr;
  switch (Op) {
  case AND:
    if (!r) return nullptr;
    return Builder.CreateAnd(l, r, "andtmp");
  case OR:
    if (!r) return nullptr;
    return Builder.CreateOr(l, r, "ortmp");
  case NOT:
    return Builder.CreateNot(l, "nottmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}

Value *CallExprAST::codegen() {
  // Look up the name in the global module table.
  Function *CalleeF = TheModule->getFunction(Callee);
  if (!CalleeF)
    return LogErrorV("Unknown function referenced");

  // If argument mismatch error.
  if (CalleeF->arg_size() != Args.size())
    return LogErrorV("Incorrect # arguments passed");

  std::vector<Value *> ArgsV;
  FunctionType *FTy = CalleeF->getFunctionType();
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    // if ( FTy->getParamType(i)->isPointerTy() ) {
      // pass pointer
      // IdExprAST *temp = dynamic_cast<IdExprAST*>(Args[i]);
      // AllocaInst* arr = context.locals()[temp->getName()];
      // Value* index = c16(0);
      // Value* indexList[2] = {ConstantInt::get(index->getType(), 0), index};
      // GetElementPtrInst* gepInst = GetElementPtrInst::CreateInBounds(arr->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), temp->getName().c_str(), context.currentBlock());
      // ArgsV.push_back(gepInst);
    // } else
      ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }

  return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *PrototypeAST::codegen() {
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

/* IR for func-decl */
Function *FunctionAST::codegen() {
   // First, check for an existing function from a previous 'extern' declaration.
  Function *TheFunction = TheModule->getFunction(Proto->getName());

  if (!TheFunction)
    TheFunction = Proto->codegen();

  if (!TheFunction)
    return nullptr;

  if (!TheFunction->empty())
    return (Function*)LogErrorV("Function cannot be redefined.");

  std::map<std::string, AllocaInst*> temp;
  if ( context.id != 0 ) { 
     temp = context.locals();
  }
  
  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
  context.pushBlock(BB);
  Builder.SetInsertPoint(context.currentBlock());

  if ( context.id > 1 ) {
    context.locals() = temp;
    for (std::map<std::string, AllocaInst*>::const_iterator it = (context.locals()).begin(); it != (context.locals()).end(); ++it ) {
        // Create an alloca for this variable
        AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, it->first, it->second->getAllocatedType());
        context.locals()[it->first] = Alloca;
      }
  }

  // Record the function arguments in the NamedValues map.
  for (auto &Arg : TheFunction->args()) {
    // Create an alloca for this variable
    AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, Arg.getName(), Arg.getType());
    // Store the initial value into alloca
    Builder.CreateStore(&Arg, Alloca);
    // Add arguments to variable symbol table
    context.locals()[Arg.getName()] = Alloca;
  }
  
  if (Value *RetVal = Body->codegen()) {
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

Value *If_ExprAST::codegen() {
  Value *CondV = Cond->codegen();
  if (!CondV)
    return nullptr;

  CondV = Builder.CreateICmpNE(CondV, c32(0), "ifcond");

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  context.pushBlock(ThenBB);
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
  context.pushBlock(ElseBB);
  // Builder.SetInsertPoint(ElseBB);
  Builder.SetInsertPoint(context.currentBlock());
  
  Value *ElseV = nullptr;
  if ( Else != nullptr ) {
    ElseV = Else->codegen();
    if (!ElseV)
      return nullptr;
  }

  if (!Builder.GetInsertBlock()->getTerminator())
    Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  context.pushBlock(MergeBB);
  // Builder.SetInsertPoint(MergeBB);
  Builder.SetInsertPoint(context.currentBlock());
  return c32(0);
}

Value *While_ExprAST::codegen() {
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

Value *Assignment_StmtAST::codegen() {
  Value *Val = RHS->codegen();
  if (!Val)
    return nullptr;

  ArrayElementExprAST *LHSA = dynamic_cast<ArrayElementExprAST *>(LHS);  
  if (!LHSA) {
    IdExprAST *LHSE = dynamic_cast<IdExprAST*>(LHS);
    if (!LHSE)
      return LogErrorV("destination of '=' is not a variable");
    Value *Variable = context.locals()[LHSE->getName()];
    if (!Variable)
      return LogErrorV("Unknown variable name");
    Builder.CreateStore(Val, Variable);
  } else {
    AllocaInst* arr = context.locals()[LHSA->getName()];
    Value* index = LHSA->getExpr();
    Value* indexList[2] = {ConstantInt::get(index->getType(), 0), index};
    GetElementPtrInst* gepInst = GetElementPtrInst::Create(arr->getAllocatedType(), arr, ArrayRef<Value*>(indexList, 2), LHSA->getName(), context.currentBlock());
    Builder.CreateStore(Val, gepInst);
  }
  return Val;
}

Value *WriteInteger::codegen() {
  Value *n = p->codegen();
  // Value *n64 = Builder.CreateZExt(n, i64, "ext");
  Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n});
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  return n;
}

Value *WriteByte::codegen() {
  Value *n = p->codegen();
  Value *n16 = Builder.CreateZExt(n, i16, "ext");
  Builder.CreateCall(TheWriteInteger, std::vector<Value *>{n16});
  Value *idxList[] = {c32(0), c32(0)};
  Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
  Builder.CreateCall(TheWriteString, std::vector<Value *>{nl});
  return n;
}

Value *ReadInteger::codegen() {
  Value* r = Builder.CreateCall(TheReadInteger, std::vector<Value *>());
  Builder.CreateIntCast(r, i16, false, "ext");
}

Value *ReadByte::codegen() {
  return Builder.CreateCall(TheReadInteger, std::vector<Value *>());
}

Value *Shrink::codegen() {
  // return Builder.CreateZExt(n, i8, "ext");
}

/* ------------------- StmtAST --------------------- */

Value *Block::codegen() {
  StatementList::const_iterator it;
  Value *last = NULL;
  for ( it = statements.begin(); it != statements.end(); it++ ) {
    // std::cout << "Generating code for " << typeid(**it).name() << std::endl;
    last = (**it).codegen();
  }
  // std::cout << "Creating block" << std::endl;
  return last;
}

Value *Statement::codegen() {
  // std::cout << "Generating code for " << typeid(expr).name() << std::endl;
  return expr.codegen();
}

Value *Return::codegen() {
  if ( expr == nullptr ) {
    context.setCurrentReturnValue(nullptr);
    return c32(0);
  }
  Value *returnValue = expr->codegen();
  context.setCurrentReturnValue(returnValue);
  // std::cout << " return  " <<  context.id << std::endl;
  return returnValue;
}

Function *VarDef::codegen() {
  return nullptr;
}

Value *FuncBody_AST::codegen() {

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  // Register all variables and emit their initializer
  for ( unsigned i = 0, e = VarNames.size(); i != e; ++i ) {
    if (dynamic_cast<VarDef *>(VarNames[i]) != nullptr ) {
      VarDef *temp = dynamic_cast<VarDef *>(VarNames[i]);
      
      const std::string &VarName = temp->vdef.first;
      Type *t = temp->vdef.second;

      AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction, VarName, t);

      context.locals()[VarName] = Alloca;
    }
    else if ( dynamic_cast<FunctionAST *>(VarNames[i]) != nullptr ) {
      FunctionAST *temp = dynamic_cast<FunctionAST *>(VarNames[i]);
      Builder.SetInsertPoint(context.currentBlock());
      temp->codegen();
    }
  }
  
  // std::cout << "  func body start  " <<  context.id << std::endl;
  
  Builder.SetInsertPoint(context.currentBlock());
  Value *BodyVal = Body->codegen();
  
  // std::cout << "  func body end  " <<  context.id << std::endl;
  if (!BodyVal)
    return nullptr;

  return BodyVal;
}