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

#endif /* ANNOTATED_H */
