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

void advance(Lexer* lexer);

Token* advanceAndReturn(Lexer* lexer, int token_type, int advances);

void skipWhitespace(Lexer* lexer);

char transcribeChar(char str);

Token* handleChar(Lexer* lexer);

Token* handleId(Lexer* lexer);

Token* handleNumber(Lexer* lexer);

#endif