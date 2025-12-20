#ifndef EON_LEXER_H
#define EON_LEXER_H
#include "token.h"
#include <stdio.h>

typedef struct _lexer {
    char* src;
    size_t src_size;
    char c;
    unsigned int i;
    unsigned int line;
    unsigned int column;
} Lexer;

Lexer* createLexer(char* src);

void lexerAdvance(Lexer* lexer);

char lexerPeek(Lexer* lexer, int offset);

void lexerSkipWhitespace(Lexer* lexer);

Token* lexerAdvanceWith(Lexer* lexer, Token* token, int offset);

Token* lexerAdvanceCurrent(Lexer* lexer, int type);

Token* lexerParseId(Lexer* lexer);

Token* lexerParseNumber(Lexer* lexer);

Token* lexerNextToken(Lexer* lexer);

#endif