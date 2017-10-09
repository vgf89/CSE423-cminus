/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "symbolTable.h"
#include "annotated.h"

std::vector<std::string> errorVector;
extern int numwarn;
extern int numerror;

SymbolTable st(true); //init single symbol table object

void scopeAndType(treeNode *parseTree) {
	treeTraverse(parseTree);
}

//Builds symbol table, semantic erro list, and (optionally) prints symbol table
void treeTraverse(treeNode *curNode) {
	Entry* e = NULL;
	Entry* previous = st.getParentLast();
	static bool funcflag = false;
	switch (curNode->kind) {
		case Compound:
			// Create new scope
			if(previous != NULL && previous->kind != Func && funcflag) {
				st.newScope();
				if(yydebug) printf("New Compound scope\n");
			} else if (previous != NULL && previous->kind && !funcflag) {
				//if(yydebug) printf("Compound immediately in function, don't make new scope\n");
				funcflag = true;
			}
			break;

		case Var:
			//if(yydebug) printf("new Var: %s\n",curNode->val.id );
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
			if (e != NULL)
				printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum);
			else {
				if(yydebug)
					printEntry(curNode->val.id, curNode->type, Var, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
			}
			break;

		case Func:
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
			else {
				if(yydebug)
					printEntry(curNode->val.id, curNode->type, Func, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
			}
			st.newScope();
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
			else {
				if(yydebug)
					printEntry(curNode->val.id, curNode->type, Rec, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
			}
			break;

		case Id:
			//process ID symbol
			e = st.searchAll(std::string(curNode->val.id));
			if (e == NULL) {
				printSymbolNotDefinedError(curNode->linenum, curNode->val.id);
			}
			else {
				curNode->type = e->type;
			}
			break;
		
		case Param:
			//Declare Parameter
			e = st.insertSymbol(
				curNode->val.id,
				curNode->type,
				Param,
				curNode->isStatic,
				curNode->isArray,
				curNode->isRecord,
				curNode->linenum
			);
			if (e != NULL) {
				printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum);
			}
			else {
				if(yydebug)
					printEntry(curNode->val.id, curNode->type, Param, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
			}
			break;

		case Assign:
			//assign type for assignment
			const char *lh_type = (const char *) getType(curNode->children[0]->type);
			const char *rh_type = (const char *) getType(curNode->children[1]->type);
			if(curNode->children[0]->type != curNode->children[1]->type) {
				//operandTypeMistmatchError(curNode->linenum, "assignment", lh_type, rh_type);
			}
			else {
				curNode->type = curNode->children[0]->type;
			}
			break;
		/*
		case If:
			break;

		case While:
			break;

		case Break:
			break;

		case Call:
			break;

		case Return:
			break;

		default:
			break;
			*/
	}
	
	//Evaluate Children
	int i = 0;
	while (i < 3 || curNode->children[i] != NULL) {
		if (curNode->children[i] != NULL)
			treeTraverse(curNode->children[i]);
		i++;
	}




	// After analyzing children
	switch (curNode->kind) {
		case Compound:
			if(previous != NULL && previous->kind != Func)
				st.pop();
			break;
		case Func:
			st.pop();
			funcflag = false;
			break;
		default:
			break;
		}


		
	if (curNode->sibling != NULL) {
		treeTraverse(curNode->sibling);
	}
}

//prints one entry for symbol table printing
void printEntry(std::string name, enum typeEnum type, enum kindEnum kind, bool isStatic, bool isArray, bool isRecord, int linenum)
{
	int depth = st.getDepth();

	for (int i = 0; i < depth; ++i)
		printf("   ");

	printf("[%d] name: %10s | %10s | kind: %10s | isStatic: %d | isArray: %d | isRecord: %d |\n", linenum, name.c_str(), getType(type), getKind(kind), isStatic, isArray, isRecord);
}

//gets kind string for printEntry
const char* getKind(int kind)
{
	const char *finalString;

	switch(kind){
		case Var:
			finalString = "variable";
			break;
		case Func:
			finalString = "function";
			break;
		case Rec:
			finalString = "record";
			break;
		case Param:
			finalString = "parameter";
			break;
		case Compound:
			finalString = "compound";
			break;
		case Const:
			finalString = "constant";
			break;
		case Id:
			finalString = "id";
			break;
		case Op:
			finalString = "operator";
			break;
		case Assign:
			finalString = "assignment";
			break;
		case If:
			finalString = "if";
			break;
		case Break:
			finalString = "break";
			break;
		case Call:
			finalString = "call";
			break;
		case Return:
			finalString = "return";
			break;
		case While:
			finalString = "while";
			break;
		default:
			return finalString;
	}
	return finalString;
}

std::string printSymbolAlreadyDefinedError(int linenum1, char* symbol, int linenum2)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum1 << "): Symbol '" << symbol 
		<< "' is already defined at line " << linenum2 << ".\n"; 
	return s.str();
}

std::string printSymbolNotDefinedError(int linenum, char* symbol)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Symbol '" << symbol
		<< "' is not defined.\n";
	return s.str();
}

std::string simpleVarCalledError(int linenum, char* var)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << var
		<< "' is a simple variable and cannot be called.\n";
	return s.str();
}

std::string requiredOpLhsError(int linenum, char* reqType, char* givenType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << reqType
		<< "' requires operands of " << reqType
		<< " but lhs is of " << givenType << ".\n";
	return s.str();
}
	
std::string requiredOpRhsError(int linenum, char* reqType, char* givenType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << reqType
		<< "' requires operands of " << reqType
		<< " but rhs is of " << givenType << ".\n";
	return s.str();
}
	
std::string operandTypeMistmatchError(int linenum, char* givenType, char *lhType, char *rhType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << givenType
		<< "' requires operands of the same type but lhs is " << lhType
		<< "and rhs is " << rhType << ".\n";
	return s.str();
}
	
std::string arrayIndexTypeError(int linenum, char* reqType, char* givenType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Array '" << reqType
		<< "' should be indexed by type int but got " << givenType << ".\n";
	return s.str();
}
	
std::string unindexedArrayError(int linenum, char* array)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Array index is the unindexed array '"
		<< array << "'.\n";
	return s.str();
}
	
std::string indexingNamedNonArrayError(int linenum, char* array)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Cannot index nonarray '"
		<< array << "'.\n";
	return s.str();
}
	
std::string indexingUnamedNonArrayError(int linenum)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Cannot index nonarray.\n";
	return s.str();
}
	
std::string returnArrayError(int linenum)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Cannot return an array.\n";
	return s.str();
}
	
std::string functionAsVariableError(int linenum, char* func)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Cannot use function '"
		<< func << "' as a variable.\n";
	return s.str();
}
	
std::string invalidArrayOperationError(int linenum, char* op)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): The operation '" << op
		<< "' does not work with arrays.\n";
	return s.str();
}
	
std::string opOnlyForArraysError(int linenum, char* op)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): The operation '" << op
		<< "' only works with arrays.\n";
	return s.str();
}
	
std::string invalidUnaryOpError(int linenum, char* reqOp, char* givenOp)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Unary '" << reqOp
		<< "' requires an operand of type " << reqOp
		<< " but was given " << givenOp << ".\n";
	return s.str();
}

void printErrors()
{
	for (std::string& error: errorVector) {
		std::cout << error;
	}
}
