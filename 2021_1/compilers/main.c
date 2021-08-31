#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

// partes da main do sor que estavam faltando e fazendo o clion gritar
int yylex();
extern char *yytext;
extern FILE *yyin;

void initMe(void);
int isRunning(void);
int getLineNumber(void);
void hashPrint(void);

void printToken(int token);
void validate(int argc);

int main(int argc, char **argv) {
    validate(argc);
    initMe();
    yyin = fopen(argv[1], "r");

    yyparse();
    
    hashPrint();
    printf("ok\n");
    
    exit(0);
}

void validate(int argc) {
    if (argc < 2) {
        fprintf(stderr, "Unspecified file name\n");
        exit(1);
    }
}

void printToken(int token) {
    switch(token) {
        case KW_CHAR: printf("KW_CHAR %d\n", getLineNumber()); break;
        case KW_INT: printf("KW_INT %d\n", getLineNumber()); break;
        case KW_FLOAT: printf("KW_FLOAT %d\n", getLineNumber()); break;
        case KW_DATA: printf("KW_DATA %d\n", getLineNumber()); break;
        case KW_IF: printf("KW_IF %d\n", getLineNumber()); break;
        case KW_ELSE: printf("KW_ELSE %d\n", getLineNumber()); break;
        case KW_UNTIL: printf("KW_UNTIL %d\n", getLineNumber()); break;
        case KW_COMEFROM: printf("KW_COMEFROM %d\n", getLineNumber()); break;
        case KW_READ: printf("KW_READ %d\n", getLineNumber()); break;
        case KW_PRINT: printf("KW_PRINT %d\n", getLineNumber()); break;
        case KW_RETURN: printf("KW_RETURN %d\n", getLineNumber()); break;
        case OPERATOR_LE: printf("OPERATOR_LE %d\n", getLineNumber()); break;
        case OPERATOR_GE: printf("OPERATOR_GE %d\n", getLineNumber()); break;
        case OPERATOR_EQ: printf("OPERATOR_EQ %d\n", getLineNumber()); break;
        case OPERATOR_DIF: printf("OPERATOR_DIF %d\n", getLineNumber()); break;
        case OPERATOR_RANGE: printf("OPERATOR_RANGE %d\n", getLineNumber()); break;
        case TK_IDENTIFIER: printf("TK_IDENTIFIER %d\n", getLineNumber()); break;
        case LIT_INTEGER: printf("LIT_INTEGER %d\n", getLineNumber()); break;
        case LIT_CHAR: printf("LIT_CHAR %d\n", getLineNumber()); break;
        case LIT_STRING: printf("LIT_STRING %d\n", getLineNumber()); break;
        case TOKEN_ERROR: printf("TOKEN_ERROR %d\n", getLineNumber()); break;
        default: printf("%c %d\n", token, getLineNumber()); break;
    }
}
