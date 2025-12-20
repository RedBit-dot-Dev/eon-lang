#include "include/eon.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/ir.h"

void eonCompile(char* src) {

    Lexer* lexer = createLexer(src);
    Parser* parser = createParser(lexer);
    AST* root = parseProgram(parser);

    buildProgram(root);

    //printf("%ld\n", root->children->item_byte_size);

    /* Token* token = 0;

    int token_counter = 0;
    while ((token = lexerNextToken(lexer))->type != TOKEN_EOF) {
        printf("%d ", ++token_counter);
        printToken(token);
        if (token->type == TOKEN_EOF) break;
    } */

    return;
}