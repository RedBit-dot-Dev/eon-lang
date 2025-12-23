#include "include/AST.h"
#include <stdlib.h>
#include <assert.h>

AST* createAST(int type) {

    AST* ast = (AST*) malloc(sizeof(AST));
    assert(ast && "Could not allocate memory for the AST");

    ast->name = NULL;
    ast->literal_value = NULL;
    ast->children = NULL;
    ast->value = NULL;
    ast->args_params = NULL;
    ast->node_type = type;
    ast->data_type = -1;

    if (type == AST_COMPOUND) ast->children = createVector(sizeof(AST*));

    return ast;
}