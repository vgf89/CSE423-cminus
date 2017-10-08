/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#include <string>
#include "symbolTable.h"
#include "annotated.h"

SymbolTable st = new SymbolTable(true);

void scopeAndType(treeNode *parseTree) {
	treeTraverse(parseTree);
}

void treeTraverse(treeNode *parseTree) {
	if(parseTree != NULL) {	
		traverseSubTree(parseTree);
	}
}

void traverseSubTree(treeNode *curNode) {
	if (curNode->kind == Compound)
	{
		// Create new scope
		st.newScope();
	}

	else if (curNode->kind == Var)
	{
		//printf("new Var: %s\n",curNode->val.id );
		// Declare Variable
		Entry* e = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Var,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord,
			curNode->linenum
		);
		if (e != NULL) {
			printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum);
		}
	}

	else if (curNode->kind == Func)
	{
		printf("new Func: %s\n", curNode->val.id);
		// Declare new Function
		Entry* e = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Func,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord,
			curNode->linenum
		);
		if (e != NULL) {
			printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum);
		}
	}
	
	else if (curNode->kind == Rec)
	{
		// Declare new Record
		Entry* e = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Rec,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord,
			curNode->linenum
		);
		if (e != NULL) {
			printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum);
		}
	}

	else if (curNode->kind == Id)
	{
		//printf("check %s\n", curNode->val.id);
		Entry* e = st.searchAll(std::string(curNode->val.id));
		if (e == NULL) {
			printSymbolNotDefinedError(curNode->linenum, curNode->val.id);
		}
	}
	
	else if (curNode->kind == Param)
	{
		// Declare Variable
		Entry* e = st.insertSymbol(
			curNode->val.id,
			curNode->type,
			Var,
			curNode->isStatic,
			curNode->isArray,
			curNode->isRecord,
			curNode->linenum
		);
		if (e != NULL) {
			printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum);
		}
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


void printSymbolAlreadyDefinedError(int linenum, char* s, int ln)
{
	printf("ERROR(%d): Symbol '%s' is already defined at line %d.\n", linenum, s, ln);
}

void printSymbolNotDefinedError(int linenum, char* s)
{
	printf("ERROR(%d): Symbol '%s' is not defined.\n", linenum, s);
}