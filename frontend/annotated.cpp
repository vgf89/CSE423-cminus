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
	int flag;
	switch (curNode->kind) {
	case Compound:
		// Create new scope
		if(previous != NULL && previous->kind != Func)
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
		if(previous != NULL && previous->kind != Func)
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
				requiredOpLhsError(curNode->linenum, typeToChar(BoolType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				flag++;
			} 
			if (curNode->children[1]->type != BoolType) {
				requiredOpRhsError(curNode->linenum, typeToChar(BoolType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(BoolType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				flag++;
			} 
			if (curNode->children[1]->type != BoolType) {
				requiredOpRhsError(curNode->linenum, typeToChar(BoolType), typeToChar(curNode->children[1]->type));
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
			if(curNode->children[0].isArray == 0) {
				opOnlyForArraysError(curNode->linenum, "[");
				break;
			}
			if(curNode->children[1].type != IntType) {
				arrayIndexTypeError(curNode->linenum, curNode->children[0]->id, curNode->children[1]->type);
				break;
			}
			curNode->type = curNode->children[0]->type;
			break;
		case Mul:
			if (curNode->children[1] == NULL && curNode->children[0] != NULL) {
				if (curNode->children[0].isArray == 0) {
					opOnlyForArraysError(curNode->linenum, "[")
					break;
				} else {
					curNode->type = IntType;
					break;
				}
			} else {
				int flag = 0;
				if(curNode->children[0]->type != IntType) {
					requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
					flag++;
				} 
				if (curNode->children[1]->type != IntType) {
					requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				requiredOpLhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				flag++;
			} 
			if (curNode->children[1]->type != IntType) {
				requiredOpRhsError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[1]->type));
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
				invalidUnaryOpError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				break;
			} 
			curNode->type = IntType;	
			break;
		case Dec:
			if(curNode->children[0]->type != IntType) {
				invalidUnaryOpError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				break;
			} 
			curNode->type = IntType;
			break;
		case Rand:
			if(curNode->children[0]->type != IntType) {
				invalidUnaryOpError(curNode->linenum, typeToChar(IntType), typeToChar(curNode->children[0]->type));
				curNode->type = UndefinedType;
				break;
			} 
			curNode->type = IntType;
			break;
		
		}
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

void simpleVarCalledError(int linenum, char* var)
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
	
