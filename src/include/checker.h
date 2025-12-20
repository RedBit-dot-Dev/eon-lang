#ifndef EON_CHECKER_H
#define EON_CHECKER_H
#include "AST.h"

void checkFunctionDefinition(AST* ast);

void checkExpression(AST* ast);

void checkProgram(AST* root);

#endif