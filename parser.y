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
  StringLiteral_ExprAST *string_literal;

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
  std::string *s;
}

%token TINT TBYTE TREFERENCE

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
%token TW_INT "writeInteger"
%token TW_BYTE "writeByte"
%token TW_CHAR "writeChar"
%token TW_STRING "writeString"
%token TR_INT "readInteger"
%token TR_BYTE "readByte"
%token TR_CHAR "readChar"
%token TR_STRING "readString"
%token T_EXNTEND "extend"
%token T_SHRINK "shrink"
%token T_STRLEN "strlen"
%token T_STRCMP "strcmp"
%token T_STRCPY "strcpy"
%token T_STRCAT "strcat"

%token<s> T_id
%token<n> T_INT_CONST
%token<c> T_CHAR_CONST
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
%type<fb> func_body
%type<block> compound_stmt
%type<block> stmt_list
%type<stmt> stmt

%type<expr> expr

%type<vfpar> fpar_list
%type<fpar> fpar_def
%type<vdef> local_def

%type<vardef> var_def
%type<t> var_type
%type<stmt> func_call
%type<vexpr> expr_list
%type<expr> l_value

%type<cond> cond
%type<t> data_type
%type<t> type
%type<t> r_type

%%


program:
  func_def                            { t = $$ = $1; }
;

func_def:
  T_id '(' fpar_list ')'':' r_type 
  func_body                           { $$ = new FunctionAST(new PrototypeAST($6, *$1, *$3), $7); } 
  | T_id '(' ')'':' r_type 
  func_body                           { $$ = new FunctionAST(new PrototypeAST($5, *$1, std::vector<std::pair<std::string, Type *>>()), $6); } 
;

func_body:
  local_def compound_stmt             { $$ = new FuncBody_AST(*$1, $2); }
;

fpar_def:
  T_id ':' type                       { $$ = new std::pair<std::string, Type*>(*$1, $3); }
| T_id ':' TREFERENCE type            { $$ = new std::pair<std::string, Type*>(*$1, PointerType::getUnqual($4)); }
;

fpar_list:
  fpar_def                            { $$ = new std::vector<std::pair<std::string, Type*>>(); $$->push_back(*$1); }
| fpar_list ',' fpar_def              { $1->push_back(*$3); }
;

type:
  data_type                           { $$ = $1; }
| data_type '[' ']'                   { $$ = PointerType::getUnqual($1); }
;

local_def:
  /* nothing */                       { $$ = new std::vector<LocalDef_AST *>(); }
| local_def var_def                   { $1->push_back($<ld>2); }
| local_def func_def                  { $1->push_back($<ld>2); }
;

var_def:
  T_id ':' var_type ';'               { $$ = new VarDef(std::pair<std::string, Type*>(*$1, $3));  }
;

var_type:
  data_type                           { $$ = $1; }
| data_type '[' T_INT_CONST ']'       { $$ = ArrayType::get($1, $3); } 
;

data_type:
  TINT                                { $$ = Type::getInt16Ty(getGlobalContext()); }
| TBYTE                               { $$ = Type::getInt8Ty(getGlobalContext()); }
;

r_type:
  data_type                           { $$ = $1; }
| "proc"                               { $$ = Type::getVoidTy(getGlobalContext()); }
;

compound_stmt:
  '{' stmt_list '}'                   { $$ = $2; }
;

stmt_list:
  /* nothing */                       { $$ = new CompoundStmt_StmtAST(); }
| stmt_list stmt                      { if ($<stmt>2 != NULL) $1->statements.push_back($<stmt>2); }
;

stmt:
  ';'                                 { $$ = NULL; }
| l_value '=' expr ';'                { $$ = new Assignment_StmtAST($1, $3); }
| compound_stmt                       { $$ = $1; }
| func_call ';'                       { $$ = $1; }
| "if" '(' cond ')' stmt              { $$ = new If_StmtAST($3, $5, NULL);  }
| "if" '(' cond ')' stmt "else" stmt  { $$ = new If_StmtAST($3, $5, $7);  }
| "while" '(' cond ')' stmt           { $$ = new While_StmtAST($3, $5); }
| "return" expr ';'                   { $$ = new Return_Stmt($2); }
| "return" ';'                        { $$ = new Return_Stmt(nullptr); } 
| "writeInteger" '(' expr ')' ';'     { $$ = new WriteInteger($3); }
| "writeByte" '(' expr ')' ';'        { $$ = new WriteByte($3); }
| "writeChar" '(' expr ')' ';'        { $$ = new WriteChar($3); }
| "readString" '(' expr ',' T_id ')'  { $$ = new ReadString($3, new Id_ExprAST(*$5)); }
| "writeString" '(' expr ')' ';'      { $$ = new WriteString($3); }
| "strcpy" '(' expr ',' expr ')'      { $$ = new Strcpy($3, $5);  }
| "strcat" '(' expr ',' expr ')'      { $$ = new Strcat($3, $5);  }
;

l_value:
  T_id                                { $$ = new Id_ExprAST(*$1); }
| T_id '[' expr ']'                   { $$ = new ArrayElement_ExprAST(*$1, $3); }
| T_STRING                            { $$ = new StringLiteral_ExprAST(*$1); }
;

func_call:
  T_id '(' expr_list ')'              { $$ = new FuncCall(*$1, *$3); }
;

expr_list:
  /* nothing */                       { $$ = new ExpressionList(); }
| expr                                { $$ = new ExpressionList(); $$->push_back($1); }
| expr_list ',' expr                  { $1->push_back($3); }
;

expr:
  T_INT_CONST                         { $$ = new IntConst_ExprAST($1); }
| T_CHAR_CONST                        { $$ = new CharConst_ExprAST($1); }
| l_value                             { $$ = $1; }
| '(' expr ')'                        { $$ = $2; }
| func_call                           { $$ = $<expr>1; }
| expr '+' expr                       { $$ = new ArithmeticOp_ExprAST($1, PLUS, $3); }
| expr '-' expr                       { $$ = new ArithmeticOp_ExprAST($1, MINUS, $3); }
| expr '*' expr                       { $$ = new ArithmeticOp_ExprAST($1, TIMES, $3); }
| expr '/' expr                       { $$ = new ArithmeticOp_ExprAST($1, DIV, $3); }
| expr '%' expr                       { $$ = new ArithmeticOp_ExprAST($1, MOD, $3); }
| '+' expr                            { $$ = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), PLUS, $2); }   %prec UPLUS
| '-' expr                            { $$ = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), MINUS, $2); }  %prec UMINUS
| "readInteger" '(' ')'               { $$ = new ReadInteger(); }
| "readByte" '(' ')'                  { $$ = new ReadByte();    }
| "readChar" '(' ')'                  { $$ = new ReadChar();    }
| "shrink" '(' expr ')'               { $$ = new Shrink($3);    }
| "extend" '(' expr ')'               { $$ = new Extend($3);    }
| "strcmp" '(' expr ',' expr ')'      { $$ = new Strcmp($3,$5); }
| "strlen" '(' expr ')'               { $$ = new Strlen($3); }
;

cond:
  "true"                              { $$ = new ComparisonOp_CondAST(new IntConst_ExprAST(0), L, new IntConst_ExprAST(1)); }
| "false"                             { $$ = new ComparisonOp_CondAST(new IntConst_ExprAST(1), L, new IntConst_ExprAST(0)); }
| '(' cond ')'                        { $$ = $2;                                                                    }
| '!' cond                            { $$ = new LogicalOp_CondAST($2, NOT, $2);                                    }
| expr '<' expr                       { $$ = new ComparisonOp_CondAST($1, L, $3);                                   }
| expr '>' expr                       { $$ = new ComparisonOp_CondAST($1, G, $3);                                   }
| expr "<=" expr                      { $$ = new ComparisonOp_CondAST($1, LE, $3);                                  }
| expr ">=" expr                      { $$ = new ComparisonOp_CondAST($1, GE, $3);                                  }
| expr "==" expr                      { $$ = new ComparisonOp_CondAST($1, EQ, $3);                                  }
| expr "!=" expr                      { $$ = new ComparisonOp_CondAST($1, NE, $3);                                  }
| cond '&' cond                       { $$ = new LogicalOp_CondAST($1, AND, $3);                                    }
| cond '|' cond                       { $$ = new LogicalOp_CondAST($1, OR, $3);                                     }
;

%%

void yyerror (const char *msg) {
  fprintf(stderr, "----------ALAN DEBUG MESSAGES----------\n\n");
  fprintf(stderr, "Alan error: %s\n", msg);
  fprintf(stderr, "Do you even know Alan? Line %d is so wrong \n\n\n\n",
          linenumber);
  exit(1);
}

int main() {
  if (yyparse()) return 1;
  llvm_compile_and_dump(t);
  return 0;
}