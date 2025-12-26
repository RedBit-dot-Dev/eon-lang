#include "include/AST.h"
#include "include/arena.h"

extern Arena* arena_main;

AST* createAST(int ast_type) {

    AST* ast = (AST*) arenaPush(arena_main, sizeof(AST), false);

    ast->ast_type = ast_type;
    ast->data_type = -1;
    ast->line = -1;
    ast->name = NULL;
    ast->literal_value = NULL;
    ast->ast_value = NULL;
    ast->children = NULL;

    return ast;
}