#ifndef EON_PARSER_H
#define EON_PARSER_H
#include "lexer.h"
#include "AST.h"

typedef struct _parser {
    Lexer* lexer;
    Token* token;
} Parser;

Parser* createParser(Lexer* lexer);

Token* parserEat(Parser* parser, int type);

AST* parseId(Parser* parser);

AST* parseIntLiteral(Parser* parser);

AST* parseDataType(Parser* parser);

AST* parseParenthesis(Parser* parser);

AST* parseSingleParameter(Parser* parser);

AST* parseFunctionParameters(Parser* parser);

AST* parseBlock(Parser* parser);

AST* parseFunction(Parser* parser);

AST* parseReturn(Parser* parser);

AST* parseExpression(Parser* parser);

AST* parseProgram(Parser* parser);

#endif