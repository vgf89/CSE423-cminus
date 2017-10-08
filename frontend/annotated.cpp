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

void simpleVarCalledError(int lineum, char* s) 
{
	printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n");
}

/*
"ERROR(%d): '%s' is a simple variable and cannot be called.\n"
"ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n"
"ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n"
"ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n"
"ERROR(%d): Array '%s' should be indexed by type int but got %s.\n"
"ERROR(%d): Array index is the unindexed array '%s'.\n"
"ERROR(%d): Cannot index nonarray '%s'.\n"
"ERROR(%d): Cannot index nonarray.\n"
"ERROR(%d): Cannot return an array.\n"
"ERROR(%d): Cannot use function '%s' as a variable.\n"
"ERROR(%d): Symbol '%s' is already defined at line %d.\n"
"ERROR(%d): Symbol '%s' is not defined.\n"
"ERROR(%d): The operation '%s' does not work with arrays.\n"
"ERROR(%d): The operation '%s' only works with arrays.\n"
"ERROR(%d): Unary '%s' requires an operand of type %s but was given %s.\n"
*/