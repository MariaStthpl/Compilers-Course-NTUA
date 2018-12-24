%{
#include <string.h>
#include "ast.hpp"
#include "parser.hpp"
#define T_eof 0

int linenumber = 1;
int comm_num = 0;

void yyerror (const char *msg);
%}

L [a-zA-Z]
D [0-9]
W [ \t\r]
HEX ({D}|[A-Fa-f])
ES "\\n"|"\\t"|"\\r"|"\\0"|"\\\\"|"\\'"|"\\\""|"\\x"{HEX}{2}
CC ([^'\"\\]|{ES})

%x COMMENT

%%

"int"                   { return TINT; }
"byte"                  { return TBYTE; }
"if"                    { return TIF; }
"else"                  { return TELSE; }
"true"                  { return TTRUE; }
"false"                 { return TFALSE; }
"proc"                  { return TPROC; }
"reference"             { return TREFERENCE; }
"return"                { return TRETURN; }
"while"                 { return TWHILE; }


"writeInteger"          { return TW_INT; }
"writeByte"             { return TW_BYTE; }
"writeChar"             { return TW_CHAR; }
"writeString"           { return TW_STRING; }

"readInteger"           { return TR_INT; }
"readByte"              { return TR_BYTE; }
"readChar"              { return TR_CHAR; }
"readString"            { return TR_STRING; }

"extend"                { return T_EXNTEND; }
"shrink"                { return T_SHRINK; }

"strlen"                { return T_STRLEN; }
"strcmp"                { return T_STRCMP; }
"strcpy"                { return T_STRCPY; }
"strcat"                { return T_STRCAT; }                

"print"                 { return T_print; }

[-=+*/%!&|<>]           { return yytext[0]; }
[()}{,:;]               { return yytext[0]; }
"["                     { return yytext[0]; }
"]"                     { return yytext[0]; }
"<="				    { return LE_OP; }
">="				    { return GE_OP; }
"=="				    { return EQ_OP; }
"!="				    { return NEQ_OP; }

'{CC}'                  { return T_CHAR_CONST; }
    /* \"([^'\"\\\n]|{ES})*\"  { yylval.s = yytext; return T_NAME; return T_STRING; } */
    /* \"([^'\"\\\n]|{ES})*\"  { yylval.s = (char *) new(strlen(yytext) + 1); strcpy((char *) ( yylval.s), yytext); return T_NAME; return T_STRING; } */

\"([^'\"\\\n]|{ES})*\"  { yylval.s = new std::string(yytext); return T_STRING; }

    /* {L}({D}|{L}|_)*         { yylval.s = yytext; return T_NAME; } */
    /* {L}({D}|{L}|_)*         { yylval.s = (char *) new(strlen(yytext) + 1); strcpy((char *) ( yylval.s), yytext); return T_NAME; } */
{L}({D}|{L}|_)*         { yylval.s = new std::string(yytext); return T_id; }
{D}+                    { yylval.n = atoi(yytext); return T_INT_CONST; }

\n                      { linenumber++;  /* ignore new lines */ }
{W}+                    { /* ignore other white space */ }

"--".*\n                { printf("COMMENT\n"); }

"(*"                    { comm_num++; BEGIN(COMMENT); }

<COMMENT>"(*"           { comm_num++;}
<COMMENT>\n             { linenumber++; }
<COMMENT>"*)"           { if (--comm_num == 0) { printf("COMMENT\n"); BEGIN(INITIAL); } }
<COMMENT>"*"            { /* nothing */ }
<COMMENT>"("            { /* nothing */ }
<COMMENT>[^*(\n]+       { /* nothing */ }

.                       { yyerror("illegal character"); }

%%