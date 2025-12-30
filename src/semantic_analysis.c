#include "include/semantic_analysis.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern Symbol_Table* symbol_table_main;

void analyseVarDeclaration(AST* ast) {

    if (searchSymbolNode(ast->scope, ast->name) != NULL) {
        printf("Redeclaration of variable '%s' at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_REDECLARATION);
    }
    
    insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_VAR, -1, -1, NULL);
    
    return;
}

void analyseVarDefinition(AST* ast) {
    
    assert(AST_MAX == 17 && TYPE_MAX == 13 && "analyseVarDefinition() must handle all AST and data types");

    if (searchSymbolNode(ast->scope, ast->name) != NULL) {
        printf("Redeclaration of variable '%s' at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_REDECLARATION);
    }

    switch (ast->ast_value->ast_type) {

        case AST_INT_LITERAL:

            if (ast->data_type == TYPE_FLOAT || ast->data_type == TYPE_DOUBLE) {
                printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
                exit(ERROR_VAR_TYPE_MISMATCH);
            }

            break;

        case AST_FLOAT_LITERAL:

            if (ast->data_type != TYPE_FLOAT && ast->data_type != TYPE_DOUBLE) {
                printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
                exit(ERROR_VAR_TYPE_MISMATCH);
            }

            break;

        case AST_CHAR_LITERAL:

            if (ast->data_type != TYPE_i8) {
                printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
                exit(ERROR_VAR_TYPE_MISMATCH);
            }

            break;

        case AST_STRING_LITERAL:

            // TODO: Make this

            break;

        case AST_VARIABLE:

            Symbol_Node* var = searchSymbolNode(ast->scope, ast->ast_value->name);

            if (var == NULL) {
                printf("Variable '%s' not defined at line %d\n", ast->ast_value->name, ast->line);
                exit(ERROR_VAR_NOT_DEFINED);
            }

            var->line_usage = ast->line;

            if (var->data_type != ast->data_type) {
                printf("Type of variable '%s' doesn't match '%s' at line %d\n", var->name, ast->name, ast->line);
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_VAR, ast->line, -1, NULL);

            return;

        default:
            break;
    }

    // This makes literals get their correct data type instead of defaulting to something like i32
    ast->ast_value->data_type = ast->data_type;

    insertInSymbolTable(ast->scope, ast->name, ast->data_type, SYMBOL_VAR, ast->line, -1, ast->ast_value->literal_value);

    return;
}

void analyseAssignment(AST* ast) {

    assert(AST_MAX == 17 && TYPE_MAX == 13 && "analyseAssignment() must handle all AST and data types");

    Symbol_Node* var_lhs = searchSymbolNode(ast->scope, ast->name);

    if (var_lhs == NULL) {
        printf("Variable '%s' not defined at line %d\n", ast->name, ast->line);
        exit(ERROR_VAR_NOT_DEFINED);
    }

    var_lhs->line_definition = ast->line;

    switch (ast->ast_value->ast_type) {

        case AST_INT_LITERAL:

            if (ast->data_type == TYPE_FLOAT || ast->data_type == TYPE_DOUBLE) {
                printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
                exit(ERROR_VAR_TYPE_MISMATCH);
            }

            break;

        case AST_FLOAT_LITERAL:

            if (ast->data_type != TYPE_FLOAT && ast->data_type != TYPE_DOUBLE) {
                printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
                exit(ERROR_VAR_TYPE_MISMATCH);
            }

            break;

        case AST_CHAR_LITERAL:

            if (ast->data_type != TYPE_i8) {
                printf("Type of variable '%s' doesn't match it's assigned value at line %d\n", ast->name, ast->line);
                exit(ERROR_VAR_TYPE_MISMATCH);
            }

            break;

        case AST_STRING_LITERAL:

            // TODO: Make this

            break;

        case AST_VARIABLE:

            Symbol_Node* var_rhs = searchSymbolNode(ast->scope, ast->ast_value->name);

            if (var_rhs == NULL) {
                printf("Variable '%s' not defined at line %d\n", ast->ast_value->name, ast->line);
                exit(ERROR_VAR_NOT_DEFINED);
            }

            var_rhs->line_usage = ast->line;

            if (var_rhs->data_type != var_lhs->data_type) {
                printf("Type of variable '%s' doesn't match '%s' at line %d\n", var_rhs->name, var_lhs->name, ast->line);
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            // This is here so that all mentions of the variable also know it's data type, otherwise it's unknown at write time
            ast->ast_value->data_type = var_rhs->data_type;

            return;

        default:
            break;
    }

    // This makes literals get their correct data type instead of defaulting to something like i32
    ast->ast_value->data_type = var_lhs->data_type;

    return;
}

// TODO: Finish this
void analyseExpression(AST* ast) {

    assert(AST_MAX == 17 && "analyseExpression() must handle all AST types");

    switch (ast->ast_type) {

        case AST_VAR_DECLARATION    : analyseVarDeclaration(ast);   return;
        case AST_VAR_DEFINITION     : analyseVarDefinition(ast);    return;
        case AST_ASSIGNMENT         : analyseAssignment(ast);       return;

        default:
            break;

    }

    printf("AnalyseExpression() you dumbass\n");
    exit(ERROR_PLACEHOLDER);

    return;
}

// TODO: Handle variable return better
void analyseReturn(AST* ast, AST* function_ast) {

    assert(AST_MAX == 17 && TYPE_MAX == 13 && "analyseReturn() must handle all AST and data types");

    int return_data_type = function_ast->data_type;

    switch (ast->ast_value->ast_type) {

        case AST_NOOP:

            if (return_data_type != TYPE_VOID) {
                printf("Return type mismatch at line %d\nFunction '%s' doesn't return any value (type void)\n", ast->line, function_ast->name);
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            break;

        case AST_INT_LITERAL:

            if (return_data_type == TYPE_FLOAT || return_data_type == TYPE_DOUBLE) {
                printf("Return type mismatch at line %d\nFunction '%s' returns an integer\n", ast->line, function_ast->name);
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            break;

        case AST_FLOAT_LITERAL:

            if (return_data_type != TYPE_FLOAT && return_data_type != TYPE_DOUBLE) {
                printf("Return type mismatch at line %d\nFunction '%s' returns a float/double\n", ast->line, function_ast->name);
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            break;

        case AST_CHAR_LITERAL:

            if (return_data_type != TYPE_i8) {
                printf("Return type mismatch at line %d\nFunction '%s' returns a character\n", ast->line, function_ast->name);
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            break;

        case AST_STRING_LITERAL:

            // TODO: Make this

            break;

        case AST_VARIABLE:

            Symbol_Node* var = searchSymbolNode(ast->scope, ast->ast_value->name);

            if (var == NULL) {
                printf("Variable '%s' not defined at line %d\n", ast->ast_value->name, ast->line);
                exit(ERROR_VAR_NOT_DEFINED);
            }

            var->line_usage = ast->line;

            if (var->data_type != return_data_type) {
                printf("Return type mismatch at line %d\nFunction '%s' return type doesn't data type of variable '%s'",
                    ast->line,
                    function_ast->name,
                    ast->ast_value->name
                );
                exit(ERROR_RETURN_TYPE_MISMATCH);
            }

            break;

        default:
            break;
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
            
            if (searchScope(function_body_scope, params[i]->name) != NULL) {
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
                printf("analyseProgram() you dumbass\n");
                exit(ERROR_PLACEHOLDER);
                break;
        }

    }

    return;
}