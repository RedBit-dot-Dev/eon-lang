#ifndef EON_LEXER_H
#define EON_LEXER_H
#include "token.h"

typedef struct _lexer {
    char c;
    char* src;
    unsigned int i;
    unsigned int line;
    unsigned int column;
    unsigned int src_length;
} Lexer;

Lexer* createLexer(char* src, unsigned int src_length);

Token* lexerNextToken(Lexer* lexer);

static void advance(Lexer* lexer);

static Token* advanceAndReturn(Lexer* lexer, int token_type, int advances);

static void skipWhitespace(Lexer* lexer);

static Token* handleId(Lexer* lexer);

static Token* handleNumber(Lexer* lexer);

#endif