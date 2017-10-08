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
	Entry* previous = st.getParentLast();
	switch (curNode->kind) {
	case Compound:
		// Create new scope
		if(previous && previous->kind != Func)
			st.newScope();
		break;

	case Var:
		//printf("new Var: %s\n",curNode->val.id );
		// Declare Variable
		e = st.insertSymbol(
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
		e = st.insertSymbol(
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
		e = st.insertSymbol(
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
		e = st.searchAll(std::string(curNode->val.id));
		if (e == NULL) {
			printSymbolNotDefinedError(curNode->linenum, curNode->val.id);
		}
		break;
	
	case Param:
		// Declare Variable
		e = st.insertSymbol(
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
		if(previous && previous->kind != Func)
			st.pop();
		break;
	case Func:
		st.pop();
		break;
	}
}

void printSymbolAlreadyDefinedError(int linenum1, char* symbol, int linenum2)
{
	printf("ERROR(%d): Symbol '%s' is already defined at line %d.\n", linenum1, symbol, linenum2);
}

void printSymbolNotDefinedError(int linenum, char* symbol)
{
	printf("ERROR(%d): Symbol '%s' is not defined.\n", linenum, symbol);
}

void simpleVarCalledError(int lineum, char* var)
{
	printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", linenum, var);
}

void requiredOpLhsError(int linenum, char* reqType, char* givenType)
{
	printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", linenum, reqType, givenType);
}
	
void requiredOpRhsError(int linenum, char* reqType, char* givenType)
{
	printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", linenum, reqType, givenType);
}
	
void operandTypeMistmatchError(int linenum, char* givenType, char *lhType, char *rhType)
{
	printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", linenum, givenType, lhType, rhType);
}
	
void arrayIndexTypeError(int linenum, char* reqType, char* givenType)
{
	printf("ERROR(%d): Array '%s' should be indexed by type int but got %s.\n", linenum, reqType, givenType);
}
	
void unindexedArrayError(int linenum, char* array)
{
	printf("ERROR(%d): Array index is the unindexed array '%s'.\n", linenum, array);
}
	
void indexingNamedNonArrayError(int linenum, char* array)
{
	printf("ERROR(%d): Cannot index nonarray '%s'.\n", linenum, array);
}
	
void indexingUnamedNonArrayError(int linenum)
{
	printf("ERROR(%d): Cannot index nonarray.\n", linenum);
}
	
void returnArrayError(int linenum)
{
	printf("ERROR(%d): Cannot return an array.\n", linenum);
}
	
void functionAsVariableError(int linenum, char* func)
{
	printf("ERROR(%d): Cannot use function '%s' as a variable.\n", linenum, func);
}
	
void invalidArrayOperationError(int linenum, char* op)
{
	printf("ERROR(%d): The operation '%s' does not work with arrays.\n", linenum, op);
}
	
void opOnlyForArraysError(int linenum, char* op)
{
	printf("ERROR(%d): The operation '%s' only works with arrays.\n", linenum, op);
}
	
void invalidUnaryOpError(int linenum, char* reqOp, char* givenOp)
{
	printf("ERROR(%d): Unary '%s' requires an operand of type %s but was given %s.\n", linenum, reqOp, givenOp);
}
	
