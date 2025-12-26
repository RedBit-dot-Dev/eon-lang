#include "include/lexer.h"
#include "include/arena.h"
#include "include/data_types.h"
#include "include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

extern Arena* arena_main;

Lexer* createLexer(char* src, unsigned int src_length) {

    Lexer* lexer = (Lexer*) arenaPush(arena_main, sizeof(Lexer), false);

    lexer->src = src;
    lexer->src_length = src_length;
    lexer->c = src[0];
    lexer->i = 0;
    lexer->line = 1;
    lexer->column = 1;

    return lexer;
}

Token* lexerNextToken(Lexer* lexer) {

    assert(TOKEN_MAX == 29 && "lexerNextToken() must account for all token types");

    while (lexer->c != '\0') {

        if (lexer->c == '\r' || lexer->c == '\n' || lexer->c == ' ' || lexer->c == '\t') skipWhitespace(lexer);

        if (isalpha(lexer->c) || lexer->c == '_') return handleId(lexer);

        if (isdigit(lexer->c)) return handleNumber(lexer);

        switch (lexer->c) {

            case '=': return advanceAndReturn(lexer, TOKEN_EQUALS,      1);
            case '(': return advanceAndReturn(lexer, TOKEN_LPAREN,      1);
            case ')': return advanceAndReturn(lexer, TOKEN_RPAREN,      1);
            case '{': return advanceAndReturn(lexer, TOKEN_LCURLY,      1);
            case '}': return advanceAndReturn(lexer, TOKEN_RCURLY,      1);
            case '[': return advanceAndReturn(lexer, TOKEN_LSQUARE,     1);
            case ']': return advanceAndReturn(lexer, TOKEN_RSQUARE,     1);
            case ':': return advanceAndReturn(lexer, TOKEN_COLON,       1);
            case ',': return advanceAndReturn(lexer, TOKEN_COMMA,       1);
            case ';': return advanceAndReturn(lexer, TOKEN_SEMI,        1);
            case '<': return advanceAndReturn(lexer, TOKEN_LESSTHAN,    1);
            case '>': return advanceAndReturn(lexer, TOKEN_GREATERTHAN, 1);

            default:
                printf("[Lexer] Unexpected character '%c' at %d:%d\n", lexer->c, lexer->line, lexer->column); exit(ERROR_UNEXPECTED_CHARACTER);
                break;
        }

    }

    return createToken(0, 0, lexer->line, lexer->column, TOKEN_EOF);
}

static void advance(Lexer* lexer) {

    if (lexer->i < lexer->src_length && lexer->c != '\0') {
        lexer->i++;
        lexer->column++;
        lexer->c = lexer->src[lexer->i];
    }

    return;
}

static Token* advanceAndReturn(Lexer* lexer, int token_type, int advances) {

    char* string = arenaPush(arena_main, 2, false); 

    string[0] = lexer->c;
    string[1] = '\0';

    Token* token = createToken(string, 1, lexer->line, lexer->column, token_type);

    for (int i = 0; i < advances; i++) {
        advance(lexer);
    }

    return token;
}

static void skipWhitespace(Lexer* lexer) {

    while (lexer->c == '\r' || lexer->c == '\n' || lexer->c == ' ' || lexer->c == '\t') {
        if (lexer->c == '\n') {
            lexer->line++;
            lexer->column = 0;
        }
        advance(lexer);
    }

    return;
}

static Token* handleId(Lexer* lexer) {

    unsigned int beg = lexer->i;

    while (isalnum(lexer->c) || lexer->c == '_') {
        advance(lexer);
    }

    unsigned int end = lexer->i;

    unsigned int length = end - beg;

    char* id = arenaPush(arena_main, length + 1, false);

    memcpy(id, lexer->src + beg, length);

    id[length] = '\0';

    Token* token = createToken(id, length, lexer->line, lexer->column, TOKEN_ID);

    int type = checkKeyword(token);

    if (type != -1) {
        token->type = type;
        return token;
    }

    type = checkDataType(token);

    if (type != -1) {
        token->type = TOKEN_DATATYPE;
        return token;
    }

    return token;
}

static Token* handleNumber(Lexer* lexer) {

    bool found_point = false;

    unsigned int beg = lexer->i;

    while (isdigit(lexer->c) || lexer->c == '.') {

        if (lexer->c == '.' && found_point) {
            printf("[Lexer] Unexpected character '%c' at %d:%d\n", lexer->c, lexer->line, lexer->column);
            exit(ERROR_UNEXPECTED_CHARACTER);
        }

        else if (lexer->c == '.') {
            found_point = true;
        }

        advance(lexer);

    }

    unsigned int end = lexer->i;

    unsigned int length = end - beg;

    char* number = arenaPush(arena_main, length + 1, false);

    memcpy(number, lexer->src + beg, length);

    number[length] = '\0';

    if (found_point) return createToken(number, length, lexer->line, lexer->column, TOKEN_FLOAT_LITERAL);

    return createToken(number, length, lexer->line, lexer->column, TOKEN_INT_LITERAL);
}