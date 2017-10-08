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

void treeTraverse(treeNode *curNode) {
	Entry* e = NULL;
	switch (curNode->kind) {
	case Compound:
		// Create new scope
		st.newScope();
		break;

	case Var:
		//printf("new Var: %s\n",curNode->val.id );
		// Declare Variable
		st.insertSymbol(
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
		break;

	case Func:
		printf("new Func: %s\n", curNode->val.id);
		// Declare new Function
		st.insertSymbol(
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
		break;
	
	case Rec:
		// Declare new Record
		st.insertSymbol(
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
		break;

	case Id:
		//printf("check %s\n", curNode->val.id);
		st.searchAll(std::string(curNode->val.id));
		if (e == NULL) {
			printSymbolNotDefinedError(curNode->linenum, curNode->val.id);
		}
		break;
	
	case Param:
		// Declare Variable
		st.insertSymbol(
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
		break;
	}
	
	//Evvaluate Children
	int i = 0;
	while (i < 3 || curNode->children[i] != NULL) {
		if (curNode->children[i] != NULL)
			treeTraverse(curNode->children[i]);
		i++;
	}
	if (curNode->sibling != NULL) {
		treeTraverse(curNode->sibling);
	}


	// This was a compound node, need to pop the top the symbol table when exiting
	switch (curNode->kind) {
	case Compound:
		st.pop();
		break;
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