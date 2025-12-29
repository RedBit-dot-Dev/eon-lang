#ifndef EON_IR_GEN_H
#define EON_IR_GEN_H
#include "AST.h"

int getAlignSize(int data_type);

void writeVarDeclaration(AST* ast, FILE* file);

void writeVarDefinition(AST* ast, FILE* file);

void writeReturn(AST* ast, FILE* file);

void writeExpression(AST* ast, FILE* file, int tab_count);

void writeParameters(AST* ast, FILE* file);

void writeFunctionDeclaration(AST* ast, FILE* file);

void writeFunctionDefinition(AST* ast, FILE* file);

void writeGlobalVarDeclaration(AST* ast, FILE* file);

void writeGlobalVarDefinition(AST* ast, FILE* file);

void writeProgram(AST* root, FILE* file);

#endif