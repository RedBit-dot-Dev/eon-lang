#ifndef EON_PARSER_H
#define EON_PARSER_H
#include "AST.h"
#include "lexer.h"

typedef struct _parser {
    Lexer* lexer;
    Token* token;
} Parser;

Parser* createParser(Lexer* lexer);

void parserEat(Parser* parser, int token_type);

void parseParameters(Parser* parser, AST* parent);

void parseFunctionBody(Parser* parser, AST* parent);

AST* parseIntLiteral(Parser* parser);

AST* parseFloatLiteral(Parser* parser);

AST* parseCharLiteral(Parser* parser);

AST* parseStringLiteral(Parser* parser);

AST* parseVar(Parser* parser);

AST* parseParenthesis(Parser* parser);

AST* parseDataType(Parser* parser);

AST* parseVarAssignment(Parser* parser);

AST* parseReturn(Parser* parser);

AST* parseRHS(Parser* parser);

AST* parseIf(Parser* parser);

AST* parseWhile(Parser* parser);

AST* parseFor(Parser* parser);

AST* parseLHS(Parser* parser);

AST* parseFunction(Parser* parser);

AST* parseGlobalScope(Parser* parser);

AST* parseProgram(Parser* parser);

#endif