/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#include <string>
#include <cstring>
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

int main_defined = false;

//Builds symbol table, semantic erro list, and (optionally) prints symbol table
void treeTraverse(treeNode *curNode) {
	Entry* e = NULL;
	Entry* previous = st.getParentLast();
	bool dontkill = false;
	switch (curNode->kind)
	{
	case Compound:
	{
		//printf("COMPOUND\n");
		// Create new scope
		if(previous != NULL && previous->kind == Func && funcflag) {
			dontkill = true;
			funcflag = false;
		} else {
			st.newScope();
		}
		break;
	}
	case Func:
	{
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
			errorVector.push_back(printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum));
		}
		else {
			if(yydebug)
				printEntry(curNode->val.id, curNode->type, Func, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
		}
		st.newScope();
		funcflag = true; // Special Case: Prevent new scope immediately after function declaration

		if (main_defined && strcmp(curNode->val.id, "main") == 0) {
			// TODO: Add main redefined print option
		}
		else if (strcmp(curNode->val.id, "main") == 0) {
			main_defined = true;
		}
		break;
	}
	case Rec:
	{
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
			errorVector.push_back(printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum));
		}
		else {
			if(yydebug)
				printEntry(curNode->val.id, curNode->type, Rec, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
		}
		st.newScope();
		break;
	}
	case Id:
	{
		//process ID symbol
		e = st.searchAll(std::string(curNode->val.id));
		if (e == NULL) {
			errorVector.push_back(printSymbolNotDefinedError(curNode->linenum, curNode->val.id));
			curNode->type = UndefinedType;
		} else if (e->kind == Func) {
			curNode->type = UndefinedType;
		} else {
			curNode->type = e->type;
			curNode->isStatic = e->isStatic;
			curNode->isArray = e->isArray;
			curNode->isRecord = e->isRecord;
		}
		break;
	}
	case Param:
	{
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
			errorVector.push_back(printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum));
		}
		else {
			if(yydebug)
				printEntry(curNode->val.id, curNode->type, Param, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
		}
		break;
	}
	case Call:
	{
		e = st.searchAll(curNode->val.id);
		if (e != NULL) {
			if(e->kind != Func) {
				errorVector.push_back(simpleVarCalledError(curNode->linenum, curNode->val.id));
			} else {
				curNode->type = e->type;
			}
		} else {
			errorVector.push_back(printSymbolNotDefinedError(curNode->linenum, curNode->val.id));
			curNode->type = UndefinedType;
		}
		break;
	}
	default:
		//printf("hit default 2\n");
		break;
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
		funcflag = true;
		break;
	case Rec:
	{
		st.pop();
		break;
	}
	case Return:
	{
		if (curNode->children[0] != NULL && curNode->children[0]->isArray == 1) {
			errorVector.push_back(returnArrayError(curNode->linenum));
		}
		break;
	}
	case Var:
	{
		//printf("new Var: %s, %d, %p\n", curNode->val.id, curNode->type, previous);
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
			errorVector.push_back(printSymbolAlreadyDefinedError(curNode->linenum, curNode->val.id, e->linenum));
		}else {
			if(yydebug)
				printEntry(curNode->val.id, curNode->type, Var, curNode->isStatic, curNode->isArray, curNode->isRecord, curNode->linenum);
		}
		break;
	}
	case Assign:
		{
		//assign type for assignment
		//first check +=, -=, /=, *=
		Entry* e = NULL;
		switch(curNode->opType) {
		case AddE:
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if (curNode->children[1]->kind == Id) {
				e = st.searchAll(curNode->children[1]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[1]->val.id));
					break;
				}
			}
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "+="));
			}
			if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "+="));
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(requiredOpLhsError(curNode->linenum, "+=", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
			}	
			break;
		case SubE:
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if (curNode->children[1]->kind == Id) {
				e = st.searchAll(curNode->children[1]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[1]->val.id));
					break;
				}
			}
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "-="));
			}
			if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "-="));
			}
			if(curNode->children[0]->type != curNode->children[1]->type
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(requiredOpRhsError(curNode->linenum, "-=", typeToChar(IntType), typeToChar(curNode->children[1]->type)));
			}
			break;
		case MulE:
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if (curNode->children[1]->kind == Id) {
				e = st.searchAll(curNode->children[1]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[1]->val.id));
					break;
				}
			}
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "*="));
			}
			if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "*="));
			}
			if(curNode->children[0]->type != curNode->children[1]->type
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "*=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}	
			break;
		case DivE:
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if (curNode->children[1]->kind == Id) {
				e = st.searchAll(curNode->children[1]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[1]->val.id));
					break;
				}
			}
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "/="));
			}
			if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "/="));
			}
			if(curNode->children[0]->type != curNode->children[1]->type
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "/=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}
			break;
		case Inc:
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if(curNode->children[0]->type != IntType) {
				errorVector.push_back(invalidUnaryOpError(curNode->linenum, "++", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				break;
			}
			else
				curNode->type = IntType;
			break;
		case Dec:
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if(curNode->children[0]->type != IntType) {
				errorVector.push_back(invalidUnaryOpError(curNode->linenum, "--", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				break;
			} 
			else 
				curNode->type = IntType;
			break;

		default: //=
			if (curNode->children[0]->kind == Id) {
				e = st.searchAll(curNode->children[0]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
					break;
				}
			}
			if (curNode->children[1]->kind == Id) {
				e = st.searchAll(curNode->children[1]->val.id);
				if(e != NULL && e->kind == Func) {
					errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[1]->val.id));
					break;
				}
			}
			std::string lh_type = typeToChar(curNode->children[0]->type);
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "="));
			}
			else if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "="));
			} else if(curNode->children[0]->type != curNode->children[1]->type
				&& curNode->children[0]->type != UndefinedType
				&& curNode->children[1]->type != UndefinedType ) {
				std::string rh_type = typeToChar(curNode->children[1]->type);
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "=", lh_type, rh_type));
			}
			curNode->type = curNode->children[0]->type;
			break;
		}
	}
	case Op:
		if (curNode->children[0]->kind == Id) {
			e = st.searchAll(curNode->children[0]->val.id);
			if(e != NULL && e->kind == Func) {
				errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[0]->val.id));
				break;
			}
		}
		if (curNode->opType != Mul && curNode->opType != Sub && curNode->opType != Rand && curNode->opType != Not && curNode->opType != Neg && curNode->children[1] != NULL && curNode->children[1]->kind == Id) {
			e = st.searchAll(curNode->children[1]->val.id);
			if(e != NULL && e->kind == Func) {
				errorVector.push_back(functionAsVariableError(curNode->linenum, curNode->children[1]->val.id));
				break;
			}
		}
		switch (curNode->opType) {
		case And:
			if(curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "and"));
			}
			if(curNode->children[0]->type != BoolType && curNode->children[0]->type != UndefinedType) {
				errorVector.push_back(requiredOpLhsError(curNode->linenum, "and", typeToChar(BoolType), typeToChar(curNode->children[0]->type)));
			} 
			if (curNode->children[1]->type != BoolType && curNode->children[1]->type != UndefinedType) {
				errorVector.push_back(requiredOpRhsError(curNode->linenum, "and", typeToChar(BoolType), typeToChar(curNode->children[1]->type)));
			}
			break;
		case Or:
			if(curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "or"));
			}
			if(curNode->children[0]->type != BoolType && curNode->children[0]->type != UndefinedType) {
				errorVector.push_back(requiredOpLhsError(curNode->linenum, "or", typeToChar(BoolType), typeToChar(curNode->children[0]->type)));
			} 
			if (curNode->children[1]->type != BoolType && curNode->children[1]->type != UndefinedType) {
				errorVector.push_back(requiredOpRhsError(curNode->linenum, "or", typeToChar(BoolType), typeToChar(curNode->children[1]->type)));

			}
			break;
		case Not:
			if(curNode->children[0]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "not"));
				break;
			}
			if(curNode->children[0]->type != BoolType && curNode->children[0]->type != UndefinedType) {
				errorVector.push_back(invalidUnaryOpError(curNode->linenum, "not", typeToChar(BoolType), typeToChar(curNode->children[0]->type)));
				break;
			} 
			break;
		case Neg:
			if(curNode->children[0]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "!"));
				//curNode->type = UndefinedType;
				break;
			}
			if(curNode->children[0]->type != BoolType && curNode->children[0]->type != UndefinedType) {
				errorVector.push_back(invalidUnaryOpError(curNode->linenum, "!", typeToChar(BoolType), typeToChar(curNode->children[0]->type)));
				//curNode->type = UndefinedType;
				break;
			} 
			//curNode->type = BoolType;
			break;
		case EEq:
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "=="));
			}
			if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "=="));
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "==", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}
			break;
		case Noteq:
			if(curNode->children[0]->type == VoidType) {
				errorVector.push_back(voidOpLhsError(curNode->linenum, "!="));
			}
			if(curNode->children[1]->type == VoidType) {
				errorVector.push_back(voidOpRhsError(curNode->linenum, "!="));
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "!=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}

			break;
		case Lss:
			if(curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "<"));
				break;
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "<", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}

			break;
		case Gss:
			if(curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, ">"));
				break;
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, ">", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}

			break;
		case Leq:
			if(curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "<="));
				break;
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, "<=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
				break;
			}
			break;
		case Geq:
			if(curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, ">="));
				break;
			}
			if(curNode->children[0]->type != curNode->children[1]->type 
				&& (curNode->children[0]->type != UndefinedType && curNode->children[1]->type != UndefinedType )) {
				errorVector.push_back(operandTypeMistmatchError(curNode->linenum, ">=", typeToChar(curNode->children[0]->type), typeToChar(curNode->children[1]->type)));
			}
			break;
		case Bracl:
				if(curNode->children[0] != NULL) {
					if(curNode->children[0]->opType == Bracl)
						curNode->type = UndefinedType;
					else
						curNode->type = curNode->children[0]->type;
				}
			if(curNode->children[0]->kind == Id && curNode->children[0]->isArray == 0) {
				errorVector.push_back(indexingNamedNonArrayError(curNode->linenum, curNode->children[0]->val.id)); 
				//errorVector.push_back(opOnlyForArraysError(curNode->linenum, "["));
				break;
			} else if (curNode->children[0]->kind != Id && curNode->children[0]->isArray == 0) {
				errorVector.push_back(indexingUnamedNonArrayError(curNode->linenum)); 
			}
			if(curNode->children[1]->type != IntType && curNode->children[1]->type != UndefinedType) {
				errorVector.push_back(arrayIndexTypeError(curNode->linenum, curNode->children[0]->val.id, typeToChar(curNode->children[1]->type)));
			}
			if(curNode->children[1]->isArray) {
				errorVector.push_back(unindexedArrayError(curNode->linenum, curNode->children[1]->val.id));
			}
			curNode->type = curNode->children[0]->type;
			break;
		case Mul:
			if (curNode->children[1] == NULL && curNode->children[0] != NULL) {
				if (curNode->children[0]->isArray == 0 && curNode->children[0]->type != UndefinedType) {
					errorVector.push_back(opOnlyForArraysError(curNode->linenum, "*"));
					break;
				}
			} else {
				if(curNode->children[0]->type != IntType && curNode->children[0]->type != UndefinedType) {
					errorVector.push_back(requiredOpLhsError(curNode->linenum, "*", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				} 
				if (curNode->children[1]->type != IntType && curNode->children[1]->type != UndefinedType) {
					errorVector.push_back(requiredOpRhsError(curNode->linenum, "*", typeToChar(IntType), typeToChar(curNode->children[1]->type)));
				}
			}
			break;	
			/*Or, And, Not, Leq, Geq, Lss, Gss, Eq, AddE, SubE, MulE, DivE, Noteq, Add, Sub, Mul, Div, Mod, Rand, Neg, Inc, Dec, Dot, Bracl, EEq*/
		case Add:
			if (curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "+"));
			} else {
				if(curNode->children[0]->type != IntType && curNode->children[0]->type != UndefinedType) {
					errorVector.push_back(requiredOpLhsError(curNode->linenum, "+", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				} 
				if (curNode->children[1]->type != IntType && curNode->children[1]->type != UndefinedType) {
					errorVector.push_back(requiredOpRhsError(curNode->linenum, "+", typeToChar(IntType), typeToChar(curNode->children[1]->type)));
				}
			}
			break;
		case Sub:
			if (curNode->children[0]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "-"));
			} else if (curNode->children[1] != NULL && 
				       curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "-"));
			} else {
				if(curNode->children[0]->type != IntType && curNode->children[0]->type != UndefinedType) {
					errorVector.push_back(invalidUnaryOpError(curNode->linenum, "-", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				} 
				if (curNode->children[1] == NULL) {
					curNode->type = curNode->children[0]->type;
				} else if (curNode->children[1]->type != IntType && curNode->children[1]->type != UndefinedType) {
					errorVector.push_back(invalidUnaryOpError(curNode->linenum, "-", typeToChar(IntType), typeToChar(curNode->children[1]->type)));
				}
			}
			break;
		case Div:
			if (curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "/"));
			} else {
				if(curNode->children[0]->type != IntType && curNode->children[0]->type != UndefinedType) {
					errorVector.push_back(requiredOpLhsError(curNode->linenum, "/", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				} 
				if (curNode->children[1]->type != IntType && curNode->children[1]->type != UndefinedType) {
					errorVector.push_back(requiredOpRhsError(curNode->linenum, "/", typeToChar(IntType), typeToChar(curNode->children[1]->type)));
				}
			}
			break;
		case Mod:
			if (curNode->children[0]->isArray || curNode->children[1]->isArray) {
				errorVector.push_back(invalidArrayOperationError(curNode->linenum, "%"));
			} else {
				if(curNode->children[0]->type != IntType && curNode->children[0]->type != UndefinedType) {
					errorVector.push_back(requiredOpLhsError(curNode->linenum, "%", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
				} 
				if (curNode->children[1]->type != IntType && curNode->children[1]->type != UndefinedType) {
					errorVector.push_back(requiredOpRhsError(curNode->linenum, "%", typeToChar(IntType), typeToChar(curNode->children[1]->type)));
			
				}
			}
			break;
		case Rand:
			if (curNode->children[1] == NULL && curNode->children[0] != NULL) {
				if (curNode->children[0]->isArray == 1) {
					errorVector.push_back(invalidArrayOperationError(curNode->linenum, "?"));
					break;
				} else if(curNode->children[0]->type != IntType) {
					errorVector.push_back(invalidUnaryOpError(curNode->linenum, "?", typeToChar(IntType), typeToChar(curNode->children[0]->type)));
					curNode->type = UndefinedType;
					break;
				} else{
					curNode->type = IntType;
					break;
				}
			}
			break;
		/* dot needs to be fixed */
		case Dot:
			if (curNode->children[0]->kind != Id) {
				if(curNode->children[0]->opType != Dot)
					curNode->type = UndefinedType;
			} else {
				Entry *e;
				e = st.searchAll(curNode->children[0]->val.id);
				if (e->isRecord == 0) {
					curNode->type = IntType;
				} else {
					curNode->type = IntType;					
				}
			}
			break;
		default:
			break;
		}
		
	default:
		//printf("hit the default\n");
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

std::string printSymbolAlreadyDefinedError(int linenum1, std::string symbol, int linenum2)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum1 << "): Symbol '" << symbol 
		<< "' is already defined at line " << linenum2 << ".\n"; 
	return s.str();
}

std::string printSymbolNotDefinedError(int linenum, std::string symbol)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Symbol '" << symbol
		<< "' is not defined.\n";
	return s.str();
}

std::string simpleVarCalledError(int linenum, std::string var)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << var
		<< "' is a simple variable and cannot be called.\n";
	return s.str();
}

std::string requiredOpLhsError(int linenum, std::string op, std::string reqType, std::string givenType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << op
		<< "' requires operands of type " << reqType
		<< " but lhs is of type " << givenType << ".\n";
	return s.str();
}
	
std::string requiredOpRhsError(int linenum, std::string op, std::string reqType, std::string givenType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << op
		<< "' requires operands of type " << reqType
		<< " but rhs is of type " << givenType << ".\n";
	return s.str();
}

std::string voidOpLhsError(int linenum, std::string op)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << op << "' requires operands of NONVOID but lhs is of type void.\n";
	return s.str();
}
	
std::string voidOpRhsError(int linenum, std::string op)
{
	numerror++;
	std::ostringstream s;
		s << "ERROR(" << linenum << "): '" << op << "' requires operands of NONVOID but rhs is of type void.\n";
	return s.str();
}
	
std::string operandTypeMistmatchError(int linenum, std::string givenType, std::string lhType, std::string rhType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): '" << givenType
		<< "' requires operands of the same type but lhs is type " << lhType
		<< " and rhs is type " << rhType << ".\n";
	return s.str();
}
	
std::string arrayIndexTypeError(int linenum, std::string reqType, std::string givenType)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Array '" << reqType
		<< "' should be indexed by type int but got type " << givenType << ".\n";
	return s.str();
}
	
std::string unindexedArrayError(int linenum, std::string array)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Array index is the unindexed array '"
		<< array << "'.\n";
	return s.str();
}
	
std::string indexingNamedNonArrayError(int linenum, std::string array)
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
	
std::string functionAsVariableError(int linenum, std::string func)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): Cannot use function '"
		<< func << "' as a variable.\n";
	return s.str();
}
	
std::string invalidArrayOperationError(int linenum, std::string op)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): The operation '" << op
		<< "' does not work with arrays.\n";
	return s.str();
}
	
std::string opOnlyForArraysError(int linenum, std::string op)
{
	numerror++;
	std::ostringstream s;
	s << "ERROR(" << linenum << "): The operation '" << op
		<< "' only works with arrays.\n";
	return s.str();
}
	
std::string invalidUnaryOpError(int linenum, std::string op, std::string reqOp, std::string givenOp)
{
	numerror++;
	std::ostringstream s;
	// TODO: This has a typo 'type type' but the test files have the same problem.
	s << "ERROR(" << linenum << "): Unary '" << op
		<< "' requires an operand of type type " << reqOp
		<< " but was given type " << givenOp << ".\n";

		
	return s.str();
}

void printErrors()
{
	for (std::string& error: errorVector) {
		std::cout << error;
	}
}

std::string typeToChar(enum typeEnum t) {
	/*IntType, VoidType, CharType, BoolType, RecordType, UndefinedType*/
	switch(t) {
	case IntType:
		return "int";
		break;
	case VoidType:
		return "void";
		break;
	case CharType:
		return "char";
		break;
	case BoolType:
		return "bool";
		break;
	case RecordType:
		return "record";
		break;
	case UndefinedType:
		return "undefined";
		break;
	default:
		return " ";
		break;
	}
}
