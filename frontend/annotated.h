/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#ifndef ANNOTATED_H
#define ANNOTATED_H

#include "printtree.h"
#include "symbolTable.h"

void scopeAndType(treeNode *parseTree);
void treeTraverse(treeNode *parseTree);
void traverseSubTree(treeNode *curNode);


// Error printing functions
void printSymbolAlreadyDefinedError(int linenum, char* s);
void printSymbolNotDefinedError(int linenum, char* s);

#endif /* ANNOTATED_H */
