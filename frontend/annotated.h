/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#ifndef ANNOTATED_H
#define ANNOTATED_H

#include "printtree.h"
#include "symbolTable.h"

extern int numwarn;
extern int numerror;

extern int yydebug;

void scopeAndType(treeNode *parseTree);
void treeTraverse(treeNode *curTree);


// Error printing functions
void printSymbolAlreadyDefinedError(int linenum1, char* symbol, int linenum2);
void printSymbolNotDefinedError(int linenum, char* symbol);
void simpleVarCalledError(int linenum, char* var); 
void requiredOpLhsError(int linenum, char* reqType, char* givenType);
void requiredOpRhsError(int linenum, char* reqType, char* givenType);
void operandTypeMistmatchError(int linenum, char* givenType, char *lhType, char *rhType);
void arrayIndexTypeError(int linenum, char* reqType, char* givenType);
void unindexedArrayError(int linenum, char* array);
void indexingNamedNonArrayError(int linenum, char* array);
void indexingUnamedNonArrayError(int linenum);
void returnArrayError(int linenum);
void functionAsVariableError(int linenum, char* func);
void invalidArrayOperationError(int linenum, char* op);
void opOnlyForArraysError(int linenum, char* op);
void invalidUnaryOpError(int linenum, char* reqOp, char* givenOp);

#endif /* ANNOTATED_H */
