#include "include/lexer.h"
#include "include/macros.h"
#include "include/types.h"
#include "include/arena.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern Arena* arena_main;

Lexer* createLexer(char* src) {

    Lexer* lexer = (Lexer*) arenaPush(arena_main, sizeof(lexer), true);
    assert(lexer && "Could not allocate memory for lexer");

    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[0];
    lexer->line = 1;
    lexer->column = 1;

    return lexer;
}

void lexerAdvance(Lexer* lexer) {

    if (lexer->i < lexer->src_size && lexer->c != '\0') {
        lexer->i++;
        lexer->column++;
        lexer->c = lexer->src[lexer->i];
    }

    return;
}

char lexerPeek(Lexer* lexer, int offset) {
    return lexer->src[MIN(lexer->i + offset, lexer->src_size)];
}

void lexerSkipWhitespace(Lexer* lexer) {

    while (lexer->c == '\r' || lexer->c == '\n' || lexer->c == ' ' || lexer->c == '\t') {
        if (lexer->c == '\n') {
            lexer->line++;
            lexer->column = 0;
        }
        lexerAdvance(lexer);
    }

    return;
}

Token* lexerAdvanceWith(Lexer* lexer, Token* token, int offset) {

    for (int i = 0; i < offset; i++) {
        lexerAdvance(lexer);
    }

    return token;
}

Token* lexerAdvanceCurrent(Lexer* lexer, int type) {

    char* value = arenaPushString(arena_main, 2, " ");

    value[0] = lexer->c;

    Token* token = createToken(value, type, 1, lexer->line, lexer->column);
    lexerAdvance(lexer);

    return token;
}

// TODO: make this better
Token* lexerParseId(Lexer* lexer) {

    int beg = lexer->i;

    while (isalnum(lexer->c)) {
        lexerAdvance(lexer);
    }

    int end = lexer->i;

    int length_excluding_null = end - beg;

    char* value = (char*) arenaPush(arena_main, length_excluding_null + 1, false);

    memcpy(value, lexer->src + beg, length_excluding_null);

    value[length_excluding_null] = '\0';

    /* char* value = calloc(1, 1);
    int length = 0;

    while (isalnum(lexer->c)) {
        value = realloc(value, strlen(value) + 2);
        strcat(value, (char[]) {lexer->c, 0});
        length++;
        lexerAdvance(lexer);
    } */

    return createToken(value, TOKEN_ID, length_excluding_null, lexer->line, lexer->column - length_excluding_null);
}

// TODO: add float compat
Token* lexerParseNumber(Lexer* lexer) {

    int beg = lexer->i;

    while (isdigit(lexer->c)) {
        lexerAdvance(lexer);
    }

    int end = lexer->i;

    int length_excluding_null = end - beg;

    char* value = (char*) arenaPush(arena_main, length_excluding_null + 1, false);

    memcpy(value, lexer->src + beg, length_excluding_null);

    value[length_excluding_null] = '\0';

    /* char* value = calloc(1, 1);
    int length = 0;

    while (isdigit(lexer->c)) {
        value = realloc(value, strlen(value) + 2);
        strcat(value, (char[]) {lexer->c, 0});
        length++;
        lexerAdvance(lexer);
    } */

    return createToken(value, TOKEN_INT_LITERAL, length_excluding_null, lexer->line, lexer->column - length_excluding_null);
}

// TODO: handle string and char literals
Token* lexerNextToken(Lexer* lexer) {

    assert(TOKEN_MAX == 29 && "lexerNextToken() must account for all token types");

    while (lexer->c != '\0') {

        if (lexer->c == '\r' || lexer->c == '\n' || lexer->c == ' ' || lexer->c == '\t') lexerSkipWhitespace(lexer);

        if (isalpha(lexer->c)) {
            Token* id = lexerParseId(lexer);
            int type = tokenToDataType(id);
            if (type != -1) id->type = TOKEN_DATATYPE;

            type = checkKeyword(id);
            if (type != -1) id->type = type;

            return id;

        }

        if (isdigit(lexer->c)) return lexerParseNumber(lexer);

        switch (lexer->c) {

            case '=':
            if (lexerPeek(lexer, 1) == '=') return lexerAdvanceWith(lexer, createToken("==", TOKEN_TWOEQUALS, 2, lexer->line, lexer->column - 2), 2);
            return lexerAdvanceCurrent(lexer, TOKEN_EQUALS);

            case '(': return lexerAdvanceCurrent(lexer, TOKEN_LPAREN);
            
            case ')': return lexerAdvanceCurrent(lexer, TOKEN_RPAREN);
            
            case '{': return lexerAdvanceCurrent(lexer, TOKEN_LCURLY);
            
            case '}': return lexerAdvanceCurrent(lexer, TOKEN_RCURLY);
            
            case '[': return lexerAdvanceCurrent(lexer, TOKEN_LSQUARE);
            
            case ']': return lexerAdvanceCurrent(lexer, TOKEN_RSQUARE);
            
            case ':': return lexerAdvanceCurrent(lexer, TOKEN_COLON);
            
            case ';': return lexerAdvanceCurrent(lexer, TOKEN_SEMI);
            
            case ',': return lexerAdvanceCurrent(lexer, TOKEN_COMMA);
            
            case '<': return lexerAdvanceCurrent(lexer, TOKEN_LESSTHAN);
            
            case '>': return lexerAdvanceCurrent(lexer, TOKEN_GREATERTHAN);

            case '\0': break;

            default:
                printf("[Lexer] Unexpected character '%c'\n", lexer->c); exit(1);
            break;
        }

    }

    return createToken(0, TOKEN_EOF, 1, lexer->line, lexer->column);
}