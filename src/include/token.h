#ifndef EON_TOKEN_H
#define EON_TOKEN_H

enum _token_type {

    // Identifiers
    TOKEN_ID,
    TOKEN_DATATYPE,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_STRING_LITERAL,

    // Syntax (misc)
    TOKEN_EQUALS,
    TOKEN_TWOEQUALS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LCURLY,
    TOKEN_RCURLY,
    TOKEN_LSQUARE,
    TOKEN_RSQUARE,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_SEMI,
    TOKEN_LESSTHAN,
    TOKEN_GREATERTHAN,

    // Arithmetic
    TOKEN_ADDITION,
    TOKEN_SUBTRACTION,
    TOKEN_DIVISION,
    TOKEN_MULTIPLICATION,

    // Keywords
    TOKEN_FN,
    TOKEN_IF,
    TOKEN_FOR,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_BREAK,
    TOKEN_CONST,
    TOKEN_RETURN,
    TOKEN_CONTINUE,

    // For control
    TOKEN_EOF,
    TOKEN_MAX,
};

typedef struct _token {
    char* value;
    int length;
    unsigned int line;
    unsigned int column;
    enum _token_type type;
} Token;

Token* createToken(char* value, int length, unsigned int line, unsigned int column, int type);

int checkKeyword(Token* token);

char* tokenTypeToString(int token_type);

void printToken(Token* token);

#endif