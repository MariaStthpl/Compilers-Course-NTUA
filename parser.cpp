/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.hpp"

extern int yylex();
void yyerror (const char *msg);

extern int linenumber;
FunctionAST *t;

#line 79 "parser.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.hpp".  */
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
#line 17 "parser.y" /* yacc.c:355  */

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

#line 186 "parser.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 203 "parser.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   377

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  78
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,     2,     2,     2,    44,    37,     2,
      50,    51,    42,    40,    53,    41,     2,    43,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    52,    56,
      38,    59,    39,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    54,     2,    55,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,    36,    58,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    45,    46,    48,    49
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   116,   116,   120,   122,   127,   131,   132,   136,   137,
     141,   142,   146,   147,   148,   152,   156,   157,   161,   162,
     166,   167,   171,   175,   176,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   199,   200,   201,   205,   209,   210,   211,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TINT", "TBYTE", "TREFERENCE", "\"if\"",
  "\"else\"", "\"while\"", "\"return\"", "\"proc\"", "\"print\"",
  "\"true\"", "\"false\"", "\"==\"", "\"!=\"", "\"<=\"", "\">=\"",
  "\"writeInteger\"", "\"writeByte\"", "\"writeChar\"", "\"writeString\"",
  "\"readInteger\"", "\"readByte\"", "\"readChar\"", "\"readString\"",
  "\"extend\"", "\"shrink\"", "\"strlen\"", "\"strcmp\"", "\"strcpy\"",
  "\"strcat\"", "T_id", "T_INT_CONST", "T_CHAR_CONST", "T_STRING", "'|'",
  "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "UPLUS",
  "UMINUS", "'!'", "IF_PART", "NO_ELSE", "'('", "')'", "':'", "','", "'['",
  "']'", "';'", "'{'", "'}'", "'='", "$accept", "program", "func_def",
  "func_body", "fpar_def", "fpar_list", "type", "local_def", "var_def",
  "var_type", "data_type", "r_type", "compound_stmt", "stmt_list", "stmt",
  "l_value", "func_call", "expr_list", "expr", "cond", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   124,    38,    60,    62,
      43,    45,    42,    47,    37,   291,   292,    33,   293,   294,
      40,    41,    58,    44,    91,    93,    59,   123,   125,    61
};
# endif

#define YYPACT_NINF -133

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-133)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -16,   -33,    24,  -133,   -26,  -133,    -1,     6,  -133,    12,
     100,   130,    33,    61,  -133,  -133,    25,  -133,    52,  -133,
    -133,  -133,   130,  -133,  -133,    58,  -133,    -9,  -133,  -133,
      44,  -133,  -133,  -133,  -133,  -133,    25,    89,    69,    63,
      82,    94,   195,    99,   111,   112,   116,   117,   122,   127,
     -43,  -133,  -133,  -133,  -133,  -133,   119,   135,  -133,   159,
     147,   147,   143,   145,   146,   153,   154,   155,   156,  -133,
    -133,   -14,   -14,   -14,  -133,  -133,  -133,    87,   -14,   -14,
     -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,  -133,   160,
    -133,  -133,   147,   147,    40,    13,    75,   165,   174,   175,
     -14,   -14,   -14,   -14,  -133,  -133,   236,   -14,   -14,   -14,
     -14,   -14,  -133,   248,   253,   265,   270,   197,   212,   217,
      85,   333,    31,   158,  -133,  -133,   169,    86,   -14,   -14,
     -14,   -14,   -14,   -14,   147,   147,   133,   133,  -133,  -133,
    -133,   282,   287,   299,   231,  -133,   113,   113,  -133,  -133,
    -133,   176,   177,   178,   186,   199,   -14,   -14,  -133,   -14,
    -133,  -133,  -133,   333,   333,   333,   333,   333,   333,   206,
    -133,   237,  -133,  -133,  -133,  -133,   -14,  -133,  -133,  -133,
    -133,   196,   304,   316,   333,   133,   321,  -133,  -133,  -133,
    -133,  -133
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     2,     0,     1,     0,     0,     8,     0,
       0,     0,     0,     0,    18,    19,     0,     6,    10,    21,
      20,    12,     0,     9,     7,     0,     4,     0,    12,    11,
       0,    23,    14,    13,     5,     3,     0,     0,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41,    43,    25,    22,    27,    24,     0,     0,    15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    48,
      49,     0,     0,     0,    33,    50,    52,     0,     0,     0,
       0,     0,     0,     0,     0,    45,     0,     0,    28,     0,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    58,    59,     0,     0,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,     0,
       0,    46,     0,     0,    17,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,     0,     0,     0,     0,    51,    53,    54,    55,    56,
      57,     0,     0,     0,     0,     0,     0,     0,    44,     0,
      42,    26,    69,    75,    76,    73,    74,    71,    72,    78,
      77,    29,    31,    64,    63,    66,     0,    34,    35,    36,
      38,     0,     0,     0,    47,     0,     0,    37,    39,    40,
      30,    65
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -133,  -133,   219,   220,   249,  -133,   233,  -133,  -133,  -133,
      11,   241,   239,  -133,  -132,   -37,   -35,  -133,   -41,   -58
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,    26,     8,     9,    17,    27,    33,    38,
      18,    21,    54,    37,    55,    75,    76,   120,    94,    95
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      56,    77,    57,    96,   171,   172,     6,    85,    62,    63,
      64,    86,    65,    66,    67,    68,     1,     4,    50,    69,
      70,    51,    20,    30,     5,     7,    71,    72,    14,    15,
     104,   105,   106,    20,   125,   127,    73,   113,   114,   115,
     116,   117,   118,   119,   121,   122,   123,    39,    31,   134,
     135,    10,   126,   190,   128,   129,   130,   131,    11,   141,
     142,   143,   144,    12,   136,    13,   146,   147,   148,   149,
     150,   107,   108,   109,   110,   111,   169,   170,   132,   133,
     107,   108,   109,   110,   111,    22,   160,   163,   164,   165,
     166,   167,   168,     6,     4,    40,    36,    41,    42,    56,
      56,    57,    57,    14,    15,    16,    25,    43,    44,    45,
      46,   134,   135,    29,    47,   182,   183,    59,   184,    48,
      49,    50,   134,   135,    51,    58,   137,   107,   108,   109,
     110,   111,    60,    14,    15,   186,   158,   162,   159,    40,
      19,    41,    42,   112,    61,    52,    31,    53,    56,    78,
      57,    43,    44,    45,    46,   109,   110,   111,    47,    90,
      91,    79,    80,    48,    49,    50,    81,    82,    51,    62,
      63,    64,    83,    65,    66,    67,    68,    84,    87,    50,
      69,    70,    51,   128,   129,   130,   131,    71,    72,    52,
      31,    88,    89,    97,    92,    98,    99,    93,   107,   108,
     109,   110,   111,   100,   101,   102,   103,   132,   133,   107,
     108,   109,   110,   111,   161,   124,   138,    62,    63,    64,
     145,    65,    66,    67,    68,   139,   140,    50,    69,    70,
      51,   181,   177,   178,   179,    71,    72,   107,   108,   109,
     110,   111,   180,   135,   185,    73,    32,   187,    35,    24,
     155,    74,   107,   108,   109,   110,   111,   107,   108,   109,
     110,   111,    23,    28,     0,   156,    34,     0,     0,     0,
     157,   107,   108,   109,   110,   111,   107,   108,   109,   110,
     111,     0,     0,     0,   176,     0,     0,   145,   107,   108,
     109,   110,   111,   107,   108,   109,   110,   111,     0,   151,
       0,     0,     0,     0,   152,   107,   108,   109,   110,   111,
     107,   108,   109,   110,   111,     0,   153,     0,     0,     0,
       0,   154,   107,   108,   109,   110,   111,   107,   108,   109,
     110,   111,     0,   173,     0,     0,     0,     0,   174,   107,
     108,   109,   110,   111,   107,   108,   109,   110,   111,     0,
     175,     0,     0,     0,     0,   188,   107,   108,   109,   110,
     111,   107,   108,   109,   110,   111,     0,   189,     0,     0,
       0,     0,   191,   107,   108,   109,   110,   111
};

static const yytype_int16 yycheck[] =
{
      37,    42,    37,    61,   136,   137,    32,    50,    22,    23,
      24,    54,    26,    27,    28,    29,    32,    50,    32,    33,
      34,    35,    11,    32,     0,    51,    40,    41,     3,     4,
      71,    72,    73,    22,    92,    93,    50,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    36,    57,    36,
      37,    52,    93,   185,    14,    15,    16,    17,    52,   100,
     101,   102,   103,    51,    51,    53,   107,   108,   109,   110,
     111,    40,    41,    42,    43,    44,   134,   135,    38,    39,
      40,    41,    42,    43,    44,    52,    55,   128,   129,   130,
     131,   132,   133,    32,    50,     6,    52,     8,     9,   136,
     137,   136,   137,     3,     4,     5,    54,    18,    19,    20,
      21,    36,    37,    55,    25,   156,   157,    54,   159,    30,
      31,    32,    36,    37,    35,    56,    51,    40,    41,    42,
      43,    44,    50,     3,     4,   176,    51,    51,    53,     6,
      10,     8,     9,    56,    50,    56,    57,    58,   185,    50,
     185,    18,    19,    20,    21,    42,    43,    44,    25,    12,
      13,    50,    50,    30,    31,    32,    50,    50,    35,    22,
      23,    24,    50,    26,    27,    28,    29,    50,    59,    32,
      33,    34,    35,    14,    15,    16,    17,    40,    41,    56,
      57,    56,    33,    50,    47,    50,    50,    50,    40,    41,
      42,    43,    44,    50,    50,    50,    50,    38,    39,    40,
      41,    42,    43,    44,    56,    55,    51,    22,    23,    24,
      51,    26,    27,    28,    29,    51,    51,    32,    33,    34,
      35,    32,    56,    56,    56,    40,    41,    40,    41,    42,
      43,    44,    56,    37,     7,    50,    27,    51,    28,    16,
      53,    56,    40,    41,    42,    43,    44,    40,    41,    42,
      43,    44,    13,    22,    -1,    53,    27,    -1,    -1,    -1,
      53,    40,    41,    42,    43,    44,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    53,    -1,    -1,    51,    40,    41,
      42,    43,    44,    40,    41,    42,    43,    44,    -1,    51,
      -1,    -1,    -1,    -1,    51,    40,    41,    42,    43,    44,
      40,    41,    42,    43,    44,    -1,    51,    -1,    -1,    -1,
      -1,    51,    40,    41,    42,    43,    44,    40,    41,    42,
      43,    44,    -1,    51,    -1,    -1,    -1,    -1,    51,    40,
      41,    42,    43,    44,    40,    41,    42,    43,    44,    -1,
      51,    -1,    -1,    -1,    -1,    51,    40,    41,    42,    43,
      44,    40,    41,    42,    43,    44,    -1,    51,    -1,    -1,
      -1,    -1,    51,    40,    41,    42,    43,    44
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    32,    61,    62,    50,     0,    32,    51,    64,    65,
      52,    52,    51,    53,     3,     4,     5,    66,    70,    10,
      70,    71,    52,    64,    66,    54,    63,    67,    71,    55,
      32,    57,    62,    68,    72,    63,    52,    73,    69,    70,
       6,     8,     9,    18,    19,    20,    21,    25,    30,    31,
      32,    35,    56,    58,    72,    74,    75,    76,    56,    54,
      50,    50,    22,    23,    24,    26,    27,    28,    29,    33,
      34,    40,    41,    50,    56,    75,    76,    78,    50,    50,
      50,    50,    50,    50,    50,    50,    54,    59,    56,    33,
      12,    13,    47,    50,    78,    79,    79,    50,    50,    50,
      50,    50,    50,    50,    78,    78,    78,    40,    41,    42,
      43,    44,    56,    78,    78,    78,    78,    78,    78,    78,
      77,    78,    78,    78,    55,    79,    78,    79,    14,    15,
      16,    17,    38,    39,    36,    37,    51,    51,    51,    51,
      51,    78,    78,    78,    78,    51,    78,    78,    78,    78,
      78,    51,    51,    51,    51,    53,    53,    53,    51,    53,
      55,    56,    51,    78,    78,    78,    78,    78,    78,    79,
      79,    74,    74,    51,    51,    51,    53,    56,    56,    56,
      56,    32,    78,    78,    78,     7,    78,    51,    51,    51,
      74,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    62,    62,    63,    64,    64,    65,    65,
      66,    66,    67,    67,    67,    68,    69,    69,    70,    70,
      71,    71,    72,    73,    73,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    75,    75,    75,    76,    77,    77,    77,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     7,     6,     2,     3,     4,     1,     3,
       1,     3,     0,     2,     2,     4,     1,     4,     1,     1,
       1,     1,     3,     0,     2,     1,     4,     1,     2,     5,
       7,     5,     3,     2,     5,     5,     5,     6,     5,     6,
       6,     1,     4,     1,     4,     0,     1,     3,     1,     1,
       1,     3,     1,     3,     3,     3,     3,     3,     2,     2,
       3,     3,     3,     4,     4,     6,     4,     1,     1,     3,
       2,     3,     3,     3,     3,     3,     3,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 116 "parser.y" /* yacc.c:1646  */
    { t = (yyval.f) = (yyvsp[0].f); }
#line 1442 "parser.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 121 "parser.y" /* yacc.c:1646  */
    { (yyval.f) = new FunctionAST(new PrototypeAST((yyvsp[-1].t), *(yyvsp[-6].s), *(yyvsp[-4].vfpar)), (yyvsp[0].fb)); }
#line 1448 "parser.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 123 "parser.y" /* yacc.c:1646  */
    { (yyval.f) = new FunctionAST(new PrototypeAST((yyvsp[-1].t), *(yyvsp[-5].s), std::vector<std::pair<std::string, Type *>>()), (yyvsp[0].fb)); }
#line 1454 "parser.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 127 "parser.y" /* yacc.c:1646  */
    { (yyval.fb) = new FuncBody_AST(*(yyvsp[-1].vdef), (yyvsp[0].block)); }
#line 1460 "parser.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 131 "parser.y" /* yacc.c:1646  */
    { (yyval.fpar) = new std::pair<std::string, Type*>(*(yyvsp[-2].s), (yyvsp[0].t)); }
#line 1466 "parser.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 132 "parser.y" /* yacc.c:1646  */
    { (yyval.fpar) = new std::pair<std::string, Type*>(*(yyvsp[-3].s), PointerType::getUnqual((yyvsp[0].t))); }
#line 1472 "parser.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 136 "parser.y" /* yacc.c:1646  */
    { (yyval.vfpar) = new std::vector<std::pair<std::string, Type*>>(); (yyval.vfpar)->push_back(*(yyvsp[0].fpar)); }
#line 1478 "parser.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 137 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].vfpar)->push_back(*(yyvsp[0].fpar)); }
#line 1484 "parser.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 141 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 1490 "parser.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 142 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = PointerType::getUnqual((yyvsp[-2].t)); }
#line 1496 "parser.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 146 "parser.y" /* yacc.c:1646  */
    { (yyval.vdef) = new std::vector<LocalDef_AST *>(); }
#line 1502 "parser.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 147 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-1].vdef)->push_back((yyvsp[0].ld)); }
#line 1508 "parser.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 148 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-1].vdef)->push_back((yyvsp[0].ld)); }
#line 1514 "parser.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 152 "parser.y" /* yacc.c:1646  */
    { (yyval.vardef) = new VarDef(std::pair<std::string, Type*>(*(yyvsp[-3].s), (yyvsp[-1].t)));  }
#line 1520 "parser.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 156 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 1526 "parser.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 157 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = ArrayType::get((yyvsp[-3].t), (yyvsp[-1].n)); }
#line 1532 "parser.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 161 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = Type::getInt16Ty(TheContext); }
#line 1538 "parser.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 162 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = Type::getInt8Ty(TheContext); }
#line 1544 "parser.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 166 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 1550 "parser.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 167 "parser.y" /* yacc.c:1646  */
    { (yyval.t) = Type::getVoidTy(TheContext); }
#line 1556 "parser.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 171 "parser.y" /* yacc.c:1646  */
    { (yyval.block) = (yyvsp[-1].block); }
#line 1562 "parser.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 175 "parser.y" /* yacc.c:1646  */
    { (yyval.block) = new CompoundStmt_StmtAST(); }
#line 1568 "parser.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 176 "parser.y" /* yacc.c:1646  */
    { if ((yyvsp[0].stmt) != NULL) (yyvsp[-1].block)->statements.push_back((yyvsp[0].stmt)); }
#line 1574 "parser.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 180 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = NULL; }
#line 1580 "parser.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 181 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new Assignment_StmtAST((yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 1586 "parser.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 182 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = (yyvsp[0].block); }
#line 1592 "parser.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 183 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = (yyvsp[-1].stmt); }
#line 1598 "parser.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 184 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new If_StmtAST((yyvsp[-2].cond), (yyvsp[0].stmt), NULL);  }
#line 1604 "parser.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 185 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new If_StmtAST((yyvsp[-4].cond), (yyvsp[-2].stmt), (yyvsp[0].stmt));  }
#line 1610 "parser.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 186 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new While_StmtAST((yyvsp[-2].cond), (yyvsp[0].stmt)); }
#line 1616 "parser.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 187 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new Return_Stmt((yyvsp[-1].expr)); }
#line 1622 "parser.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 188 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new Return_Stmt(nullptr); }
#line 1628 "parser.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 189 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new WriteInteger((yyvsp[-2].expr)); }
#line 1634 "parser.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 190 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new WriteByte((yyvsp[-2].expr)); }
#line 1640 "parser.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 191 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new WriteChar((yyvsp[-2].expr)); }
#line 1646 "parser.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 192 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new ReadString((yyvsp[-3].expr), new Id_ExprAST(*(yyvsp[-1].s))); }
#line 1652 "parser.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 193 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new WriteString((yyvsp[-2].expr)); }
#line 1658 "parser.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 194 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new Strcpy((yyvsp[-3].expr), (yyvsp[-1].expr));  }
#line 1664 "parser.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 195 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new Strcat((yyvsp[-3].expr), (yyvsp[-1].expr));  }
#line 1670 "parser.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 199 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Id_ExprAST(*(yyvsp[0].s)); }
#line 1676 "parser.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 200 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArrayElement_ExprAST(*(yyvsp[-3].s), (yyvsp[-1].expr)); }
#line 1682 "parser.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 201 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new StringLiteral_ExprAST(*(yyvsp[0].s)); }
#line 1688 "parser.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 205 "parser.y" /* yacc.c:1646  */
    { (yyval.stmt) = new FuncCall(*(yyvsp[-3].s), *(yyvsp[-1].vexpr)); }
#line 1694 "parser.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 209 "parser.y" /* yacc.c:1646  */
    { (yyval.vexpr) = new ExpressionList(); }
#line 1700 "parser.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 210 "parser.y" /* yacc.c:1646  */
    { (yyval.vexpr) = new ExpressionList(); (yyval.vexpr)->push_back((yyvsp[0].expr)); }
#line 1706 "parser.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 211 "parser.y" /* yacc.c:1646  */
    { (yyvsp[-2].vexpr)->push_back((yyvsp[0].expr)); }
#line 1712 "parser.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 215 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new IntConst_ExprAST((yyvsp[0].n)); }
#line 1718 "parser.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 216 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new CharConst_ExprAST((yyvsp[0].c)); }
#line 1724 "parser.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 217 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1730 "parser.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 218 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 1736 "parser.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 219 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1742 "parser.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 220 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST((yyvsp[-2].expr), PLUS, (yyvsp[0].expr), 0); }
#line 1748 "parser.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 221 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST((yyvsp[-2].expr), MINUS, (yyvsp[0].expr), 0); }
#line 1754 "parser.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 222 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST((yyvsp[-2].expr), TIMES, (yyvsp[0].expr), 0); }
#line 1760 "parser.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 223 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST((yyvsp[-2].expr), DIV, (yyvsp[0].expr), 0); }
#line 1766 "parser.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 224 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST((yyvsp[-2].expr), MOD, (yyvsp[0].expr), 0); }
#line 1772 "parser.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 225 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), PLUS, (yyvsp[0].expr), 1); }
#line 1778 "parser.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 226 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ArithmeticOp_ExprAST(new IntConst_ExprAST(0), MINUS, (yyvsp[0].expr), 1); }
#line 1784 "parser.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 227 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ReadInteger(); }
#line 1790 "parser.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 228 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ReadByte();    }
#line 1796 "parser.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 229 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new ReadChar();    }
#line 1802 "parser.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 230 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Shrink((yyvsp[-1].expr));    }
#line 1808 "parser.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 231 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Extend((yyvsp[-1].expr));    }
#line 1814 "parser.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 232 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Strcmp((yyvsp[-3].expr),(yyvsp[-1].expr)); }
#line 1820 "parser.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 233 "parser.y" /* yacc.c:1646  */
    { (yyval.expr) = new Strlen((yyvsp[-1].expr)); }
#line 1826 "parser.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 237 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST(new IntConst_ExprAST(0), L, new IntConst_ExprAST(1)); }
#line 1832 "parser.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 238 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST(new IntConst_ExprAST(1), L, new IntConst_ExprAST(0)); }
#line 1838 "parser.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 239 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = (yyvsp[-1].cond);                                                                    }
#line 1844 "parser.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 240 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new LogicalOp_CondAST((yyvsp[0].cond), NOT, (yyvsp[0].cond));                                    }
#line 1850 "parser.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 241 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST((yyvsp[-2].expr), L, (yyvsp[0].expr));                                   }
#line 1856 "parser.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 242 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST((yyvsp[-2].expr), G, (yyvsp[0].expr));                                   }
#line 1862 "parser.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 243 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST((yyvsp[-2].expr), LE, (yyvsp[0].expr));                                  }
#line 1868 "parser.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 244 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST((yyvsp[-2].expr), GE, (yyvsp[0].expr));                                  }
#line 1874 "parser.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 245 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST((yyvsp[-2].expr), EQ, (yyvsp[0].expr));                                  }
#line 1880 "parser.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 246 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new ComparisonOp_CondAST((yyvsp[-2].expr), NE, (yyvsp[0].expr));                                  }
#line 1886 "parser.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 247 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new LogicalOp_CondAST((yyvsp[-2].cond), AND, (yyvsp[0].cond));                                    }
#line 1892 "parser.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 248 "parser.y" /* yacc.c:1646  */
    { (yyval.cond) = new LogicalOp_CondAST((yyvsp[-2].cond), OR, (yyvsp[0].cond));                                     }
#line 1898 "parser.cpp" /* yacc.c:1646  */
    break;


#line 1902 "parser.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 251 "parser.y" /* yacc.c:1906  */


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
