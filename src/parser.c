#include "include/parser.h"
#include "include/types.h"
#include "stdlib.h"
#include <assert.h>
#include <string.h>

// TODO: make single literals forbidden

Parser* createParser(Lexer* lexer) {

    Parser* parser = (Parser*) malloc(sizeof(Parser));
    assert(parser && "Could not allocate memory for the parser");

    parser->lexer = lexer;
    parser->token = lexerNextToken(lexer);

    return parser;
}

Token* parserEat(Parser* parser, int type) {

    Token* tok = parser->token; // just to shrink printf call

    if (tok->type != type) {

        printf("[Parser]: Unexpected token: '%s' '%s' at %d:%d\n, was expecting '%s'", 
            tok->value, 
            tokenIdToString(tok->type), 
            tok->line, tok->column, 
            tokenIdToString(type)
        );

        exit(1);
    }

    parser->token = lexerNextToken(parser->lexer);
    return parser->token;
}

// TODO: make this handle function calls
AST* parseId(Parser* parser) {

    char* name = malloc(parser->token->length + 1);
    assert(name && "Could not allocate memory for name string");
    strcpy(name, parser->token->value);
    parserEat(parser, TOKEN_ID);

    // if it's an assignment, store the variable name, then store it's value as the expression of the rhs of the assignment
    if (parser->token->type == TOKEN_EQUALS) {
        parserEat(parser, TOKEN_EQUALS);
        AST* ast = createAST(AST_ASSIGNMENT);
        ast->name = name;
        ast->value = parseExpression(parser);
        return ast;
    }

    AST* ast = createAST(AST_VARIABLE);
    ast->name = name;
    parserEat(parser, TOKEN_SEMI);

    return ast;
}

// TODO: handle sum, subtraction, division etc.
// TODO: think about this atoi, is it really necessary? I mean, I'm gonna write in a file right?
AST* parseIntLiteral(Parser* parser) {

    int int_value = atoi(parser->token->value);
    parserEat(parser, TOKEN_INT_LITERAL);

    AST* ast = createAST(AST_INT_LITERAL);
    ast->data_type = TYPE_i32; //! PLACEHOLDER?
    ast->literal_value.int_v = int_value;
    parserEat(parser, TOKEN_SEMI);

    return ast;
}

// TODO: make this compatible with complex data types such as vectors and linked lists
AST* parseDataType(Parser* parser) {

    int data_type = tokenToDataType(parser->token);
    parserEat(parser, TOKEN_DATATYPE);

    char* variable_name = malloc(parser->token->length + 1);
    assert(variable_name && "Could not allocate memory for variable_name string");
    strcpy(variable_name, parser->token->value);

    parserEat(parser, TOKEN_ID);

    if (parser->token->type == TOKEN_EQUALS) {
        parserEat(parser, TOKEN_EQUALS);
        AST* ast = createAST(AST_VAR_DEFINITION);
        ast->name = variable_name;
        ast->data_type = data_type;
        ast->value = parseExpression(parser);
        return ast;
    }

    AST* ast = createAST(AST_VAR_DECLARATION);
    ast->name = variable_name;
    ast->data_type = data_type;
    parserEat(parser, TOKEN_SEMI);

    return ast;
}

// This is used for expression order
// TODO: actually make this work
//! DO NOT use this for parsing for loops
AST* parseParenthesis(Parser* parser) {

    parserEat(parser, TOKEN_LPAREN);

    AST* ast = createAST(AST_COMPOUND);

    vectorPush(ast->children, parseExpression(parser));

    while (parser->token->type == TOKEN_COMMA) {
        parserEat(parser, TOKEN_COMMA);
        vectorPush(ast->children, parseExpression(parser));
    }

    parserEat(parser, TOKEN_RPAREN);

    return ast;
}

AST* parseSingleParameter(Parser* parser) {

    int data_type = tokenToDataType(parser->token);
    parserEat(parser, TOKEN_DATATYPE);

    char* parameter_name = malloc(parser->token->length + 1);
    assert(parameter_name && "Could not allocate memory for parameter_name string");
    strcpy(parameter_name, parser->token->value);

    parserEat(parser, TOKEN_ID);

    AST* ast = createAST(AST_PARAMETER);
    ast->data_type = data_type;
    ast->name = parameter_name;

    if (parser->token->type == TOKEN_EQUALS) {
        parserEat(parser, TOKEN_EQUALS);
        ast->value = parseExpression(parser);
    }

    return ast;
}

// This is usable for function parameters
//! DO NOT use this for parsing for loops
AST* parseFunctionParameters(Parser* parser) {

    parserEat(parser, TOKEN_LPAREN);

    AST* ast = createAST(AST_COMPOUND);

    if (parser->token->type != TOKEN_RPAREN) {
        vectorPush(ast->children, parseSingleParameter(parser));
    
        while (parser->token->type == TOKEN_COMMA) {
            parserEat(parser, TOKEN_COMMA);
            vectorPush(ast->children, parseSingleParameter(parser));
        }
    }

    parserEat(parser, TOKEN_RPAREN);

    return ast;
}

AST* parseBlock(Parser* parser) {

    parserEat(parser, TOKEN_LCURLY);
    AST* ast = createAST(AST_COMPOUND);

    while (parser->token->type != TOKEN_RCURLY) {
        vectorPush(ast->children, parseExpression(parser));
    }

    parserEat(parser, TOKEN_RCURLY);

    return ast;
}

AST* parseFunction(Parser* parser) {

    parserEat(parser, TOKEN_FN);

    int return_type = tokenToDataType(parser->token);
    parserEat(parser, TOKEN_DATATYPE);

    char* function_name = malloc(parser->token->length + 1);
    assert(function_name && "Could not allocate memory for function_name string");
    strcpy(function_name, parser->token->value);

    parserEat(parser, TOKEN_ID);

    AST* ast = createAST(AST_FUNCTION_DECLARATION);
    ast->name = function_name;
    ast->data_type = return_type;
    ast->args_params = parseFunctionParameters(parser); // Should parse all the arguments and be on the curly braces or semi

    if (parser->token->type == TOKEN_LCURLY) {
        ast->node_type = AST_FUNCTION_DEFINITION;
        ast->value = parseBlock(parser);
        return ast;
    }

    parserEat(parser, TOKEN_RPAREN);

    return ast;
}

AST* parseReturn(Parser* parser) {

    AST* ast = createAST(AST_RETURN);
    parserEat(parser, TOKEN_RETURN);

    if (parser->token->type != TOKEN_SEMI) {
        ast->value = parseExpression(parser);
    }

    else {
        parserEat(parser, TOKEN_SEMI);
        ast->value = createAST(AST_NOOP);
        ast->value->data_type = TYPE_VOID;
    }

    return ast;
}

AST* parseExpression(Parser* parser) {

    assert(TOKEN_MAX == 29 && "parseExpression() must handle all token cases");

    switch (parser->token->type) {

        case TOKEN_ID               : return parseId(parser);
        case TOKEN_INT_LITERAL      : return parseIntLiteral(parser);
        case TOKEN_DATATYPE         : return parseDataType(parser);
        case TOKEN_LPAREN           : return parseParenthesis(parser);
        case TOKEN_FN               : return parseFunction(parser);
        case TOKEN_RETURN           : return parseReturn(parser);
        default:
        printf("[Parser]: Unexpected token: '%s' at %d:%d\n", parser->token->value, parser->token->line, parser->token->column);
        exit(1);
    }

}

AST* parseProgram(Parser* parser) {

    AST* compound = createAST(AST_COMPOUND);

    while (parser->token->type != TOKEN_EOF) {
        vectorPush(compound->children, parseExpression(parser));
    }

    return compound;
}