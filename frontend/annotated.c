/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#include "symbolTable.h"
#include "annotated.h"

void scopeAndType(treeNode *parseTree) {
	treeTraverse(parseTree);
}

void treeTraverse(treeNode *parseTree) {
	if(parseTree != NULL) {	
		int i = 0;
		while (i < 3 || parseTree->children[i] != NULL) {
			if (parseTree->children[i] != NULL)
				traverseSubTree(parseTree->children[i]);
			i++;
		}
		if (parseTree->sibling != NULL) {
			traverseSubTree(parseTree->sibling);
		}
	}
}

void traverseSubTree(treeNode *curNode) {
	int i = 0;
	while (i < 3 || curNode->children[i] != NULL) {
		if (curNode->children[i] != NULL)
			traverseSubTree(curNode->children[i]);
		i++;
	}
	if (curNode->sibling != NULL) {
		traverseSubTree(curNode->sibling);
	}

}
