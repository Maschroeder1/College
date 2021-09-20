
#include "hash.h"

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4

#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MUL 4
#define AST_DIV 5
#define AST_GE 6
#define AST_LE 7
#define AST_EQ 8
#define AST_DIF 9
#define AST_PIPE 10
#define AST_G 11
#define AST_L 12
#define AST_AND 13
#define AST_TILDA 14
#define AST_ATTR 15
#define AST_CMD_BLOCK 16
#define AST_PRINT 17
#define AST_PRINT_BLOCK 18
#define AST_RETURN 19
#define AST_IF 20
#define AST_ELSE 21
#define AST_BRACKET_EXPR 22
#define AST_LEAF_BRACKET_OPTIONAL 23
#define AST_UNTIL 24
#define AST_COME_FROM 25
#define AST_READ 26
#define AST_FUN 27
#define AST_FUN_ARG 28

typedef struct astnode {
    int type;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *astCreate(int type, HASH_NODE* symbol, AST* s0, AST* s1, AST* s2, AST* s3);

void astPrint(AST *node, int level);

#endif
