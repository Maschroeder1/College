#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

#define LEVEL_EMPTY "    "

void printNode(AST *node, int level);
void printChildren(AST *node, int level);
void printType(int type);
void printValue(AST *node);
void printClosing(int level);
void printIndentation(int level);

AST *astCreate(int type, HASH_NODE* symbol, AST* s0, AST* s1, AST* s2, AST* s3) {
    AST* newNode;
    newNode = (AST*) calloc(1, sizeof(AST));
    newNode->type = type;
    newNode->symbol = symbol;
    newNode->son[0] = s0;
    newNode->son[1] = s1;
    newNode->son[2] = s2;
    newNode->son[3] = s3;
    return newNode;
}

void astPrint(AST *node, int level) {
    if (node == 0) {
        return;
    }

    printNode(node, level);
    printChildren(node, level);
    printClosing(level);
}

void printNode(AST *node, int level) {
    printIndentation(level);
    fprintf(stderr, "ast(");
    
    printType(node->type);
    printValue(node);
}

void printType(int type) {
    switch (type) {
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
        case AST_ADD: fprintf(stderr, "AST_ADD"); break;
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL"); break;
        case AST_DIV: fprintf(stderr, "AST_DIV"); break;
        case AST_GE: fprintf(stderr, "AST_GE"); break;
        case AST_LE: fprintf(stderr, "AST_LE"); break;
        case AST_EQ: fprintf(stderr, "AST_EQ"); break;
        case AST_DIF: fprintf(stderr, "AST_DIF"); break;
        case AST_PIPE: fprintf(stderr, "AST_PIPE"); break;
        case AST_G: fprintf(stderr, "AST_G"); break;
        case AST_L: fprintf(stderr, "AST_L"); break;
        case AST_AND: fprintf(stderr, "AST_AND"); break;
        case AST_TILDA: fprintf(stderr, "AST_TILDA"); break;
        case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
        case AST_CMD_BLOCK: fprintf(stderr, "AST_CMD_BLOCK"); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
        case AST_PRINT_BLOCK: fprintf(stderr, "AST_PRINT_BLOCK"); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
        case AST_IF: fprintf(stderr, "AST_IF"); break;
        case AST_ELSE: fprintf(stderr, "AST_ELSE"); break;
        case AST_BRACKET_EXPR: fprintf(stderr, "AST_BRACKET_EXPR"); break;
        case AST_LEAF_BRACKET_OPTIONAL: fprintf(stderr, "AST_LEAF_BRACKET_OPTIONAL"); break;
        case AST_UNTIL: fprintf(stderr, "AST_UNTIL"); break;
        case AST_COME_FROM: fprintf(stderr, "AST_COME_FROM"); break;
        case AST_READ: fprintf(stderr, "AST_READ"); break;
        case AST_FUN: fprintf(stderr, "AST_FUN"); break;
        case AST_FUN_ARG: fprintf(stderr, "AST_FUN_ARG"); break;
        default: fprintf(stderr, "dafuq"); break;
    }
}

void printValue(AST *node) {
    if (node->symbol != 0) {
        fprintf(stderr, ",%s\n", node->symbol->text);
    } else {
        fprintf(stderr, ",0\n");
    }
}

void printChildren(AST *node, int level) {
    int i = 0;
    for (i=0; i<MAX_SONS; ++i) {
        astPrint(node->son[i], level+1);
    }
}

void printClosing(int level) {
    printIndentation(level);
    fprintf(stderr, ")\n");
}

void printIndentation(int level) {
    int i = 0;
    for (i=0; i<level; i++) {
        fprintf(stderr, LEVEL_EMPTY);
    }
}
