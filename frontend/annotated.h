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
std::string operandTypeMistmatchError(int linenum, char* givenType, char *lhType, char *rhType);
std::string arrayIndexTypeError(int linenum, char* reqType, char* givenType);
std::string unindexedArrayError(int linenum, char* array);
std::string indexingNamedNonArrayError(int linenum, char* array);
std::string indexingUnamedNonArrayError(int linenum);
std::string returnArrayError(int linenum);
std::string functionAsVariableError(int linenum, char* func);
std::string invalidArrayOperationError(int linenum, char* op);
std::string opOnlyForArraysError(int linenum, char* op);
std::string invalidUnaryOpError(int linenum, char* reqOp, char* givenOp);
void printErrors();

//symbol table printing for debug
void printEntry(std::string name, enum typeEnum type, enum kindEnum kind, bool isStatic, bool isArray, bool isRecord, int linenum);
const char* getKind(int kind);

#endif /* ANNOTATED_H */
