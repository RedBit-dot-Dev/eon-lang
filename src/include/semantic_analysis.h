#ifndef EON_SEMANTIC_ANALYSIS_H
#define EON_SEMANTIC_ANALYSIS_H
#include "AST.h"
#include "symbol_table.h"

//void analyseReturn(AST* ast, int return_type);

//void analyseVarDeclaration(AST* ast, Symbol_Table* scope);

//void analyseVarDefinition(AST* ast, Symbol_Table* scope);

//void analyseAssignment(AST* ast, Symbol_Table* scope);

//void analyseFunctionDefinition(AST* ast);

//void analyseExpression(AST* ast);

void analyseExpression(AST* ast);

void analyseReturn(AST* ast, AST* function_ast);

void analyseFunctionDeclaration(AST* ast);

void analyseFunctionDefinition(AST* ast);

void analyseVarDefinition(AST* ast);

void analyseVarDeclaration(AST* ast);

void analyseProgram(AST* root);

#endif