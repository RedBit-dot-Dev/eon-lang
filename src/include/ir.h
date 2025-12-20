#ifndef EON_IR_H
#define EON_IR_H
#include "AST.h"
#include "token.h"
#include <stdio.h>

void writeFunctionDefinition(FILE* file, AST* ast);

void writeExpression(FILE* file, AST* ast, int num_tabs);

void writeProgram(FILE* file, AST* root);

void buildProgram(AST* root);

#endif