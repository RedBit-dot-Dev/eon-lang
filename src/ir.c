#include "include/ir.h"
#include "include/types.h"
#include <assert.h>

void writeFunctionDefinition(FILE* file, AST* ast) {

    fprintf(file, "define %s @%s(", dataTypeToString(ast->data_type), ast->name);

    Vector* children_vec = ast->args_params->children;
    AST** args_params = (AST**) children_vec->items;

    if (ast->args_params->children->used != 0) {

        fprintf(file, "%s %%%s", dataTypeToString(args_params[0]->data_type), args_params[0]->name);

        for (int i = 1; i < ast->args_params->children->used; i++) {
            fprintf(file, ", %s %%%s", dataTypeToString(args_params[i]->data_type), args_params[i]->name);
        }

    }

    fprintf(file, ") {\nentry:\n");

    Vector* body_vec = ast->value->children;
    AST** body_expressions = (AST**) body_vec->items;

    for (int i = 0; i < body_vec->used; i++) {
        writeExpression(file, body_expressions[i], 1);
    }

    fprintf(file, "\n}");

    return;
}

void writeExpression(FILE* file, AST* ast, int num_tabs) {

    for (int i = 0; i < num_tabs; i++) {
        fprintf(file, "\t");
    }

    switch (ast->node_type) {
        case AST_RETURN:

            fprintf(file, "ret %s", dataTypeToString(ast->value->data_type));

            if (ast->value->node_type != AST_NOOP) {
                fprintf(file, " %s", ast->value->literal_value); //TODO: FIXME Assuming is int 
            }

            break;
        
        default:
            break;
    }

}

void writeProgram(FILE* file, AST* root) {

    fprintf(file, "target triple = \"x86_64-pc-linux-gnu\"\n\n"); // TODO: make this architecture agnostic

    AST** children = (AST**) root->children->items;

    // Checking all the children of the program root node
    for (int i = 0; i < root->children->used; i++) {

        switch (children[i]->node_type) {

            case AST_FUNCTION_DEFINITION: writeFunctionDefinition(file, children[i]); break;
            
            default:
                break;
        }

    }

    return;
}

void buildProgram(AST* root) {

    FILE* file;

    file = fopen("IR.ll", "w");
    assert(file && "Could not create IR file");

    writeProgram(file, root);
    fclose(file);

    return;
}