#ifndef EON_TOKEN_H
#define EON_TOKEN_H

enum _token_types{
    TOKEN_ID,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_STRING_LITERAL,
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
    TOKEN_DATATYPE,
    // KEYWORDS
    TOKEN_FN,
    TOKEN_RETURN,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_CONST,
    TOKEN_ELSE,
    // For control
    TOKEN_EOF,
    TOKEN_MAX,
};

typedef struct _token {
    char* value;
    enum _token_types type;
    int length;
    unsigned int line;
    unsigned int column;
} Token;

Token* createToken(char* value, int type, int length, int line, int column);

int checkKeyword(Token* token);

char* tokenIdToString(int token_id);

void printToken(Token* token);

#endif