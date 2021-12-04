// Trabalho de Mikael Arnez Schroeder

#ifndef TACS_HEADER
#define TACS_HEADER

#include <stdlib.h>

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MOVE 4
#define TAC_MUL 5
#define TAC_DIV 6
#define TAC_GE 7
#define TAC_LE 8
#define TAC_EQ 9
#define TAC_DIF 10
#define TAC_PIPE 11
#define TAC_G 12
#define TAC_L 13
#define TAC_AND 14
#define TAC_VET_APPEND 15
#define TAC_TILDA 16
#define TAC_BEGINFUN 17
#define TAC_ENDFUN 18
#define TAC_RETURN 19
#define TAC_READ 20
#define TAC_OPEN_BRACKET 21
#define TAC_CLOSE_BRACKET 22
#define TAC_PRINT 23
#define TAC_IFZ 24
#define TAC_JUMP 25
#define TAC_LABEL 26
#define TAC_ARG 27
#define TAC_CALL 28

typedef struct tac_node {
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);

void tacPrint(TAC* tac);

void tacPrintBackwards(TAC *tac);

TAC* tacJoin(TAC* l1, TAC* l2);
HASH_NODE* safeGetResult(TAC* something);

TAC* generateCode(AST *node);
TAC* generateBinOp(int op, TAC* op1, TAC* op2);

#endif
