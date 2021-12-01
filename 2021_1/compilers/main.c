// trabalho de Mikael Arnez schroeder
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "y.tab.h"

// partes da main do sor que estavam faltando e fazendo o clion gritar
int yylex();
extern char *yytext;
extern FILE *yyin;

void initMe(void);
int isRunning(void);
int getLineNumber(void);
void hashPrint(void);

void decompileTo(AST *ast, FILE *file);
void decompileData(AST *ast, FILE *file);
void decompileVector(AST *ast, FILE *file, char* type);
void decompileFunction(AST *ast, FILE *file, char* type);
void decompileAttribution(AST *ast, FILE *file);
void decompileBinaryOperator(AST *ast, FILE *file, char* type);
void simpleDecompile(AST *ast, FILE *file, char* type, char* fileMessage, int numChildren);
void validate(int argc);

int main(int argc, char **argv) {
    validate(argc);
    initMe();
    yyin = fopen(argv[1], "r");

    yyparse();

    astPrint(globalAst, 0);
    
    printf("ok\n");
    
    exit(0);
}

void validate(int argc) {
    if (argc < 2) {
        fprintf(stderr, "Unspecified file name\n");
        exit(1);
    }
}

void decompileTo(AST *ast, FILE *file) {
    fprintf(stderr, "Decompiling...\n");
    if (ast != 0) {
        switch (ast->type) {
            case AST_SYMBOL:
                fprintf(stderr, "Decompiling Symbol %s...\n", ast->symbol->text);
                fputs(ast->symbol->text, file);
                break;
            case AST_ADD:
                decompileBinaryOperator(ast, file, "+");
                break;
            case AST_SUB:
                decompileBinaryOperator(ast, file, "-");
                break;
            case AST_MUL:
                decompileBinaryOperator(ast, file, "*");
                break;
            case AST_DIV:
                decompileBinaryOperator(ast, file, "/");
                break;
            case AST_GE:
                decompileBinaryOperator(ast, file, ">=");
                break;
            case AST_LE:
                decompileBinaryOperator(ast, file, "<=");
                break;
            case AST_EQ:
                decompileBinaryOperator(ast, file, "==");
                break;
            case AST_DIF:
                decompileBinaryOperator(ast, file, "!=");
                break;
            case AST_PIPE:
                decompileBinaryOperator(ast, file, "|");
                break;
            case AST_G:
                decompileBinaryOperator(ast, file, ">");
                break;
            case AST_L:
                decompileBinaryOperator(ast, file, "<");
                break;
            case AST_AND:
                decompileBinaryOperator(ast, file, "&");
                break;
            case AST_TILDA:
                fprintf(stderr, "Decompiling ~...\n");
                fputs("~", file);
                decompileTo(ast->son[0], file);
                break;
            case AST_ATTR:
                decompileAttribution(ast, file);
                break;
            case AST_CMD_BLOCK:
                fprintf(stderr, "Decompiling command block...\n");
                decompileTo(ast->son[0], file);
                fputs(";\n", file);
                decompileTo(ast->son[1], file);
                break;
            case AST_PRINT:
                fprintf(stderr, "Decompiling print...\n");
                fputs("print ", file);
                decompileTo(ast->son[0], file);
                break;
            case AST_PRINT_BLOCK:
                fprintf(stderr, "Decompiling print block...\n");
                decompileTo(ast->son[0], file);
                if (ast->son[1] != 0) {
                    fputs(", ", file);
                    decompileTo(ast->son[1], file);
                }
                break;
            case AST_RETURN:
                fprintf(stderr, "Decompiling return...\n");
                fputs("return ", file);
                decompileTo(ast->son[0], file);
                break;
            case AST_IF:
                fprintf(stderr, "Decompiling if...\n");
                fputs("if (", file);
                decompileTo(ast->son[0], file);
                fputs(") ", file);
                decompileTo(ast->son[1], file);
                decompileTo(ast->son[2], file);
                break;
            case AST_ELSE:
                fprintf(stderr, "Decompiling else...\n");
                fputs(" else\n", file);
                decompileTo(ast->son[0], file);
                break;
            case AST_BRACKET_EXPR:
                fprintf(stderr, "Decompiling attr []...\n");
                fputs("[", file);
                decompileTo(ast->son[0], file);
                fputs("]", file);
                break;
            case AST_LEAF_BRACKET_OPTIONAL:
                fprintf(stderr, "Decompiling leaf opt []...\n");
                decompileTo(ast->son[0], file);
                decompileTo(ast->son[1], file);
                break;
            case AST_UNTIL:
                fprintf(stderr, "Decompiling until...\n");
                fputs("until (", file);
                decompileTo(ast->son[0], file);
                fputs(")\n", file);
                decompileTo(ast->son[1], file);
                break;
            case AST_COME_FROM:
                fprintf(stderr, "Decompiling come_from...\n");
                fputs("comefrom: ", file);
                decompileTo(ast->son[0], file);
                break;
            case AST_READ:
                fprintf(stderr, "Decompiling read...\n");
                fputs(" read ", file);
                break;
            case AST_FUN:
                fprintf(stderr, "Decompiling function...\n");
                decompileTo(ast->son[0], file);
                fputs("(", file);
                decompileTo(ast->son[1], file);
                fputs(")", file);
                break;
            case AST_FUN_ARG:
                fprintf(stderr, "Decompiling function arg...\n");
                decompileTo(ast->son[0], file);
                if (ast->son[1] != 0) {
                    fputs(", ", file);
                    decompileTo(ast->son[1], file);
                }
                break;
            case AST_FUN_DEC_PARAM:
                fprintf(stderr, "Decompiling function parameter...\n");
                decompileTo(ast->son[0], file);
                if (ast->son[1] != 0) {
                    fputs(", ", file);
                    decompileTo(ast->son[1], file);
                }
                break;
            case AST_INT:
                simpleDecompile(ast, file, "int", "int: ", 1);
                break;
            case AST_CHAR:
                simpleDecompile(ast, file, "char", "char: ", 1);
                break;
            case AST_FLOAT:
                simpleDecompile(ast, file, "float", "float: ", 1);
                break;
            case AST_FUN_BLOCK:
                fprintf(stderr, "Decompiling fun block...\n");
                decompileTo(ast->son[0], file);
                decompileTo(ast->son[1], file);
                break;
            case AST_FUN_INT:
                decompileFunction(ast, file, "int");
                break;
            case AST_FUN_CHAR:
                decompileFunction(ast, file, "char");
                break;
            case AST_FUN_FLOAT:
                decompileFunction(ast, file, "float");
                break;
            case AST_VAR:
                fprintf(stderr, "Decompiling var...\n");
                decompileTo(ast->son[0], file);
                fputs(" = ", file);
                decompileTo(ast->son[1], file);
                fputs(";\n", file);
                break;
            case AST_VET_RANGE:
                fprintf(stderr, "Decompiling vet range...\n");
                decompileTo(ast->son[0], file);
                fputs("..", file);
                decompileTo(ast->son[1], file);
                break;
            case AST_VET_INIT:
                fprintf(stderr, "Decompiling vet init...\n");
                decompileTo(ast->son[0], file);
                if (ast->son[1] != 0) {
                    fputs(" ", file);
                    decompileTo(ast->son[1], file);
                }
                break;
            case AST_DATA_SECT:
                decompileData(ast, file);
                break;
            case AST_INIT:
                simpleDecompile(ast, file, "init", "", 2);
                break;
            case AST_VET_CHAR:
                decompileVector(ast, file, "char");
                break;
            case AST_VET_INT:
                decompileVector(ast, file, "int");
                break;
            case AST_VET_FLOAT:
                decompileVector(ast, file, "float");
                break;
            case AST_EXPR_PARENT:
                fprintf(stderr, "Decompiling expr in ()...\n");
                fputs("(", file);
                decompileTo(ast->son[0], file);
                fputs(")", file);
                break;
            case AST_CMD_CURLY:
                fprintf(stderr, "Decompiling expr in {}...\n");
                fputs("{\n", file);
                decompileTo(ast->son[0], file);
                fputs("}", file);
                 break;
            default: fprintf(stderr, "erro erro erro erro erro erro erro erro erro erro erro erro erro erro"); exit(3);
        }
    }
}

void decompileData(AST* ast, FILE *file) {
    int isRoot = ast->son[1] == ast->son[0];
    
    fprintf(stderr, "Decompiling data...\n");
    if (isRoot) {
        fprintf(stderr, "Is root...\n");
        fputs("data {\n", file);
        decompileTo(ast->son[0], file);
        fputs("}\n", file);
    } else {
        decompileTo(ast->son[0], file);
        decompileTo(ast->son[1], file);
    }
}


void decompileVector(AST *ast, FILE *file, char* type) {
    int hasInitialization = ast->son[2] != 0;
    
    fprintf(stderr, "Decompiling vet %s...\n", type);
    fputs(type, file);
    fputs("[", file);
    decompileTo(ast->son[0], file);
    fputs("]: ", file);
    decompileTo(ast->son[1], file);
    
    if (hasInitialization) {
        fputs(" = ", file);
        decompileTo(ast->son[2], file);
    }
    
    fputs(";\n", file);
}


void decompileFunction(AST *ast, FILE *file, char* type) {
    fprintf(stderr, "Decompiling function %s...\n", type);
    
    fputs(type, file);
    fputs(": ", file);
    decompileTo(ast->son[0], file);
    fputs(" (", file);
    decompileTo(ast->son[1], file);
    fputs(") {\n", file);
    decompileTo(ast->son[2], file);
    fputs("}\n",file);
}


void decompileAttribution(AST *ast, FILE *file) {
    fprintf(stderr, "Decompiling attribution...\n");
    decompileTo(ast->son[0], file);
    decompileTo(ast->son[2], file);
    fputs(" = ", file);
    decompileTo(ast->son[1], file);
}

void decompileBinaryOperator(AST *ast, FILE *file, char* type) {
    fprintf(stderr, "Decompiling operator %s...\n", type);

    decompileTo(ast->son[0], file);
    fputs(" ", file);
    fputs(type, file);
    fputs(" ", file);
    decompileTo(ast->son[1], file);
}

void simpleDecompile(AST *ast, FILE *file, char* type, char* fileMessage, int numChildren) {
    int i;
    
    fprintf(stderr, "Decompiling %s...\n", type);
    fputs(fileMessage, file);
    
    for (i = 0; i < numChildren; i++) {
        decompileTo(ast->son[i], file);
    }
}

