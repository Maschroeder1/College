// Trabalho de Mikael Arnez Schroeder

#include "tacs.h"

TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2) {
    TAC* newtac;
    
    newtac = (TAC*) calloc(1,sizeof(TAC));
    newtac-> type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    newtac->next = 0;
    
    return newtac;
}

void tacPrint(TAC* tac) {
    if (!tac) return;

    fprintf(stderr, "TAC(");

    switch (tac->type) {
        case TAC_SYMBOL: fprintf(stderr, "TAC_SYMBOL"); break;
        case TAC_ADD: fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB: fprintf(stderr, "TAC_SUB"); break;
        case TAC_MOVE: fprintf(stderr, "TAC_MOVE"); break;
        case TAC_MUL: fprintf(stderr, "TAC_MUL"); break;
        case TAC_DIV: fprintf(stderr, "TAC_DIV"); break;
        case TAC_GE: fprintf(stderr, "TAC_GE"); break;
        case TAC_LE: fprintf(stderr, "TAC_LE"); break;
        case TAC_EQ: fprintf(stderr, "TAC_EQ"); break;
        case TAC_DIF: fprintf(stderr, "TAC_DIF"); break;
        case TAC_PIPE: fprintf(stderr, "TAC_PIPE"); break;
        case TAC_G: fprintf(stderr, "TAC_G"); break;
        case TAC_L: fprintf(stderr, "TAC_L"); break;
        case TAC_AND: fprintf(stderr, "TAC_AND"); break;
        case TAC_VET_APPEND: fprintf(stderr, "TAC_VET_APPEND"); break;
        default: fprintf(stderr, "TAC_UNKNOWN"); break;
    }

    fprintf(stderr, ",%s", (tac->res) ? tac->res->text : "0");
    fprintf(stderr, ",%s", (tac->op1) ? tac->op1->text : "0");
    fprintf(stderr, ",%s", (tac->op2) ? tac->op2->text : "0");
    fprintf(stderr, ");\n");
}

void tacPrintBackwards(TAC *tac) {
    if (tac) {
        tacPrintBackwards(tac->prev);
        tacPrint(tac);
    }
}

TAC* tacJoin(TAC* l1, TAC* l2) {
    TAC* point;
    
    if (!l1) return l2;
    if (!l2) return l1;
    
    for (point = l2; point->prev != 0; point = point->prev);
    point->prev = l1;
    
    return l2;
}

TAC* generateCode(AST *node) {
    int i;
    TAC* result = 0;
    TAC *code[MAX_SONS];

    if (!node)  {
        return 0;
    }

    for (i = 0; i < MAX_SONS; i++) {
        code[i] = generateCode(node->son[i]);
    }

    switch (node->type) {
        case AST_SYMBOL:
            result = tacCreate(TAC_SYMBOL,node->symbol,0,0);
            break;
        case AST_ADD:
            result = generateBinOp(TAC_ADD, code[0], code[1]);
            break;
        case AST_SUB:
            result = generateBinOp(TAC_SUB, code[0], code[1]);
            break;
        case AST_MUL:
            result = generateBinOp(TAC_MUL, code[0], code[1]);
            break;
        case AST_DIV:
            result = generateBinOp(TAC_DIV, code[0], code[1]);
            break;
        case AST_GE:
            result = generateBinOp(TAC_GE, code[0], code[1]);
            break;
        case AST_LE:
            result = generateBinOp(TAC_LE, code[0], code[1]);
            break;
        case AST_EQ:
            result = generateBinOp(TAC_EQ, code[0], code[1]);
            break;
        case AST_DIF:
            result = generateBinOp(TAC_DIF, code[0], code[1]);
            break;
        case AST_PIPE:
            result = generateBinOp(TAC_PIPE, code[0], code[1]);
            break;
        case AST_G:
            result = generateBinOp(TAC_G, code[0], code[1]);
            break;
        case AST_L:
            result = generateBinOp(TAC_L, code[0], code[1]);
            break;
        case AST_AND:
            result = generateBinOp(TAC_AND, code[0], code[1]);
            break;
        case AST_INT:
        case AST_FLOAT:
        case AST_CHAR:
        case AST_LEAF_BRACKET_OPTIONAL: // vai ter que separar por causa do optional
            result = code[0];
            break;
        case AST_ATTR:
        case AST_VAR:
            result = tacJoin(code[1], tacCreate(TAC_MOVE, safeGet(code[0]), safeGet(code[1]), 0));
            //result = tacJoin(code[0], tacCreate(TAC_COPY, node->son[1]->symbol, safeGet(code[0]), 0));
            break;
        default:
            result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
            break;
    }
    
    return result;
}

TAC* generateBinOp(int type, TAC* op1, TAC* op2) {
    TAC* temp = tacJoin(tacJoin(op1, op2), tacCreate(type, makeTemp(), safeGet(op1), safeGet(op2)));
    return temp;
}

HASH_NODE* safeGet(TAC* something) {
    if (something) {
        return something->res;
    } else {
        return 0;
    }
}