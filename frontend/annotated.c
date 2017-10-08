/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#include <string>
#include "symbolTable.h"
#include "annotated.h"

SymbolTable st = NULL;

void scopeAndType(treeNode *parseTree) {
	st = SymbolTable(true);
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
	if (curNode->kind == Compound)
	{
		// Create new scope
		st.newScope();
	}

	if (curNode->kind == Var)
	{
		// Declare Variable
		int retval = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Var,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord
		);
		if (retval) {
			//print error
		}
	}
	
	else if (curNode->kind == Rec)
	{
		// Declare new Record
		int retval = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Rec,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord
		);
		if (retval) {
			//print error
		}
	}

	else if (curNode->kind == Func)
	{
		// Declare new Function
		int retval = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Func,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord
		);
		if (retval) {
			//print error
		}
	}

	else if (curNode->kind == Id)
	{
		st.searchAll(std::string(curNode->val.id));
	}

	int i = 0;
	while (i < 3 || curNode->children[i] != NULL) {
		if (curNode->children[i] != NULL)
			traverseSubTree(curNode->children[i]);
		i++;
	}
	if (curNode->sibling != NULL) {
		traverseSubTree(curNode->sibling);
	}

	// This was a compound node, need to pop the top the symbol table when exiting
	if (curNode->kind == Compound)
	{
		st.pop();
	}

}
