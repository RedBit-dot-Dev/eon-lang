#include "include/ir_gen.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int global_counter = 1;

int getAlignSize(int data_type) {

    assert(TYPE_MAX == 13 && "getAlignSize() must handle all data types");

    switch (data_type) {

        case TYPE_i8        : return 1;
        case TYPE_i16       : return 2;
        case TYPE_i32       : return 4;
        case TYPE_i64       : return 8;
        case TYPE_i128      : return 16;
        case TYPE_u8        : return 1;      // LLVM ir doesn't recognize unsingned
        case TYPE_u16       : return 2;     // LLVM ir doesn't recognize unsingned
        case TYPE_u32       : return 4;     // LLVM ir doesn't recognize unsingned
        case TYPE_u64       : return 8;     // LLVM ir doesn't recognize unsingned
        case TYPE_u128      : return 16;    // LLVM ir doesn't recognize unsingned
        case TYPE_FLOAT     : return 4;
        case TYPE_DOUBLE    : return 8;

        default:
            break;
    }

    printf("Data type unrecognized\n");
    exit(ERROR_DATA_TYPE_UNRECOGNIZED);

    return -1;
}

void writeVarDeclaration(AST* ast, FILE* file) {

    Symbol_Node* var = searchSymbolNode(ast->scope, ast->name);

    char* data_type_string = dataTypeToString(var->data_type);
    int align_size = getAlignSize(var->data_type);

    fprintf(file, "%%%s = alloca %s, align %d\n", var->name, data_type_string, align_size);

    return;
}

void writeVarDefinition(AST* ast, FILE* file) {

    Symbol_Node* var = searchSymbolNode(ast->scope, ast->name);

    char* data_type_string = dataTypeToString(var->data_type);
    int align_size = getAlignSize(var->data_type);

    fprintf(file, "%%%s = alloca %s, align %d\n", var->name, data_type_string, align_size);

    // Only occurs if initialized with another variable
    if (var->literal_value == NULL) {

        fprintf(file, "\t%%%d = load %s, %s* %%%s, align %d\n", //!FIXME: assumed to be 1 tab
            global_counter,
            data_type_string,
            data_type_string,
            ast->ast_value->name,
            align_size
        );

        fprintf(file, "\tstore %s %%%d, %s* %%%s\n", //!FIXME: assumed to be 1 tab
            data_type_string,
            global_counter++,
            data_type_string,
            var->name
        );

    }

    else {

        fprintf(file, "\tstore %s %s, %s* %%%s\n", //!FIXME: assumed to be 1 tab
            data_type_string,
            var->literal_value,
            data_type_string,
            var->name
        );

    }

    return;
}

void writeReturn(AST* ast, FILE* file) {

    char* data_type_string = dataTypeToString(ast->ast_value->data_type);
    int align_size = getAlignSize(ast->ast_value->data_type);

    if (ast->ast_value->ast_type != AST_VARIABLE) {

        if (ast->ast_value->ast_type == AST_NOOP) {
            fprintf(file, "ret void\n");
            return;
        }

        fprintf(file, "ret %s %s\n", data_type_string, ast->ast_value->literal_value);
        return;
    }

    fprintf(file, "%%%d = load %s, %s* %%%s, align %d\n",
        global_counter,
        data_type_string,
        data_type_string,
        ast->ast_value->name,
        align_size
    );

    fprintf(file, "\tret %s %%%d\n", data_type_string, global_counter++); //!FIXME: assumed to be 1 tab

    return;
}

// TODO: Finish this
void writeExpression(AST* ast, FILE* file, int tab_count) {

    assert(AST_MAX == 17 && "writeExpression() must handle all AST types");

    for (int i = 0; i < tab_count; i++) {
        fprintf(file, "\t");
    }

    switch (ast->ast_type) {

        case AST_VAR_DECLARATION    : writeVarDeclaration(ast, file);   break;
        case AST_VAR_DEFINITION     : writeVarDefinition(ast, file);    break;
        case AST_RETURN             : writeReturn(ast, file);           break;

        default:
            printf("ERROR_PLACEHOLDER\n");
            exit(ERROR_PLACEHOLDER);
            break;
    }

    return;
}

void writeParameters(AST* ast, FILE* file) {

    fprintf(file, "(");

    if (ast->args_params != NULL) {

        AST** params = (AST**) ast->args_params->items;

        // Writing first param
        fprintf(file, "%s %%%s", dataTypeToString(params[0]->data_type), params[0]->name);

        // Writing the rest
        for (int i = 1; i < ast->args_params->used; i++) {
            fprintf(file, ", %s %%%s", dataTypeToString(params[i]->data_type), params[i]->name);
        }

    }

    fprintf(file, ")");

    return;
}

void writeFunctionDeclaration(AST* ast, FILE* file) {

    fprintf(file, "declare %s @%s", dataTypeToString(ast->data_type), ast->name);
    writeParameters(ast, file);

    return;
}

void writeFunctionDefinition(AST* ast, FILE* file) {

    global_counter = 1;

    fprintf(file, "define %s @%s", dataTypeToString(ast->data_type), ast->name);
    writeParameters(ast, file);

    fprintf(file, " {\nentry:\n");

    if (ast->children != NULL) {

        AST** children = (AST**) ast->children->items;

        for (int i = 0; i < ast->children->used; i++) {
            writeExpression(children[i], file, 1);
        }

    }

    else {
        fprintf(file, "ret void\n");
    }

    fprintf(file, "}");

    return;
}

void writeGlobalVarDeclaration(AST* ast, FILE* file) {

    Symbol_Node* var = searchSymbolNode(ast->scope, ast->name);

    // Global variables always need to be initialized with a starting value (in this case, 0)
    fprintf(file, "@%s = global %s 0, align %d", var->name, dataTypeToString(var->data_type), getAlignSize(var->data_type));

    return;
}

void writeGlobalVarDefinition(AST* ast, FILE* file) {

    Symbol_Node* var = searchSymbolNode(ast->scope, ast->name);

    fprintf(file, "@%s = global %s %s, align %d",
        var->name,
        dataTypeToString(var->data_type),
        var->literal_value,
        getAlignSize(var->data_type)
    );

}

void writeProgram(AST* root, FILE* file) {

    assert(AST_MAX == 17 && "writeProgram() must handle all AST types");

    fprintf(file, "target triple = \"x86_64-pc-linux-gnu\"");// TODO: Make this architecture agnostic

    AST** children = (AST**) root->children->items;
    for (int i = 0; i < root->children->used; i++) {

        fprintf(file, "\n\n");

        switch (children[i]->ast_type) {

            case AST_FUNCTION_DECLARATION   : writeFunctionDeclaration(children[i], file);      break;
            case AST_FUNCTION_DEFINITION    : writeFunctionDefinition(children[i], file);       break;
            case AST_VAR_DECLARATION        : writeGlobalVarDeclaration(children[i], file);     break;
            case AST_VAR_DEFINITION         : writeGlobalVarDefinition(children[i], file);      break;

            default:
                printf("ERROR_PLACEHOLDER\n");
                exit(ERROR_PLACEHOLDER);
                break;
        }

    }

    return;
}