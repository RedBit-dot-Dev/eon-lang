#include "include/AST.h"
#include <stdlib.h>
#include <assert.h>

AST* createAST(int type) {

    AST* ast = (AST*) malloc(sizeof(AST));
    assert(ast && "Could not allocate memory for the AST");

    ast->node_type = type;
    ast->children = NULL;
    ast->value = NULL;

    if (type == AST_COMPOUND) ast->children = createVector(sizeof(AST*));

    return ast;
}