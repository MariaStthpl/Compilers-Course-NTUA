%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.hpp"

extern int yylex();
void yyerror (const char *msg);

extern int linenumber;
FunctionAST *t;
%}


%expect 1 // if else 
// %expect 10 // 1 - if else, 1 - var_def, 6 - stmt_list, 1 - return, 1 - WriteInteger
%union{
  Id_ExprAST *id;

  ExprAST *expr;
  CondAST *cond;

  std::vector<ExprAST*> *vexpr;
  
  FunctionAST *f;
  FuncBody_AST *fb;
  
  Type *t;
  LocalDef_AST *ld;
  VarDef *vardef;
  std::vector<LocalDef_AST *> *vdef;
  std::pair<std::string, Type *> *fpar;
  std::vector<std::pair<std::string, Type *>> *vfpar;

  CompoundStmt_StmtAST *block;
  StmtAST *stmt;

  int n;
  char c;
  std::string s;
}

%token TINT TBYTE TREFERENCE
%token TW_INT TW_BYTE TW_CHAR TW_STRING TR_INT TR_BYTE TR_CHAR TR_STRING T_EXNTEND T_SHRINK T_STRLEN T_STRCMP T_STRCPY T_STRCAT

%token TIF "if"
%token TELSE "else"
%token TWHILE "while"
%token TRETURN "return"
%token TPROC "proc"
%token T_print "print"
%token TTRUE "true"
%token TFALSE "false"
%token EQ_OP "=="
%token NEQ_OP "!="
%token LE_OP "<="
%token GE_OP ">="
%token<s> T_id
%token<n> T_INT_CONST
%token<n> T_CHAR_CONST
%token<s> T_STRING

%left '|'
%left '&'
%nonassoc '<' '>' LE_OP GE_OP EQ_OP NEQ_OP
%left '+' '-'
%left '*' '/' '%'
%left UPLUS UMINUS '!' // not sure about prefix
%nonassoc IF_PART NO_ELSE

%type<f> program
%type<f> func_def
// %type<fb> func_body
%type<block> compound_stmt
// %type<block> stmt_list
%type<stmt> stmt
%type<stmt> stmt_help

%type<expr> expr
%type<expr> expr_list_help

%type<vfpar> fpar_list
%type<fpar> fpar_def
%type<fpar> fpar_def_help
%type<vdef> local_def
%type<vdef> local_def_help

%type<vardef> var_def
// %type<t> var_type
%type<stmt> func_call
%type<vexpr> expr_list
%type<expr> l_value

// %type<a> ret_value
%type<cond> cond
%type<t> data_type
%type<t> type
%type<t> r_type

%%



//Allaksa tous kanones wste na mhn exei shift reduce conflicts.
//Xrhsimopoihshsa se kathe kanona tous kwdikes pou eixe hdh gia tis times apotimhshs twn ekfrasewn me kapoies parallages opou xreiazotan
//Den kserw an einai ola swsta gia tis times autes
//Epishs exw afhsei ena lathos sto Function Prototype. Tou pernaw san deytero orisma null giati den kserw ti xreiazetai pleon.
//Den kserw epishs an xalaei genika kati allo twra. O parser pantws kanei compile.


program:
  func_def                            { t = $$ = $1; }
;

func_def:
  T_id '(' ')' ':' r_type
  local_def_help compound_stmt               { $$ = new FunctionAST(new PrototypeAST($5, $1, NULL), NULL); } //Needs FIXING!! the second argument

  | T_id '(' fpar_list ')' ':' r_type
  local_def_help compound_stmt               { $$ = new FunctionAST(new PrototypeAST($6, $1, NULL), NULL); } //Needs FIXING!! the second argument
;

local_def_help:
  /* nothing */                           { $$ = new std::vector<LocalDef_AST *>(); }
  | local_def_help local_def              {$$->push_back($2); }
;

fpar_list:
  fpar_def fpar_def_help          { $$ = new std::vector<std::pair<std::string, Type*>>(); $$->push_back(*$1); $$->std::move($2.begin(), $2.end(), std::back_inserter($$)); }
;

fpar_def_help:
  /*nothing*/                             { $$ = new std::vector<std::pair<std::string, Type*>>(); }
  | ',' fpar_def fpar_def_help            { $$->push_back($2); }
;

fpar_def:
  T_id ':' type                       { $$ = new std::pair<std::string, Type*>($1, $3); }
  | T_id ':' TREFERENCE type          { $$ = $1; }
;

data_type:
  TINT                         { $$ = Type::getInt16Ty(getGlobalContext()); }
  | TBYTE                      { $$ = Type::getInt8Ty(getGlobalContext()); }
;

type:
  data_type                     { $$ = $1; }
  | data_type '[' ']'           { $$ = PointerType::getUnqual($1); }
;

r_type:
  data_type        { $$ = $1; }
  | "proc"         { $$ = Type::getVoidTy(getGlobalContext()); }
;


local_def: 
  func_def              { $$ = new std::vector<LocalDef_AST *>(); $$->push_back($<ld>1); }
  | var_def             { $$ = new std::vector<LocalDef_AST *>(); $$->push_back($<ld>1); }
;

var_def:
  T_id ":" data_type                               { $$ = new VarDef(std::pair<std::string, Type*>($1, $3));  }
  | T_id ":" data_type '[' T_INT_CONST ']'         { $$ = new VarDef(std::pair<std::string, Type*>($1, ArrayType::get($3, $5)));  }
;

stmt:
  ';'                                     { $$ = NULL; }
  | l_value '=' expr ';'                  { $$ = new Assignment_StmtAST($1, $3); }
  | compound_stmt                         { $$ = $1; }
  | func_call ';'                         { $$ = $1; }
  | "if" '(' cond ')' stmt                { $$ = new If_StmtAST($3, $5, NULL);  }
  | "if" '(' cond ')' stmt "else" stmt    { $$ = new If_StmtAST($3, $5, $7);  }
  | "while" '(' cond ')' stmt             { $$ = new While_StmtAST($3, $5); }
  | "return" ';'                          { $$ = new Return_Stmt(nullptr); } 
  | "return" expr ';'                     { $$ = new Return_Stmt($2); }
;


compound_stmt:
  '{' stmt_help '}'       { $$ = $2; }
;


stmt_help:
  /* nothing */           { $$ = new CompoundStmt_StmtAST(); }
  | stmt stmt_help        { if ($<stmt>2 != NULL) $1->statements.push_back($<stmt>2); }
;

func_call:
  T_id '(' ')'                { $$ = new FuncCall($1, NULL); }
  | T_id '(' expr_list ')'    { $$ = new FuncCall($1, *$3); }
;


expr_list:
  expr expr_list_help         { $$ = new ExpressionList(); $$->push_back($1); $$->std::move($2.begin(), $2.end(), std::back_inserter($$));  }
;

expr_list_help:
  /* nothing */                         { $$ = new ExpressionList(); }
  | ',' expr expr_list_help             { $$ = $$->push_back($2); }
;

expr:
  T_INT_CONST                { $$ = new IntConst_ExprAST($1); }
  | T_CHAR_CONST             { $$ = new CharConst_ExprAST($1); }
  | l_value                  { $$ = $1; }
  | '(' expr ')'             { $$ = $2; }
  | func_call                { $$ = $<expr>1; }
  | '+' expr                 { $$ = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), PLUS, $2); }   %prec UPLUS
  | '-' expr                 { $$ = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), MINUS, $2); }  %prec UMINUS
  | expr '+' expr            { $$ = new ArithmeticOp_ExprAST($1, PLUS, $3); }
  | expr '-' expr            { $$ = new ArithmeticOp_ExprAST($1, MINUS, $3); }
  | expr '*' expr            { $$ = new ArithmeticOp_ExprAST($1, TIMES, $3); }
  | expr '/' expr            { $$ = new ArithmeticOp_ExprAST($1, DIV, $3); }
  | expr '%' expr            { $$ = new ArithmeticOp_ExprAST($1, MOD, $3); }
;

l_value: 
  T_id                      { $$ = new Id_ExprAST($1); }
  | T_id '[' expr ']'       { $$ = new ArrayElement_ExprAST($1, $3); }
  | T_STRING                { $$ = $1; }
;

cond:
  "true"                    { $$ = new ComparisonOp_CondAST(new IntConst_ExprAST(0), L, new IntConst_ExprAST(1)); } 
  | "false"                 { $$ = new ComparisonOp_CondAST(new IntConst_ExprAST(1), L, new IntConst_ExprAST(0)); }
  | '(' cond ')'            { $$ = $2;                                                                   }
  | '!' cond                { $$ = new LogicalOp_CondAST($2, NOT, $2);                                   }
  | expr "==" expr          { $$ = new ComparisonOp_CondAST($1, EQ, $3);                                 }
  | expr "!=" expr          { $$ = new ComparisonOp_CondAST($1, NE, $3);                                 }
  | expr '<' expr           { $$ = new ComparisonOp_CondAST($1, L, $3);                                  }
  | expr '>' expr           { $$ = new ComparisonOp_CondAST($1, G, $3);                                  }
  | expr "<=" expr          { $$ = new ComparisonOp_CondAST($1, LE, $3);                                 }
  | expr ">=" expr          { $$ = new ComparisonOp_CondAST($1, GE, $3);                                 }
  | cond '&' cond           { $$ = new LogicalOp_CondAST($1, AND, $3);                                   }
  | cond '|' cond           { $$ = new LogicalOp_CondAST($1, OR, $3);                                    }
;

/*------------------------------------------------------------------------------------------------------
-
-
-
-
OLD//////////////////////////////////////////////////////////////////////////////////////////////////////
-
-
-
-
-------------------------------------------------------------------------------------------------------
*/

// program:
//   func_def                            { t = $$ = $1; }
// ;

// func_def:
//   T_id '(' fpar_list ')'':' r_type 
//   func_body                           { $$ = new FunctionAST(new PrototypeAST($6, *$1, *$3), $7); } 
// ;

// func_body:
//   local_def compound_stmt             { $$ = new FuncBody_AST(*$1, $2); }
// ;

// fpar_def:
//   T_id ':' type                       { $$ = new std::pair<std::string, Type*>(*$1, $3); }
// | T_id ':' TREFERENCE type            { $$ = new std::pair<std::string, Type*>(*$1, PointerType::getUnqual($4)); }
// ;

// fpar_list:
//   /* nothing */                       { $$ = new std::vector<std::pair<std::string, Type*>>(); }
// | fpar_def                            { $$ = new std::vector<std::pair<std::string, Type*>>(); $$->push_back(*$1); }
// | fpar_list ',' fpar_def              { $1->push_back(*$3); }
// ;

// type:
//   data_type                           { $$ = $1; }
// | data_type '[' ']'                   { $$ = PointerType::getUnqual($1); }
// ;

// local_def:
//   /* nothing */                       { $$ = new std::vector<LocalDef_AST *>(); }
// | local_def var_def                   { $1->push_back($<ld>2); }
// | local_def func_def                  { $1->push_back($<ld>2); }
// ;

// var_def:
//   T_id ':' var_type ';'               { $$ = new VarDef(std::pair<std::string, Type*>(*$1, $3));  }
// ;

// var_type:
//   data_type                           { $$ = $1; }
// | data_type '[' T_INT_CONST ']'       { $$ = ArrayType::get($1, $3); } 
// ;

// data_type:
//   TINT                                { $$ = Type::getInt16Ty(getGlobalContext()); }
// | TBYTE                               { $$ = Type::getInt8Ty(getGlobalContext()); }
// ;

// r_type:
//   data_type                           { $$ = $1; }
// | TPROC                               { $$ = Type::getVoidTy(getGlobalContext()); }
// ;

// compound_stmt:
//   '{' stmt_list '}'                   { $$ = $2; }
// ;

// stmt_list:
//   /* nothing */                       { $$ = new CompoundStmt_StmtAST(); }
// | stmt_list stmt                      { if ($<stmt>2 != NULL) $1->statements.push_back($<stmt>2); }
// ;

// stmt:
//   ';'                                 { $$ = NULL; }
// | l_value '=' expr ';'                { $$ = new Assignment_StmtAST($1, $3); }
// | compound_stmt                       { $$ = $1; }
// | func_call ';'                       { $$ = $1; }
// | TIF '(' cond ')' stmt               { $$ = new If_StmtAST($3, $5, NULL);  }
// | TIF '(' cond ')' stmt TELSE stmt    { $$ = new If_StmtAST($3, $5, $7);  }
// | TWHILE '(' cond ')' stmt            { $$ = new While_StmtAST($3, $5); }
// | TRETURN expr ';'                    { $$ = new Return_Stmt($2); }
// | TRETURN ';'                         { $$ = new Return_Stmt(nullptr); } 
// | TW_INT '(' expr ')' ';'             { $$ = new WriteInteger($3); }
// | TW_BYTE '(' expr ')' ';'            { $$ = new WriteByte($3); }
// ;

// l_value:
//   T_id                              { $$ = new Id_ExprAST(*$1); }
// | T_id '[' expr ']'                 { $$ = new ArrayElement_ExprAST(*$1, $3); }
// // // | T_STRING                            { $$ = $1; }
// ;

// // ret_value:
// //   /* nothing */                       {  }
// // | expr                                {  }
// // ;

// func_call:
//   T_id '(' expr_list ')'            { $$ = new FuncCall(*$1, *$3); }
// ;

// expr_list :
//   /* nothing */                       { $$ = new ExpressionList(); }
// | expr                                { $$ = new ExpressionList(); $$->push_back($1); }
// | expr_list ',' expr                  { $1->push_back($3); }
// ;

// expr:
//   T_INT_CONST                         { $$ = new IntConst_ExprAST($1); }
// | T_CHAR_CONST                        { $$ = new CharConst_ExprAST($1); }
// | l_value                             { $$ = $1; }
// | '(' expr ')'                        { $$ = $2; }
// | func_call                           { $$ = $<expr>1; }
// | expr '+' expr                       { $$ = new ArithmeticOp_ExprAST($1, PLUS, $3); }
// | expr '-' expr                       { $$ = new ArithmeticOp_ExprAST($1, MINUS, $3); }
// | expr '*' expr                       { $$ = new ArithmeticOp_ExprAST($1, TIMES, $3); }
// | expr '/' expr                       { $$ = new ArithmeticOp_ExprAST($1, DIV, $3); }
// | expr '%' expr                       { $$ = new ArithmeticOp_ExprAST($1, MOD, $3); }
// | '+' expr                            { $$ = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), PLUS, $2); }   %prec UPLUS
// | '-' expr                            { $$ = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), MINUS, $2); }  %prec UMINUS
// | TR_INT '(' ')'                      { $$ = new ReadInteger(); }
// | TR_BYTE '(' ')'                     { $$ = new ReadByte(); }
// ;

// cond:
//   TTRUE                               { $$ = new ComparisonOp_CondAST(new IntConst_ExprAST(0), L, new IntConst_ExprAST(1)); }
// | TFALSE                              { $$ = new ComparisonOp_CondAST(new IntConst_ExprAST(1), L, new IntConst_ExprAST(0)); }
// | '(' cond ')'                        { $$ = $2;                                                                    }
// | '!' cond                            { $$ = new LogicalOp_CondAST($2, NOT, $2);                                    }
// | expr '<' expr                       { $$ = new ComparisonOp_CondAST($1, L, $3);                                   }
// | expr '>' expr                       { $$ = new ComparisonOp_CondAST($1, G, $3);                                   }
// | expr LE_OP expr                     { $$ = new ComparisonOp_CondAST($1, LE, $3);                                  }
// | expr GE_OP expr                     { $$ = new ComparisonOp_CondAST($1, GE, $3);                                  }
// | expr EQ_OP expr                     { $$ = new ComparisonOp_CondAST($1, EQ, $3);                                  }
// | expr NEQ_OP expr                    { $$ = new ComparisonOp_CondAST($1, NE, $3);                                  }
// | cond '&' cond                       { $$ = new LogicalOp_CondAST($1, AND, $3);                                    }
// | cond '|' cond                       { $$ = new LogicalOp_CondAST($1, OR, $3);                                     }
// ;


%%

void yyerror (const char *msg) {
  fprintf(stderr, "Alan error: %s\n", msg);
  fprintf(stderr, "Do you even know Alan? Line %d is so wrong \n",
          linenumber);
  exit(1);
}

int main() {
  if (yyparse()) return 1;
  llvm_compile_and_dump(t);
  return 0;
}

// int main() {
//   if (yyparse()) return 1;
//   printf("Compilation was successful.\n");
//   initSymbolTable(997);
//   ast_sem(t);
//   ast_run(t);
//   destroySymbolTable();
//   return 0;
// }