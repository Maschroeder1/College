%{
    #include "tokens.h"
    #include "hash.h"
    int running = 1;
    int lineNumber = 1;
%}

%x COMMENT

%%

char                                { return KW_CHAR; }
int                                 { return KW_INT; }
float                               { return KW_FLOAT; }
data                                { return KW_DATA; }
if                                  { return KW_IF; }
else                                { return KW_ELSE; }
until                               { return KW_UNTIL; }
comefrom                            { return KW_COMEFROM; }
read                                { return KW_READ; }
print                               { return KW_PRINT; }
return                              { return KW_RETURN; }

">="                                { return OPERATOR_GE; }
"<="                                { return OPERATOR_LE; }
"=="                                { return OPERATOR_EQ; }
"!="                                { return OPERATOR_DIF; }
".."                                { return OPERATOR_RANGE; }

[a-zA-Z]+[ a-zA-Z0-9]*[a-zA-Z0-9]+  { hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }
[a-zA-Z]+[a-zA-Z0-9]*               { hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }

'.'                                 { return LIT_CHAR; }
[0-9]+                              { return LIT_INTEGER; }
(?# match em abre aspas, sequencia de chars \" OU qualquer coisa que não seja " ou nova linha, parcialmente inspirado)
\"(\\\"|[^"\n])*\"                  { return LIT_STRING; }

[-,;:{}()\[\]+*/<>|&~=]             { return yytext[0]; }
"\n"                                { ++lineNumber; }
"#/"                                { BEGIN(COMMENT); }
[ \t]
"##".*
.                                   { return TOKEN_ERROR; }

<COMMENT>"\n"   { ++lineNumber; }
<COMMENT>"/#"   { BEGIN(INITIAL); }
<COMMENT>.

%%

#include "hash.c"
#include "main.c"

int isRunning(void) {
    return running;
}

int getLineNumber(void) {
    return lineNumber;
}

void initMe(void) {
    running = 1;
    lineNumber = 1;
    hashInit();
}

int yywrap() {
    running = 0;
    return 1;
}