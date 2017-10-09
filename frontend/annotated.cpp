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

static bool funcflag = false;

void scopeAndType(treeNode *parseTree) {
	treeTraverse(parseTree);
}

//Builds symbol table, semantic erro list, and (optionally) prints symbol table
void treeTraverse(treeNode *curNode) {
	Entry* e = NULL;
	Entry* previous = st.getParentLast();
	int flag;

	bool dontkill = false;
	switch (curNode->kind) {
		case Compound:
			// Create new scope
			if(previous != NULL && previous->kind == Func && funcflag) {
				dontkill = true;
				funcflag = false;
			} else {
				st.newScope();
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

		case Call:
			e = st.searchAll(curNode->val.id);
			if (e != NULL) {
				if(e->kind != Func) {
					simpleVarCalledError(e->linenum, curNode->val.id);
				} else {
					curNode->type = e->type;
				}
			} else {
				printSymbolNotDefinedError(e->linenum, curNode->val.id);
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
			break; */
			
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
		if(!dontkill)
			st.pop();
		break;
	case Func:
		st.pop();
		break;
	case Op:
		switch (curNode->opType) {
		case And:
			flag = 0;
			if(curNode->children[0]->type != BoolType) {
				requiredOpLhsError(curNode->linenum, "and", typeToChar(BoolType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				flag++;
			} 
			if (curNode->children[1]->type != BoolType) {
				requiredOpRhsError(curNode->linenum, "and", typeToChar(BoolType), typeToChar(curNode->children[1]->type));
				curNode->type = UndefinedType;
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Or:
			flag = 0;
			if(curNode->children[0]->type != BoolType) {
				requiredOpLhsError(curNode->linenum, "or", typeToChar(BoolType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				flag++;
			} 
			if (curNode->children[1]->type != BoolType) {
				requiredOpRhsError(curNode->linenum, "or", typeToChar(BoolType), typeToChar(curNode->children[1]->type));
				curNode->type = UndefinedType;
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Eq:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, "=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = curNode->children[0]->type;
			}
			break;
		case EEq:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, "==", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Noteq:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, "!=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Lss:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, "<", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Gss:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, ">", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Leq:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, "<=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Geq:
			if(curNode->children[0]->type != curNode->children[1]->type) {
				operandTypeMistmatchError(curNode->linenum, ">=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
			} else {
				curNode->type = BoolType;
			}
			break;
		case Bracl:
			if(curNode->children[0]->isArray == 0) {
				opOnlyForArraysError(curNode->linenum, "[");
				break;
			}
			if(curNode->children[1]->type != IntType) {
				arrayIndexTypeError(curNode->linenum, curNode->children[0]->val.id, curNode->children[1]->type);
				break;
			}
			curNode->type = curNode->children[0]->type;
			break;
		case Mul:
			if (curNode->children[1] == NULL && curNode->children[0] != NULL) {
				if (curNode->children[0].isArray == 0) {
					opOnlyForArraysError(curNode->linenum, "*");
					break;
				} else {
					curNode->type = IntType;
					break;
				}
			} else {
				int flag = 0;
				if(curNode->children[0]->type != IntType) {
					requiredOpLhsError(curNode->linenum, "*", typeToChar(IntType), typeToChar(curNode->children[0]->type));
					flag++;
				} 
				if (curNode->children[1]->type != IntType) {
					requiredOpRhsError(curNode->linenum, "*", typeToChar(IntType), typeToChar(curNode->children[1]->type));
					flag++;
				}
				if (flag) {
					curNode->type = UndefinedType;
				} else {
					curNode->type = IntType;
				}
			}
			break;	
			/*Or, And, Not, Leq, Geq, Lss, Gss, Eq, AddE, SubE, MulE, DivE, Noteq, Add, Sub, Mul, Div, Mod, Rand, Neg, Inc, Dec, Dot, Bracl, EEq*/
		case Add:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "+", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "+", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case Sub:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "-", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "-", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case AddE:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "+=", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "+=", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case SubE:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "-=", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "-=", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case MulE:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "*=", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "*=", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case Div:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "/", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "/", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case DivE:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "/=", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "/=", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case Mod:
			flag = 0;
			if(curNode->children[0]->type != IntType) {
				requiredOpLhsError(curNode->linenum, "%", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, "%", typeToChar(IntType), typeToChar(curNode->children[1]->type));
				flag++;
			}
			if (flag) {
				curNode->type = UndefinedType;
			} else {
				curNode->type = IntType;
			}	
			break;
		case Inc:
			if(curNode->children[0]->type != IntType) {
				invalidUnaryOpError(curNode->linenum, "++", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				break;
			} 
			curNode->type = IntType;	
			break;
		case Dec:
			if(curNode->children[0]->type != IntType) {
				invalidUnaryOpError(curNode->linenum, "--", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				break;
			} 
			curNode->type = IntType;
			break;
		case Rand:
			if(curNode->children[0]->type != IntType) {
				invalidUnaryOpError(curNode->linenum, "?", typeToChar(IntType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				break;
			} 
			curNode->type = IntType;
			break;
		
		}
	
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

const char* typeToChar(enum typeEnum t, char* c) {
	/*IntType, VoidType, CharType, BoolType, RecordType, UndefinedType*/
	switch(t) {
	case IntType:
		return "Int";
		break;
	case VoidType:
		return "Void";
		break;
	case CharType:
		return "Char";
		break;
	case BoolType:
		return "Bool";
		break;
	case RecordType:
		return "Record";
		break;
	case UndefinedType:
		return "Undefined";
		break;
	default:
		return " ";
		break;
	}
}