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
std::string printSymbolAlreadyDefinedError(int linenum1, char* symbol, int linenum2);
std::string printSymbolNotDefinedError(int linenum, char* symbol);
std::string simpleVarCalledError(int linenum, char* var); 
std::string requiredOpLhsError(int linenum, char* reqType, char* givenType);
std::string requiredOpRhsError(int linenum, char* reqType, char* givenType);
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
void printErrors();

//symbol table printing for debug
void printEntry(std::string name, enum typeEnum type, enum kindEnum kind, bool isStatic, bool isArray, bool isRecord, int linenum);
const char* getKind(int kind);

#endif /* ANNOTATED_H */
