%{
#include <string.h>
#include <iostream>
#include <regex>
#include <fstream>
#include "ast.hpp"
#include "parser.hpp"
#define T_eof 0

int linenumber = 1;
int comm_num = 0;
using namespace std;

void yyerror (const char *msg);

string *fixEscapeChars (char* stringToFix, char mode) {
    std::string *temp = new std::string(stringToFix);

    // ofstream stdfile;
    // stdfile.open("stdout.txt");

    // string s ("there is a subsequence in the string\n");
    // regex e ("\\b(sub)([^ ]*)");
    // stdfile << "WTF" << endl;
    // stdfile << regex_replace (s,e,"sub-$2");
    // // std::cout << std::regex_replace ("",(x([0-9]|[A-Fa-f])([0-9]|[A-Fa-f]) ),temp);
    // std::cout << temp->length() << std::endl;
    int counter = 0;
    for(int i=0; stringToFix[i]!='\0'; i++ ) {
        char c[2] ;
        c[1]={'\0'};
         if( (stringToFix[i]==('\\')) ) {
            switch(stringToFix[i+1]){
                case 'n':
                    c[0] ='\n'; 
                    (*temp).replace(i - counter++,2, c);
                    break;
                case 't':
                    c[0] = '\t'; 
                    (*temp).replace(i - counter++,2, c);
                    break;
                case 'r':
                    c[0] = '\r'; 
                    (*temp).replace(i - counter++,2, c);
                    break;
                case '0':
                    c[0] = '\0'; 
                    (*temp).replace(i - counter++,2, c);
                     //null is considered as end of string and not included in it so can't return it, so we manually return the ascii value of null
                     if (mode == 'c')  {
                             (*temp)[1] = 0;
                             return temp;
                     }
                    break;
                case '\\':
                    c[0] = '\\';
                    (*temp).replace(i - counter++,2, c);
                    i++;//Needed because \ can escape next charcter also.
                    break;
                case '\'':
                    c[0] = '\''; 
                    (*temp).replace(i - counter++,2, c);
                    break;
                case '\"':
                    c[0] = '\"'; 
                    (*temp).replace(i - counter++,2, c);
                    break;
                case 'x':
                {
                    char d1 = stringToFix[i+2];
                    char d2;
                    if(d1 != '\0') {
                        d2 = stringToFix[i+3];
                    }
                    else break;

                    if ( (d1>='0' && d1<='9') || (d1>='a' && d1<='f') || (d1>='A' && d1<='F') ) {
                        if( (d2>='0' && d2<='9') || (d2>='a' && d2<='f') || (d2>='A' && d2<='F'))
                            c[0] = (d1-'0')*10 + d2-'0';
                            (*temp).replace(i - counter,4, c);
                            counter+=3;
                            i+=3;
                    }
                    break;
                }
                default:
                    break;
            }
        }
    }
    return temp;
}

int hex2bin( const char *s )
{
    int ret=0;
    int i;
    for( i=0; i<2; i++ )
    {
        char c = *s++;
        int n=0;
        if( '0'<=c && c<='9' )
            n = c-'0';
        else if( 'a'<=c && c<='f' )
            n = 10 + c-'a';
        else if( 'A'<=c && c<='F' )
            n = 10 + c-'A';
        ret = n + ret*16;
    }
    return ret;
}

%}

L [a-zA-Z]
D [0-9]
W [ \t\r]
HEX ({D}|[A-Fa-f])
ES "\\n"|"\\t"|"\\r"|"\\0"|"\\\\"|"\\'"|"\\\""|"\\x"{HEX}{2}
CC ([^'\"\\]|{ES})

%x COMMENT
%x CHARACTER

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

[-=+*/%!&|<>]           { return yytext[0]; }
[()}{,:;]               { return yytext[0]; }
"["                     { return yytext[0]; }
"]"                     { return yytext[0]; }
"<="				    { return LE_OP; }
">="				    { return GE_OP; }
"=="				    { return EQ_OP; }
"!="				    { return NEQ_OP; }

'                       { BEGIN(CHARACTER); }
<CHARACTER>\\x{HEX}{2}  { yylval.c = hex2bin(&yytext[2]); return T_CHAR_CONST; }
<CHARACTER>\\0          { yylval.c = 0; return T_CHAR_CONST; }
<CHARACTER>\\n          { yylval.c = 10; return T_CHAR_CONST; }
<CHARACTER>\\t          { yylval.c = 9; return T_CHAR_CONST; }
<CHARACTER>\\r          { yylval.c = 13; return T_CHAR_CONST; }
<CHARACTER>\\\\         { yylval.c = 92; return T_CHAR_CONST; }
<CHARACTER>\\\"         { yylval.c = 34; return T_CHAR_CONST; }
<CHARACTER>[^'\"\\]     { yylval.c = yytext[0]; return T_CHAR_CONST; }
<CHARACTER>'            { BEGIN(INITIAL); }

\"([^'\"\\\n]|{ES})*\"  { yylval.s = fixEscapeChars(yytext, 's'); return T_STRING; }
{L}({D}|{L}|_)*         { yylval.s = new std::string(yytext); return T_id; }
{D}+                    { yylval.n = atoi(yytext); return T_INT_CONST; }
\n                      { linenumber++;  /* ignore new lines */ }
{W}+                    { /* ignore other white space */ }
"--".*                  { }
"(*"                    { comm_num++; BEGIN(COMMENT); }
<COMMENT>"(*"           { comm_num++;}
<COMMENT>\n             { linenumber++; }
<COMMENT>"*)"           { if (--comm_num == 0) BEGIN(INITIAL); }
<COMMENT>"*"            { /* nothing */ }
<COMMENT>"("            { /* nothing */ }
<COMMENT>[^*(\n]+       { /* nothing */ }
.                       { yyerror("illegal character"); }
%%