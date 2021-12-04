// Trabalho de Mikael Arnez Schroeder

#include "tacs.h"

TAC* passThrough(TAC* code);
TAC* generateBracketExpression(TAC* code);
TAC* generateAttrExpression(TAC* code0, TAC* code1, TAC* code2, AST* optionalSon);
TAC* generateFunBody(TAC* code0, TAC* code2);
TAC* generateVector(TAC* code1, TAC* code2, AST* optionalSon);
TAC* generateVectorInit(TAC* code0, TAC* code1, AST* optionalSon);

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
    if (!tac || tac->type == TAC_SYMBOL) return;

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
        case TAC_TILDA: fprintf(stderr, "TAC_TILDA"); break;
        case TAC_BEGINFUN: fprintf(stderr, "TAC_BEGINFUN"); break;
        case TAC_ENDFUN: fprintf(stderr, "TAC_ENDFUN"); break;
        case TAC_RETURN: fprintf(stderr, "TAC_RETURN"); break;
        case TAC_READ: fprintf(stderr, "TAC_READ"); break;
        case TAC_OPEN_BRACKET: fprintf(stderr, "TAC_OPEN_BRACKET"); break;
        case TAC_CLOSE_BRACKET: fprintf(stderr, "TAC_CLOSE_BRACKET"); break;
        default: fprintf(stderr, "TAC_UNKNOWN TAC_UNKNOWN TAC_UNKNOWN TAC_UNKNOWN TAC_UNKNOWN TAC_UNKNOWN"); break;
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
            return tacCreate(TAC_SYMBOL,node->symbol,0,0);
        case AST_ADD:
            return generateBinOp(TAC_ADD, code[0], code[1]);
        case AST_SUB:
            return generateBinOp(TAC_SUB, code[0], code[1]);
        case AST_MUL:
            return generateBinOp(TAC_MUL, code[0], code[1]);
        case AST_DIV:
            return generateBinOp(TAC_DIV, code[0], code[1]);
        case AST_GE:
            return generateBinOp(TAC_GE, code[0], code[1]);
        case AST_LE:
            return generateBinOp(TAC_LE, code[0], code[1]);
        case AST_EQ:
            return generateBinOp(TAC_EQ, code[0], code[1]);
        case AST_DIF:
            return generateBinOp(TAC_DIF, code[0], code[1]);
        case AST_PIPE:
            return generateBinOp(TAC_PIPE, code[0], code[1]);
        case AST_G:
            return generateBinOp(TAC_G, code[0], code[1]);
        case AST_L:
            return generateBinOp(TAC_L, code[0], code[1]);
        case AST_AND:
            return generateBinOp(TAC_AND, code[0], code[1]);
        case AST_TILDA:
            return tacJoin(code[1], tacCreate(TAC_TILDA, makeTemp(), safeGetResult(code[0]), 0));
        case AST_RETURN:
            return tacJoin(code[0], tacCreate(TAC_RETURN, safeGetResult(code[0]),0 ,0));
        case AST_READ:
            return tacCreate(TAC_READ, makeTemp(), 0, 0);
        case AST_INT:
        case AST_FLOAT:
        case AST_CHAR:
        case AST_BRACKET_EXPR:
            return passThrough(code[0]);
        case AST_ATTR:
            return generateAttrExpression(code[0], code[1], code[2], node->son[2]);
        case AST_VAR:
            return tacJoin(code[1], tacCreate(TAC_MOVE, safeGetResult(code[0]), safeGetResult(code[1]), 0));
        case AST_VET_INT:
        case AST_VET_CHAR:
        case AST_VET_FLOAT:
            return generateVector(code[1], code[2], node->son[2]);
        case AST_VET_INIT:
            return generateVectorInit(code[0], code[1], node->son[1]);
        case AST_FUN_INT:
        case AST_FUN_CHAR:
        case AST_FUN_FLOAT:
            return generateFunBody(code[0], code[2]);
        case AST_LEAF_BRACKET_OPTIONAL:
            if (node->son[1]) {
                return tacJoin(code[0], generateBracketExpression(code[1]));
            } else {
                return code[0];
            }
        case AST_CMD_BLOCK:
            return tacJoin(code[0], code[1]);
        default:
            return tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
    }
}

TAC* generateBinOp(int type, TAC* op1, TAC* op2) {
    return tacJoin(tacJoin(op1, op2), tacCreate(type, makeTemp(), safeGetResult(op1), safeGetResult(op2)));
}

TAC* passThrough(TAC* code) {
    return code;
}

TAC* generateBracketExpression(TAC* code) {
    HASH_NODE *label = makeLabel();
    
    return tacJoin(tacCreate(TAC_OPEN_BRACKET, label, 0,0),
                   tacJoin(code, 
                           tacCreate(TAC_CLOSE_BRACKET, label, 0,0)));
}

TAC* generateAttrExpression(TAC* code0, TAC* code1, TAC* code2, AST* optionalSon) {
    if (optionalSon) {
        // como marcar a parte do salvar assign de vetor?
        return tacJoin(code1, tacCreate(TAC_MOVE, safeGetResult(code2), safeGetResult(code1), 0));
    } else {
        return tacJoin(code1, tacCreate(TAC_MOVE, safeGetResult(code0), safeGetResult(code1), 0));
    }
}

TAC* generateFunBody(TAC* code0, TAC* code2) {
    return tacJoin(
            tacCreate(TAC_BEGINFUN, safeGetResult(code0), 0, 0),
            tacJoin(
                    code2,
                    tacCreate(TAC_ENDFUN, safeGetResult(code0), 0, 0)));
}

TAC* generateVector(TAC* code1, TAC* code2, AST* optionalSon) {
    // pensando num vetor ser tipo cons(elem1, cons(elem2, cons(elem3, null)))
    if (optionalSon) {
        return tacJoin(code2, tacCreate(TAC_VET_APPEND, safeGetResult(code1), safeGetResult(code2), 0));
    }
    else {
        return tacCreate(TAC_VET_APPEND, safeGetResult(code1),0,0);
    }
}

TAC* generateVectorInit(TAC* code0, TAC* code1, AST* optionalSon) {
    if (optionalSon) {
        return tacJoin(code1, tacCreate(TAC_VET_APPEND, safeGetResult(code0), safeGetResult(code1), 0));
    }
    else {
        return code0;
    }
}

HASH_NODE* safeGetResult(TAC* something) {
    if (something) {
        return something->res;
    } else {
        return 0;
    }
}