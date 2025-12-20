#include "include/token.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

Token* createToken(char* value, int type, int length, int line, int column) {

    Token* token = (Token*) malloc(sizeof(Token));
    assert(token && "Could not allocate memory for token");

    token->value = value;
    token->type = type;
    token->length = length;
    token->line = line;
    token->column = column;

    return token;
}

int checkKeyword(Token* token) {

    assert(TOKEN_MAX == 29 && "checkKeyword() must handle all token types");

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

char* tokenIdToString(int token_id) {

    assert(TOKEN_MAX == 29 && "tokenIdToString() must handle all token types");

    switch (token_id) {

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
        case TOKEN_RETURN           : return "TOKEN_RETURN";
        case TOKEN_FOR              : return "TOKEN_FOR";
        case TOKEN_IF               : return "TOKEN_IF";
        case TOKEN_WHILE            : return "TOKEN_WHILE";
        case TOKEN_BREAK            : return "TOKEN_BREAK";
        case TOKEN_CONTINUE         : return "TOKEN_CONTINUE";
        case TOKEN_CONST            : return "TOKEN_CONST";
        case TOKEN_ELSE             : return "TOKEN_ELSE";
        case TOKEN_EOF              : return "TOKEN_EOF"; // Just so that the compiler doesn't scream at me
        case TOKEN_MAX              : return "TOKEN_MAX"; // Just so that the compiler doesn't scream at me

        default:
            break;
    }

    return "INVALID_TOKEN";
}

void printToken(Token* token) {

    char* type_string = tokenIdToString(token->type);

    printf("Token: [%s] -> [%s]\n", token->value, type_string);

    return;
}