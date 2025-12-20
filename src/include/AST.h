#ifndef EON_AST_H
#define EON_AST_H
#include "vector.h"
#include "types.h"

enum _ast_types{
    AST_INT_LITERAL,
    AST_FLOAT_LITERAL,
    AST_CHAR_LITERAL,
    AST_STRING_LITERAL,
    AST_COMPOUND,
    AST_FUNCTION_DECLARATION,
    AST_FUNCTION_DEFINITION,
    AST_FUNCTION_CALL,
    AST_RETURN,
    AST_PARAMETER,
    AST_ARGUMENT,
    AST_ASSIGNMENT,
    AST_VAR_DECLARATION, // int a;
    AST_VAR_DEFINITION, // int a = 3;
    AST_VARIABLE,
    AST_STATEMENT,
    AST_NOOP, // Sinalizes the AST is "null"
    AST_MAX,
};

typedef struct _ast {
    enum _ast_types node_type;
    enum _data_types data_type;
    Vector* children;
    char* name;
    int int_value;
    struct _ast* value;
    struct _ast* args_params;
} AST;

AST* createAST(int type);

#endif