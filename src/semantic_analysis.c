#include "include/semantic_analysis.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern Symbol_Table* symbol_table_main;

// TODO: Finish this
void analyseExpression(AST* ast) {

    assert(AST_MAX == 17 && "analyseExpression() must handle all AST types");

    switch (ast->ast_type) {

        case AST_VAR_DECLARATION    : analyseVarDeclaration(ast);   return;
        case AST_VAR_DEFINITION     : analyseVarDefinition(ast);    return;

        default:
            break;

    }

    printf("ERROR_PLACEHOLDER\n");
    exit(ERROR_PLACEHOLDER);

    return;
}

// TODO: Handle variable return better
void analyseReturn(AST* ast, AST* function_ast) {

    int literal_type = ast->ast_value->ast_type;
    int data_type = function_ast->data_type;

    // TODO?: Maybe turn this into a switch case
    if (literal_type == AST_NOOP && data_type != TYPE_VOID) {
        printf("Return type mismatch at line %d\n", ast->line);
        exit(ERROR_RETURN_TYPE_MISMATCH);
    }

    else if (literal_type == AST_INT_LITERAL && (data_type == TYPE_FLOAT || data_type == TYPE_DOUBLE)) {
        printf("Return type mismatch at line %d\n", ast->line);
        exit(ERROR_RETURN_TYPE_MISMATCH);
    }

    else if (literal_type == AST_FLOAT_LITERAL && (data_type != TYPE_FLOAT && data_type != TYPE_DOUBLE)) {
        printf("Return type mismatch at line %d\n", ast->line);
        exit(ERROR_RETURN_TYPE_MISMATCH);
    }

    else if (literal_type == AST_CHAR_LITERAL && data_type != TYPE_i8) {
        printf("Return type mismatch at line %d\n", ast->line);
        exit(ERROR_RETURN_TYPE_MISMATCH);
    }

    // TODO: check this
    else if (literal_type == AST_STRING_LITERAL && data_type != TYPE_i64) {
        printf("Return type mismatch at line %d\n", ast->line);
        exit(ERROR_RETURN_TYPE_MISMATCH);
    }

    ast->ast_value->data_type = function_ast->data_type;

    return;
}

void analyseFunctionDeclaration(AST* ast) {

    if (searchSymbolNode(ast->scope, ast->name) != NULL) {
        printf("Redeclaration of function '%s' at line %d\n", ast->name, ast->line);
        exit(ERROR_FUNCTION_REDECLARATION);
    }

    insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_FUNCTION, -1, -1, NULL);

    return;
}

void analyseFunctionDefinition(AST* ast) {

    Symbol_Table* function_body_scope = createSymbolTable(ast->scope);

    appendChildScope(ast->scope, function_body_scope);

    if (ast->args_params != NULL) {
        AST** params = (AST**) ast->args_params->items;
        for (int i = 0; i < ast->args_params->used; i++) {
    
            params[i]->scope = function_body_scope;
            
            if (searchSymbolNode(function_body_scope, params[i]->name) != NULL) {
                printf("Redeclaration of parameter '%s' at line %d\n", ast->name, ast->line);
                exit(ERROR_PARAMETER_REDECLARATION);
            }
    
            insertInSymbolTable(function_body_scope, params[i]->name, params[i]->data_type, SYMBOL_PARAMETER, params[i]->line, -1, NULL);
    
        }
    }


    if (ast->children != NULL) {
        AST** children = (AST**) ast->children->items;
        for (int i = 0; i < ast->children->used; i++) {
    
            children[i]->scope = function_body_scope;
    
            if (children[i]->ast_type == AST_RETURN) analyseReturn(children[i], ast);
    
            else analyseExpression(children[i]);
    
        }
    }

    Symbol_Node* func_declaration = searchSymbolNode(ast->scope, ast->name);

    if (func_declaration != NULL) {
        func_declaration->line_definition = ast->line;
    }

    else {
        insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_FUNCTION, ast->line, -1, NULL);
    }

    return;
}

void analyseVarDeclaration(AST* ast) {

    // Searches current scope and all parent scopes
    Symbol_Table* scope_to_search = ast->scope;

    while (scope_to_search != NULL) {

        if (searchSymbolNode(scope_to_search, ast->name) != NULL) {
            printf("Redeclaration of variable '%s' at line %d\n", ast->name, ast->line);
            exit(ERROR_VAR_REDECLARATION);
        }

        scope_to_search = scope_to_search->parent;

    }

    insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_VAR, -1, -1, NULL);

    return;
}

void analyseVarDefinition(AST* ast) {

    // Searches current scope and all parent scopes

    Symbol_Table* scope_to_search = ast->scope;

    while (scope_to_search != NULL) {

        if (searchSymbolNode(scope_to_search, ast->name) != NULL) {
            printf("Redeclaration of variable '%s' at line %d\n", ast->name, ast->line);
            exit(ERROR_VAR_REDECLARATION);
        }

        scope_to_search = scope_to_search->parent;

    }

    int literal_type = ast->ast_value->ast_type;
    int data_type = ast->data_type;

    // TODO?: Maybe turn this into a switch case
    if (literal_type == AST_INT_LITERAL && (data_type == TYPE_FLOAT || data_type == TYPE_DOUBLE)) {
        printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_TYPE_MISMATCH);
    }

    else if (literal_type == AST_FLOAT_LITERAL && (data_type != TYPE_FLOAT && data_type != TYPE_DOUBLE)) {
        printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_TYPE_MISMATCH);
    }

    else if (literal_type == AST_CHAR_LITERAL && data_type != TYPE_i8) {
        printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_TYPE_MISMATCH);
    }

    // TODO: check this
    else if (literal_type == AST_STRING_LITERAL && data_type != TYPE_i64) {
        printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_TYPE_MISMATCH);
    }

    insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_VAR, ast->line, -1, ast->ast_value->literal_value);

}

void analyseProgram(AST* root) {

    assert(AST_MAX == 17 && "analyseProgram() must handle all AST types");

    root->scope = symbol_table_main;

    AST** children = (AST**) root->children->items;

    // Checking all the children of the program root node
    for (int i = 0; i < root->children->used; i++) {

        children[i]->scope = symbol_table_main;

        switch (children[i]->ast_type) {

            case AST_FUNCTION_DECLARATION   : analyseFunctionDeclaration(children[i]);  break;
            case AST_FUNCTION_DEFINITION    : analyseFunctionDefinition(children[i]);   break;
            case AST_VAR_DECLARATION        : analyseVarDeclaration(children[i]);       break;
            case AST_VAR_DEFINITION         : analyseVarDefinition(children[i]);        break;

            default:
                printf("PLACEHOLDER ERROR\n");
                exit(ERROR_PLACEHOLDER);
                break;
        }

    }

    return;
}