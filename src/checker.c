#include "include/checker.h"

void checkFunctionDefinition(AST* ast) {

    //int return_type = ast->data_type;



}

void checkExpression(AST* ast) {



}

void checkProgram(AST* root) {

    AST** children = (AST**) root->children->items;

    // Checking all the children of the program root node
    for (int i = 0; i < root->children->used; i++) {

        switch (children[i]->node_type) {

            case AST_FUNCTION_DEFINITION: checkFunctionDefinition(children[i]); break;
                

            
            default:
                break;
        }

    }
    

}