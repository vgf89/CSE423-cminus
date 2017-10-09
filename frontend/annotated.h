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
std::string printSymbolAlreadyDefinedError(int linenum1, std::string symbol, int linenum2);
std::string printSymbolNotDefinedError(int linenum, std::string symbol);
std::string simpleVarCalledError(int linenum, std::string var); 
std::string requiredOpLhsError(int linenum, std::string op, std::string reqType, std::string givenType);
std::string requiredOpRhsError(int linenum, std::string op, std::string reqType, std::string givenType);
std::string operandTypeMistmatchError(int linenum, std::string givenType, std::string lhType, std::string rhType);
std::string arrayIndexTypeError(int linenum, std::string reqType, std::string givenType);
std::string unindexedArrayError(int linenum, std::string array);
std::string indexingNamedNonArrayError(int linenum, std::string array);
std::string indexingUnamedNonArrayError(int linenum);
std::string returnArrayError(int linenum);
std::string functionAsVariableError(int linenum, std::string func);
std::string invalidArrayOperationError(int linenum, std::string op);
std::string opOnlyForArraysError(int linenum, std::string op);
std::string invalidUnaryOpError(int linenum, std::string op, std::string reqOp, std::string givenOp);
void printErrors();

//symbol table printing for debug
void printEntry(std::string name, enum typeEnum type, enum kindEnum kind, bool isStatic, bool isArray, bool isRecord, int linenum);
const char* getKind(int kind);

std::string typeToChar(enum typeEnum t);

#endif /* ANNOTATED_H */
