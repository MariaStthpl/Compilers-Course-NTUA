%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.hpp"
#include "symbol.h"

extern int yylex();
void yyerror (const char *msg);

extern int linenumber;
FunctionAST *t;
%}

%expect 1 // 1 - if else, 8 - stmt_list
%union{
  ExprAST *e;
  std::vector<ExprAST*> *vec;
  FunctionAST *f;

  Block *block;
  StmtAST *stmt;
  // std::vector<StmtAST*> stmtvec;

  char c;
  // char *s;
  std::string *s;
  std::vector<std::string> *svec;
  int n;
  Stype t;
  // struct vars {
  //   char *s;
  //   int offset;
  // } variable;
}

%token TINT TBYTE TIF TELSE TTRUE TFALSE TPROC TREFERENCE TRETURN TWHILE
%token TW_INT TW_BYTE TW_CHAR TW_STRING TR_INT TR_BYTE TR_CHAR TR_STRING T_EXNTEND T_SHRINK T_STRLEN T_STRCMP T_STRCPY T_STRCAT
%token LE_OP GE_OP EQ_OP NEQ_OP

%token T_print "print"
%token T_do "do"
%token T_begin "begin"
%token T_end "end"
%token T_var "var"
%token T_bool "bool"
%token<c> T_id
%token<n> T_const
%token<s> T_NAME
%token<n> T_CHAR
%token<s> T_STRING

%left '|'
%left '&'
%left '!'
%nonassoc '<' '>' LE_OP GE_OP EQ_OP NEQ_OP
%left '+' '-'
%left '*' '/' '%'
%left UPLUS UMINUS
%nonassoc IF_PART NO_ELSE

%type<f> program
%type<f> func_def
%type<block> func_body
%type<block> compound_stmt
%type<block> stmt_list
%type<stmt> stmt
%type<e> expr
%type<svec> fpar_list
%type<s> fpar_def
%type<f> local_def
// %type<a> var_def
// %type<t> var_type
%type<stmt> func_call
%type<vec> expr_list
// %type<variable> l_value
// %type<a> l_value

// %type<a> ret_value
%type<e> cond
%type<t> data_type
// %type<t> type
%type<t> r_type

%%
program:
  func_def                            { t = $$ = $1; }
;

// program:
//   stmt_list                           { t = $$ = $1; }
// ;

func_def:
  T_NAME '(' fpar_list ')'':' r_type func_body    { $$ = new FunctionAST(new PrototypeAST(*$1, *$3), $7); } 
;

func_body:
  local_def compound_stmt             { $$ = new SeqExprAST($1, $2); } //{ $$ = new Block(); if ($<stmt>1 != NULL) $$->Block::statements.push_back($<stmt>1); if ($<stmt>2 != NULL) $$->Block::statements.push_back($<stmt>2); }//{ $$ = new SeqExprAST($1, $2); }
;

fpar_def:
//   T_NAME ':' type                     { $$ = $1; }
// | T_NAME ':' TREFERENCE type          { $$ = $1; }
  T_NAME                              { $$ = $1; }
;

fpar_list:
  /* nothing */                       { $$ = new std::vector<std::string>(); }
| fpar_def                            { $$ = new std::vector<std::string>(); $$->push_back(*$1); }
| fpar_list ',' fpar_def              { $$ = nullptr; }
;

local_def:
  /* nothing */                       { $$ = NULL; }
| func_def                            { $$ = $1; } 
// | var_def ';' local_def               {  }
;

// var_def:
//   T_NAME ':' var_type                 {  }
// ;

// var_type:
//   data_type                           {  }
// | data_type '[' T_const ']'           {  } 
// ;

data_type:
  TINT                                { $$ = typeInteger; }
| TBYTE                               { $$ = typeInteger; }
;

r_type:
  data_type                           { $$ = $1; }
| TPROC                               { $$ = typeVoid; }
;

compound_stmt:
  '{' stmt_list '}'                   { $$ = $2; }
;

stmt_list:
  stmt                                { $$ = new Block(); if ($<stmt>1 != NULL) $$->Block::statements.push_back($<stmt>1); }
| stmt_list stmt                      { if ($<stmt>2 != NULL) $1->statements.push_back($<stmt>2); }
;

stmt:
  ';'                                 { $$ = NULL; }
// | l_value '=' expr ';'                { $$ = ast_let($1, $3); }         // Anathesi
| compound_stmt                       { $$ = $1; }
| func_call ';'                       { $$ = $1; }
| TIF '(' cond ')' stmt               { $$ = new If_ExprAST($3, $5, NULL);  }
| TIF '(' cond ')' stmt TELSE stmt    { $$ = new If_ExprAST($3, $5, $7);  }
| TWHILE '(' cond ')' stmt            { $$ = new While_ExprAST($3, $5); }       // while loop
// | TRETURN ret_value ';'               {  }      //???????
| "print" expr ';'                    { $$ = new PRINTAST($2); }
;

// l_value:
//   T_NAME                              {  }
// | T_NAME '[' expr ']'                 {  }
// // | T_STRING                            { $$ = $1; }
// ;

// ret_value:
//   /* nothing */                       {  }
// | expr                                {  }
// ;

func_call:
  T_NAME '(' expr_list ')'            { $$ = new CallExprAST(*$1, *$3); }
;

expr_list :
  /* nothing */                       { $$ = new ExpressionList(); }
| expr                                { $$ = new ExpressionList(); $$->push_back($1); }
| expr_list ',' expr                  { $1->push_back($3); }
;

expr:
  T_const                             { $$ = new ConstExprAST($1); }
// | T_CHAR                              {  }
// | l_value                             {  }
| '(' expr ')'                        { $$ = $2; }
| expr '+' expr                       { $$ = new ArithmeticOp_ExprAST($1, PLUS, $3); }
| expr '-' expr                       { $$ = new ArithmeticOp_ExprAST($1, MINUS, $3); }
| expr '*' expr                       { $$ = new ArithmeticOp_ExprAST($1, TIMES, $3); }
| expr '/' expr                       { $$ = new ArithmeticOp_ExprAST($1, DIV, $3); }
| expr '%' expr                       { $$ = new ArithmeticOp_ExprAST($1, MOD, $3); }
| '+' expr                            { $$ = new ArithmeticOp_ExprAST(new ConstExprAST(0), PLUS, $2); }   %prec UPLUS
| '-' expr                            { $$ = new ArithmeticOp_ExprAST(new ConstExprAST(0), MINUS, $2); }  %prec UMINUS
;

cond:
  TTRUE                               { $$ = new ComparisonOp_ExprAST(new ConstExprAST(0), L, new ConstExprAST(1)); }
| TFALSE                              { $$ = new ComparisonOp_ExprAST(new ConstExprAST(1), L, new ConstExprAST(0)); }
| '(' cond ')'                        { $$ = $2;                                                                    }
| '!' cond                            { $$ = new LogicalOp_ExprAST($2, NOT, $2);                                    }
| expr '<' expr                       { $$ = new ComparisonOp_ExprAST($1, L, $3);                                   }
| expr '>' expr                       { $$ = new ComparisonOp_ExprAST($1, G, $3);                                   }
| expr LE_OP expr                     { $$ = new ComparisonOp_ExprAST($1, LE, $3);                                  }
| expr GE_OP expr                     { $$ = new ComparisonOp_ExprAST($1, GE, $3);                                  }
| expr EQ_OP expr                     { $$ = new ComparisonOp_ExprAST($1, EQ, $3);                                  }
| expr NEQ_OP expr                    { $$ = new ComparisonOp_ExprAST($1, NE, $3);                                  }
| cond '&' cond                       { $$ = new LogicalOp_ExprAST($1, AND, $3);                                    }
| cond '|' cond                       { $$ = new LogicalOp_ExprAST($1, OR, $3);                                     }
;

// type:
//   data_type                           {  }
// | data_type '['']'                    {  }
// ;

%%

void yyerror (const char *msg) {
  fprintf(stderr, "Minibasic error: %s\n", msg);
  fprintf(stderr, "Aborting, I've had enough with line %d...\n",
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