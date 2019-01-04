/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TINT = 258,
    TBYTE = 259,
    TREFERENCE = 260,
    TIF = 261,
    TELSE = 262,
    TWHILE = 263,
    TRETURN = 264,
    TPROC = 265,
    T_print = 266,
    TTRUE = 267,
    TFALSE = 268,
    EQ_OP = 269,
    NEQ_OP = 270,
    LE_OP = 271,
    GE_OP = 272,
    TW_INT = 273,
    TW_BYTE = 274,
    TW_CHAR = 275,
    TW_STRING = 276,
    TR_INT = 277,
    TR_BYTE = 278,
    TR_CHAR = 279,
    TR_STRING = 280,
    T_EXNTEND = 281,
    T_SHRINK = 282,
    T_STRLEN = 283,
    T_STRCMP = 284,
    T_STRCPY = 285,
    T_STRCAT = 286,
    T_id = 287,
    T_INT_CONST = 288,
    T_CHAR_CONST = 289,
    T_STRING = 290,
    UPLUS = 291,
    UMINUS = 292,
    IF_PART = 293,
    NO_ELSE = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "parser.y" /* yacc.c:1909  */

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

#line 121 "parser.hpp" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
