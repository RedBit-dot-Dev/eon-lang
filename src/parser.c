#include "include/parser.h"
#include "include/arena.h"
#include "include/data_types.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern Arena* arena_main;

Parser* createParser(Lexer* lexer) {

    Parser* parser = (Parser*) arenaPush(arena_main, sizeof(Parser), false);

    parser->lexer = lexer;
    parser->token = lexerNextToken(parser->lexer);

    return parser;
}

void parserEat(Parser* parser, int token_type) {

    Token* token = parser->token; // just to shrink printf call

    if (token->type != token_type) {

        printf("[Parser]: Unexpected token: '%s' '%s' at %d:%d\n, was expecting '%s'", 
            token->value, 
            tokenTypeToString(token->type), 
            token->line, token->column, 
            tokenTypeToString(token_type) // TODO: change this to make error message more comprehensible
        );

        exit(ERROR_UNEXPECTED_TOKEN);
    }

    parser->token = lexerNextToken(parser->lexer);

    return;
}

void parseParameters(Parser* parser, AST* parent) {

    parserEat(parser, TOKEN_LPAREN);

    if (parser->token->type == TOKEN_RPAREN) {
        parserEat(parser, TOKEN_RPAREN);
        return;
    }

    parent->args_params = createVector(sizeof(AST*));

    //parsing the first parameter
    AST* parem = createAST(AST_PARAMETER);

    parem->data_type = checkDataType(parser->token);
    parserEat(parser, TOKEN_DATATYPE);

    parem->name = parser->token->value;
    parserEat(parser, TOKEN_ID);

    vectorPush(parent->args_params, parem);

    // parsing the rest
    while (parser->token->type != TOKEN_RPAREN) {

        parserEat(parser, TOKEN_COMMA);

        parem = createAST(AST_PARAMETER);

        parem->data_type = checkDataType(parser->token);
        parserEat(parser, TOKEN_DATATYPE);

        parem->name = parser->token->value;
        parserEat(parser, TOKEN_ID);

        vectorPush(parent->args_params, parem);

    }

    parserEat(parser, TOKEN_RPAREN);

    return;
}

AST* parseIntLiteral(Parser* parser) {

    AST* ast = createAST(AST_INT_LITERAL);

    ast->literal_value = parser->token->value;
    ast->name = parser->token->value;
    ast->line = parser->lexer->line;
    ast->data_type = TYPE_i32; // Default int value

    parserEat(parser, TOKEN_INT_LITERAL);

    return ast;
}

AST* parseFloatLiteral(Parser* parser) {

    AST* ast = createAST(AST_FLOAT_LITERAL);

    ast->literal_value = parser->token->value;
    ast->name = parser->token->value;
    ast->line = parser->lexer->line;
    ast->data_type = TYPE_FLOAT; // Default float type

    parserEat(parser, TOKEN_FLOAT_LITERAL);

    return ast;
}

AST* parseCharLiteral(Parser* parser) {

    AST* ast = createAST(AST_CHAR_LITERAL);

    ast->literal_value = parser->token->value;
    ast->name = parser->token->value;
    ast->line = parser->lexer->line;
    ast->data_type = TYPE_i8; // Default char type

    parserEat(parser, TOKEN_CHAR_LITERAL);

    return ast;
}

AST* parseStringLiteral(Parser* parser) {

    AST* ast = createAST(AST_STRING_LITERAL);

    ast->literal_value = parser->token->value;
    ast->line = parser->lexer->line;
    ast->data_type = TYPE_i64; // Default string type // TODO: check this

    parserEat(parser, TOKEN_STRING_LITERAL);
    parserEat(parser, TOKEN_SEMI);

    return ast;
}

AST* parseVar(Parser* parser) {

    AST* ast = createAST(AST_VARIABLE);

    ast->name = parser->token->value;
    ast->line = parser->token->line;

    parserEat(parser, TOKEN_ID);

    return ast;
}

// TODO: make this
AST* parseParenthesis(Parser* parser) {
    return NULL;
}

AST* parseBinaryOp(Parser* parser, int op_type) {

    AST* ast = createAST(AST_BIOP);

    ast->name = parser->token->value;
    ast->line = parser->token->line;

    if (op_type == TOKEN_ADDITION || op_type == TOKEN_SUBTRACTION) {
        ast->precedence_value = 1;
    }

    else if (op_type == TOKEN_DIVISION || op_type == TOKEN_MULTIPLICATION) {
        ast->precedence_value = 2;
    }

    else {
        printf("ERROR_PLACEHOLDER\n");
        exit(ERROR_PLACEHOLDER);
        return NULL;
    }

    parserEat(parser, op_type);

    return ast;
}

AST* parseTerm(Parser* parser) {

    /*
        Precedence Value cheat sheet: (Higher is delt with first)
        + = 1
        - = 1
        / = 2
        * = 2
    */

    assert(TOKEN_MAX == 33 && "parseRHS() must handle all token types");

    AST* term_left;

    switch (parser->token->type) {

        //case TOKEN_STRING_LITERAL   : return parseStringLiteral(parser);

        case TOKEN_INT_LITERAL      : term_left = parseIntLiteral(parser);    break;
        case TOKEN_FLOAT_LITERAL    : term_left = parseFloatLiteral(parser);  break;
        case TOKEN_CHAR_LITERAL     : term_left = parseCharLiteral(parser);   break;
        case TOKEN_ID               : term_left = parseVar(parser);           break;

        case TOKEN_LPAREN           : return parseParenthesis(parser);

        default:
            // TODO: make this error message better
            printf("[Parser]: Expression at %d:%d not supported in the RHS\n", parser->token->line, parser->token->column);
            exit(ERROR_EXPRESSION_NOT_SUPPORTED);
            return NULL;
    }

    AST* operator;

    switch (parser->token->type) {

        case TOKEN_SEMI:
            parserEat(parser, TOKEN_SEMI);
            return term_left;

        case TOKEN_ADDITION         : operator = parseBinaryOp(parser, TOKEN_ADDITION);         break;
        case TOKEN_SUBTRACTION      : operator = parseBinaryOp(parser, TOKEN_SUBTRACTION);      break;
        case TOKEN_DIVISION         : operator = parseBinaryOp(parser, TOKEN_DIVISION);         break;
        case TOKEN_MULTIPLICATION   : operator = parseBinaryOp(parser, TOKEN_MULTIPLICATION);   break;

        default:
            printf("[Parser]: Expected ';' but recieved %s at %d:%d\n",
                tokenTypeToString(parser->token->type),
                parser->token->line,
                parser->token->column
            );
            exit(ERROR_EXPRESSION_NOT_SUPPORTED);
            return NULL;
    }

    AST* term_right = parseTerm(parser);

    operator->left = term_left;
    operator->right = term_right;

    if (operator->precedence_value >= term_right->precedence_value) {

        AST* lesser_op = term_right;
        operator->right = lesser_op->left;
        lesser_op->left = operator;

        return lesser_op;
    }

    return operator;
}

// TODO: Finish this
AST* parseRHS(Parser* parser) {

    switch (parser->token->type) {

        case TOKEN_INT_LITERAL      : break;
        case TOKEN_FLOAT_LITERAL    : break;
        case TOKEN_CHAR_LITERAL     : break;
        case TOKEN_ID               : break;

        default: 
            printf("ERROR_PLACEHOLDER\n");
            exit(ERROR_PLACEHOLDER);
            return NULL;
    }

    return parseTerm(parser);
}

AST* parseDataType(Parser* parser) {

    int data_type = checkDataType(parser->token);
    parserEat(parser, TOKEN_DATATYPE);

    char* var_name = parser->token->value;
    parserEat(parser, TOKEN_ID);

    AST* ast = createAST(AST_VAR_DECLARATION);

    ast->data_type = data_type;
    ast->name = var_name;
    ast->line = parser->lexer->line;

    if (parser->token->type == TOKEN_SEMI) {
        parserEat(parser, TOKEN_SEMI);
        return ast;
    }

    ast->ast_type = AST_VAR_DEFINITION;
    parserEat(parser, TOKEN_EQUALS);

    ast->ast_value = parseRHS(parser);

    return ast;
}

AST* parseVarAssignment(Parser* parser) {

    char* var_name = parser->token->value;
    parserEat(parser, TOKEN_ID);

    parserEat(parser, TOKEN_EQUALS);

    AST* ast = createAST(AST_ASSIGNMENT);
    ast->name = var_name;
    ast->line = parser->lexer->line;
    ast->ast_value = parseRHS(parser);

    return ast;
}

AST* parseReturn(Parser* parser) {

    AST* ast = createAST(AST_RETURN);

    parserEat(parser, TOKEN_RETURN);

    ast->line = parser->lexer->line;

    if (parser->token->type == TOKEN_SEMI) {
        ast->ast_value = createAST(AST_NOOP);
        ast->ast_value->data_type = TYPE_VOID;
        parserEat(parser, TOKEN_SEMI);
        return ast;
    }

    ast->ast_value = parseRHS(parser);

    return ast;
}

AST* parseLHS(Parser* parser) {

    assert(TOKEN_MAX == 33 && "parseLHS() must handle all token types");

    switch (parser->token->type) {

        case TOKEN_DATATYPE :   return parseDataType(parser);
        case TOKEN_ID       :   return parseVarAssignment(parser);
        case TOKEN_RETURN   :   return parseReturn(parser);

        default:
            break;

    }

    printf("[Parser]: Expression at %d:%d not supported in the LHS\n", parser->token->line, parser->token->column);
    exit(ERROR_EXPRESSION_NOT_SUPPORTED);

    return NULL;
}

void parseFunctionBody(Parser* parser, AST* parent) {

    parserEat(parser, TOKEN_LCURLY);

    if (parser->token->type == TOKEN_RCURLY) {
        parserEat(parser, TOKEN_RCURLY);
        return;
    }

    parent->children = createVector(sizeof(AST*));

    while (parser->token->type != TOKEN_RCURLY) {
        vectorPush(parent->children, parseLHS(parser));
    }

    parserEat(parser, TOKEN_RCURLY);

    return;
}

// This is only called on the main scope
AST* parseFunction(Parser* parser) {

    parserEat(parser, TOKEN_FN);

    int data_type = checkDataType(parser->token);
    parserEat(parser, TOKEN_DATATYPE);

    char* function_name = parser->token->value;
    parserEat(parser, TOKEN_ID);

    AST* ast = createAST(AST_FUNCTION_DECLARATION);

    ast->data_type = data_type;
    ast->name = function_name;
    ast->line = parser->lexer->line;

    parseParameters(parser, ast);

    if (parser->token->type == TOKEN_SEMI) {
        parserEat(parser, TOKEN_SEMI);
        return ast;
    }

    ast->ast_type = AST_FUNCTION_DEFINITION;
    parseFunctionBody(parser, ast);

    return ast;
}

// This is only called on the main scope
AST* parseGlobalScope(Parser* parser) {

    assert(AST_MAX == 17 && "parseGlobalScope() must handle all AST types");

    switch (parser->token->type) {

        case TOKEN_FN: return parseFunction(parser); 
        case TOKEN_DATATYPE: return parseDataType(parser);
        case TOKEN_ID: return parseVarAssignment(parser);

        default:
            break;
    }

    printf("[Parser]: Expression at %d:%d not supported in the main scope\n", parser->token->line, parser->token->column);
    exit(ERROR_EXPRESSION_NOT_SUPPORTED);

    return NULL;
}

// This is only called on the root AST node
AST* parseProgram(Parser* parser) {

    AST* root = createAST(AST_COMPOUND);
    root->children = createVector(sizeof(AST*));

    while (parser->token->type != TOKEN_EOF) {
        vectorPush(root->children, parseGlobalScope(parser));
    }

    return root;
}