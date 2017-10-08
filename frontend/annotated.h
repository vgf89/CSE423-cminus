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

void scopeAndType(treeNode *parseTree);
void treeTraverse(treeNode *curTree);


// Error printing functions
void printSymbolAlreadyDefinedError(int linenum, char* s, int ln);
void printSymbolNotDefinedError(int linenum, char* s);

#endif /* ANNOTATED_H */
