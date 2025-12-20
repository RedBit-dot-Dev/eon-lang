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
    TOKEN_FN, //x
    TOKEN_RETURN, //x
    TOKEN_FOR, //x
    TOKEN_IF, //x
    TOKEN_WHILE, //x
    TOKEN_BREAK, //x
    TOKEN_CONTINUE,
    TOKEN_CONST, //x
    TOKEN_ELSE, //x
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