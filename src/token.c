#include "include/token.h"
#include "include/error_codes.h"
#include "include/arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern Arena* arena_main;

Token* createToken(char* value, int length, unsigned int line, unsigned int column, int type) {

    Token* token = (Token*) arenaPush(arena_main, sizeof(Token), false);

    token->value = value; // string is pre-allocated on the arena in the lexing phase
    token->length = length;
    token->line = line;
    token->type = type;

    return token;
}

int checkKeyword(Token* token) {

    assert(TOKEN_MAX == 29 && "checkKeyword() must handle all keywords");

    switch (token->length) {

        case 2:
            if (!strcmp(token->value, "fn")) return TOKEN_FN;
            if (!strcmp(token->value, "if")) return TOKEN_IF;
            break;

        case 3:
            if (!strcmp(token->value, "for")) return TOKEN_FOR;
            break;

        case 4:
            if (!strcmp(token->value, "else")) return TOKEN_ELSE;
            break;

        case 5:
            if (!strcmp(token->value, "while")) return TOKEN_WHILE;
            if (!strcmp(token->value, "break")) return TOKEN_BREAK;
            if (!strcmp(token->value, "const")) return TOKEN_CONST;
            break;

        case 6:
            if (!strcmp(token->value, "return")) return TOKEN_RETURN;
            break;

        case 8:
            if (!strcmp(token->value, "continue")) return TOKEN_CONTINUE;
            break;

        default:
            break;

    }

    return -1;
}

char* tokenTypeToString(int token_type) {

    assert(TOKEN_MAX == 29 && "tokenTypeToString() must handle all token types");

    switch (token_type) {

        case TOKEN_ID               : return "TOKEN_ID";
        case TOKEN_INT_LITERAL      : return "TOKEN_INT_LITERAL";
        case TOKEN_FLOAT_LITERAL    : return "TOKEN_FLOAT_LITERAL";
        case TOKEN_CHAR_LITERAL     : return "TOKEN_CHAR_LITERAL";
        case TOKEN_STRING_LITERAL   : return "TOKEN_STRING_LITERAL";
        case TOKEN_EQUALS           : return "TOKEN_EQUALS";
        case TOKEN_TWOEQUALS        : return "TOKEN_TWOEQUALS";
        case TOKEN_LPAREN           : return "TOKEN_LPAREN";
        case TOKEN_RPAREN           : return "TOKEN_RPAREN";
        case TOKEN_LCURLY           : return "TOKEN_LCURLY";
        case TOKEN_RCURLY           : return "TOKEN_RCURLY";
        case TOKEN_LSQUARE          : return "TOKEN_LSQUARE";
        case TOKEN_RSQUARE          : return "TOKEN_RSQUARE";
        case TOKEN_COLON            : return "TOKEN_COLON";
        case TOKEN_COMMA            : return "TOKEN_COMMA";
        case TOKEN_SEMI             : return "TOKEN_SEMI";
        case TOKEN_LESSTHAN         : return "TOKEN_LESSTHAN";
        case TOKEN_GREATERTHAN      : return "TOKEN_GREATERTHAN";
        case TOKEN_DATATYPE         : return "TOKEN_DATATYPE";
        case TOKEN_FN               : return "TOKEN_FN";
        case TOKEN_IF               : return "TOKEN_IF";
        case TOKEN_FOR              : return "TOKEN_FOR";
        case TOKEN_ELSE             : return "TOKEN_ELSE";
        case TOKEN_WHILE            : return "TOKEN_WHILE";
        case TOKEN_BREAK            : return "TOKEN_BREAK";
        case TOKEN_CONST            : return "TOKEN_CONST";
        case TOKEN_RETURN           : return "TOKEN_RETURN";
        case TOKEN_CONTINUE         : return "TOKEN_CONTINUE";
        case TOKEN_EOF              : return "TOKEN_EOF";
        case TOKEN_MAX              : break;

    }

    printf("Token type unrecognized\n");
    exit(ERROR_TOKEN_TYPE_UNRECONIZED);

    return "ERROR";
}

void printToken(Token* token) {

    printf("[Token]: [%s] [%d] [%d] [%d] [%s]\n", token->value, token->length, token->line, token->column, tokenTypeToString(token->type));

    return;
}