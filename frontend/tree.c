#include "tree.h"
#include "scanType.h"
#include <stdlib.h>


void printTree(treeNode *parseTree) 
{
	//If sentinal node has no children the program is empty, else it has one child which is the true root.
	if(parseTree == NULL) {
		printf("Tree is empty.\n");
		return;
	}
	else {	
		int siblingNum = -1;
		int childNum = -1;
		int treeLevel = 0;

		//Special case for printing of root so we don't write "Child" before it
		printNode(parseTree);

		int i = 0;
		while (parseTree->children[i] != NULL) {
			printSubTree(parseTree->children[i], -1, i, treeLevel + 1);
			i++;
		}
		if (parseTree->sibling != NULL) {
			printSubTree(parseTree->sibling, 0, -1, treeLevel);
		}
	}
}

void printSubTree(treeNode *curNode, int siblingNum, int childNum, int treeLevel) 
{
	int i = 0;
	for (i = 0; i < treeLevel; i++) {
		printf("!   ");
	}

	if (siblingNum == -1) {
		printf("Child: %d  ", childNum);
		printNode(curNode);
	} else {
		printf("Sibling: %d  ", siblingNum);
		printNode(curNode);
	}

	i = 0;
	// Always try to print first three children (since the first two or three could be void)
	// Makes sure functions without params and matched/unmatched stuff always prints properly
	// A less hacky way of doing this would be to add a first-child value to nodes, but that would be messier IMO
	while (i < 3 || curNode->children[i] != NULL) {
		if (curNode->children[i] != NULL)
			printSubTree(curNode->children[i], -1, i, treeLevel + 1);
		i++;
	}
	if (curNode->sibling != NULL) {
		printSubTree(curNode->sibling, siblingNum + 1, -1, treeLevel);
	}

}

/*
 * 
 */
void printNode(treeNode *parseTree)
{
		if(parseTree->kind == Var)
			printVar(parseTree, parseTree->val.id, parseTree->type, parseTree->linenum);
		
		else if (parseTree->kind == Rec)
			printRec(parseTree, parseTree->val.id, parseTree->linenum);

		else if (parseTree->kind == Func)
			printFunc(parseTree->val.id, parseTree->type, parseTree->linenum);

		else if (parseTree->kind == Param)
			printParam(parseTree, parseTree->val.id, parseTree->type, parseTree->linenum);

		else if (parseTree->kind == Compound)
			printCompound(parseTree->linenum);

		else if (parseTree->kind == Const)
			printConst(parseTree, parseTree->type, parseTree->linenum);

		else if (parseTree->kind == Id)
			printId(parseTree->val.id, parseTree->linenum);

		else if (parseTree->kind == Op)
			printOp(parseTree->val.op, parseTree->linenum);

		else if (parseTree->kind == Assign)
			printAssign(parseTree->linenum);

		else if (parseTree->kind == If)
			printIf(parseTree->linenum);

		else if (parseTree->kind == Break)
			printBreak(parseTree->linenum);

		else if (parseTree->kind == Call)
			printCall(parseTree->val.id, parseTree->linenum);

		else if (parseTree->kind == Return)
			printReturn(parseTree->linenum);
}

void printType(int type)
{
	switch(type){
		case IntType:
			printf("int");
			return;
		case VoidType:
			printf("void");
			return;
		case CharType:
			printf("char");
			return;
		case BoolType:
			printf("bool");
			return;
		case RecordType:
			printf("record");
			return;
		default:
			return;
	}
}
void printVar(treeNode *parseTree, char *name, int type, int linenum)
{
	printf("Var %s ", name);
	if (parseTree->isArray) {
		printf("is array ");
	}
	printf("of type ");
	printType(type);
	printf(" [line: %d]\n", linenum);
}
void printRec(treeNode *parseTree, char *name, int linenum)
{
	printf("Record %s ", name);
	printf(" [line: %d]\n", linenum);
}
void printFunc(char *name, int type, int linenum)
{
	printf("Func %s returns type ", name);
	printType(type);
	printf(" [line: %d]\n", linenum);
}
void printParam(treeNode *parseTree, char *name, int type, int linenum)
{
	printf("Param %s ", name);
	if (parseTree->isArray) printf("is array ");
	printf("of type ");
	printType(type);
	printf(" [line: %d]\n", linenum);
}
void printCompound(int linenum)
{
	printf("Compound [line: %d]\n", linenum);
}
void printConst(treeNode *parseTree, int type, int linenum)
{
	printf("Const: ");
	switch(type) {
		case IntType:
			printf("%d", parseTree->val.intconst);
			break;
		case CharType:
			printf("%c", parseTree->val.charconst);
			break;
		case BoolType:
			printf("%d", parseTree->val.boolconst);
			break;
		default:
			break;
	}
	printf(" [line: %d]\n", linenum);
}
void printId(char *name, int linenum)
{
	printf("Id: %s [line: %d]\n", name, linenum);
}
void printOp(char *op, int linenum)
{
	printf("Op: %s [line: %d]\n", op, linenum);
}
void printAssign(int linenum)
{
	printf("Assign: = [line: %d]\n", linenum);
}
void printIf(int linenum)
{
	printf("If [line: %d]\n", linenum);
}
void printBreak(int linenum)
{
	printf("Break [line: %d]\n", linenum);
}
void printCall(char *name, int linenum)
{
	printf("Call: %s [line: %d]\n", name, linenum);
}
void printReturn(int linenum)
{
	printf("Return [line: %d]\n", linenum);
}

treeNode *newNode() 
{
	treeNode *node = (treeNode*) calloc (1, sizeof(treeNode));
	node->kind = Incomplete;
	return node;
}




treeNode *makeRecordDeclaration(char* id, treeNode* localDeclarations) 
{
	//printf("In makeRecordDeclaration\n");
	treeNode *n = newNode();
	n->kind = Rec;
	n->val.id = id;
	n->children[0] = localDeclarations;
	return n;
}

treeNode *makeDeclaration(treeNode* declarationList, treeNode* declaration) 
{
	treeNode* t = declarationList;
	if (t != NULL)
	{
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = declaration;
		return declarationList;
	} else {
		return declaration;
	}
}

treeNode *makeLocalDeclaration(treeNode* localDeclarations, treeNode* scopedVarDeclaration) {
	treeNode* t = localDeclarations;
	if (t != NULL)
	{
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = scopedVarDeclaration;
		return localDeclarations;
	} else {
		return scopedVarDeclaration;
	}
	
}

treeNode *makeScopedVarDeclaration(treeNode *scopedTypedSpecifier, treeNode *varDeclList) 
{
	//Iterate over each sibling in varDeclList and set its type to scopedTypeSpecifier
	treeNode *t = varDeclList;
	while (t != NULL)
	{
		t->type = scopedTypedSpecifier->type;
		t = t->sibling;
	}
	return varDeclList;
}

treeNode *addVarDeclarationInitialize(treeNode *varDeclList, treeNode *varDeclInitialize) 
{
	treeNode* t = varDeclList;
	if (t != NULL)
	{
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = varDeclInitialize;
		return varDeclList;
	} else {
		return varDeclInitialize;
	}
}

treeNode *makeVarDeclaration(char* id)
{
	treeNode* t = newNode();
	t->val.id = id;
	t->kind = Var;
	return t;
}

treeNode *makeVarDeclarationId(char* id, int isArray, int arraylength)
{
	treeNode* t = newNode();
	t->val.id = id;
	t->kind = Var;
	if (isArray) {
		t->isArray = isArray;
		t->val.intconst = arraylength;
	}
	return t;
}

treeNode *addSimpleExpressionToVarDeclarationID(treeNode *varDeclId, treeNode *simpleExpression)
{
	varDeclId->children[0] = simpleExpression;
	return varDeclId;
}

treeNode *makeId(char* id, int isArray)
{
	treeNode *n = newNode();
	n->kind = Id;
	n->val.id = id;
	n->isArray = isArray;
	return n;
}

treeNode *makeEquExpression(treeNode* mutable, treeNode* expression)
{
	treeNode *n = newNode();
	n->kind = Assign;
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = Eq;
	return n;
}
treeNode *makeAddEExpression(treeNode* mutable, treeNode* expression)
{
	treeNode *n = newNode();
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = Add;
	return n;
}
treeNode *makeSubEExpression(treeNode* mutable, treeNode* expression)
{
	treeNode *n = newNode();
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = Sub;
	return n;
}
treeNode *makeMulEExpression(treeNode* mutable, treeNode* expression)
{
	treeNode *n = newNode();
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = Mul;
	return n;
}
treeNode *makeDivEExpression(treeNode* mutable, treeNode* expression)
{
	treeNode *n = newNode();
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = Div;
	return n;
}
treeNode *makeIncExpression(treeNode* mutable)
{
	treeNode *n = newNode();
	n->children[0] = mutable;
	n->opType = Inc;
	return n;
}
treeNode *makeDecExpression(treeNode* mutable)
{
	treeNode *n = newNode();
	n->children[0] = mutable;
	n->opType = Dec;
	return n;
}

treeNode *makeBoolConst(int b)
{
	treeNode *n = newNode();
	n->type = BoolType;
	n->kind = Const;
	n->val.boolconst = b;
	return n;
}

treeNode *makeIntConst(int i)
{
	treeNode *n = newNode();
	n->type = IntType;
	n->kind = Const;
	n->val.intconst = i;
	return n;
}

treeNode *makeCharConst(char c)
{
	treeNode *n = newNode();
	n->type = CharType;
	n->kind = Const;
	n->val.charconst = c;
	return n;
}

treeNode *makeSimpleExpression(treeNode *simpleExpression, treeNode *andExpression)
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Or;
	n->children[0] = simpleExpression;
	n->children[1] = andExpression;
	return n;
}

treeNode *makeAndExpression(treeNode *andExpression, treeNode *unaryRelExpression)
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = And;
	n->children[0] = andExpression;
	n->children[1] = unaryRelExpression;
	return n;
}

treeNode *makeNotExpression(treeNode *unaryRelExpression)
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Not;
	n->children[0] = unaryRelExpression;
	return n;
}

treeNode* makeRelExpression(treeNode *sumExpressionl, treeNode *relop, treeNode *sumExpressionr)
{
	treeNode *n = relop;
	n->children[0] = sumExpressionl;
	n->children[1] = sumExpressionr;
	return n;
}

treeNode* makeLEQ()
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Leq;
	return n;
}

treeNode* makeGEQ()
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Geq;
	return n;
}

treeNode* makeLSS()
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Lss;
	return n;
}

treeNode* makeGSS()
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Gss;
	return n;
}

treeNode* makeEQ()
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Eq;
	return n;
}

treeNode* makeNOTEQ()
{
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Noteq;
	return n;
}

treeNode *makeCompound(treeNode *localDeclarations, treeNode *statementList)
{
	treeNode *n = newNode();
	n->kind = Compound;
	n->children[0] = localDeclarations;
	n->children[1] = statementList;
	return n;
}

treeNode *makeReturnStatement(treeNode *expression)
{
	treeNode *n = newNode();
	n->kind = Return;
	n->children[0] = expression;
	return n;
}

treeNode *makeBreakStatement( ) {
	treeNode *n = newNode();
	n->kind = Break;
	return n; 
}

treeNode *makeFuncStatement( treeNode* typeSpecifier, char* id, treeNode* params, treeNode* statement )
{
	treeNode *n = newNode();
	if (typeSpecifier == NULL) {
		n->type = VoidType;
	} else {
		n->type = typeSpecifier->type;
	}
	n->kind = Func;
	n->val.id = id;
	n->children[0] = params;
	n->children[1] = statement;
	return n;
}

treeNode* makeParamList(treeNode* paramList, treeNode* paramTypeList) 
{
	treeNode* t = paramList;
	if (t != NULL)
	{
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = paramTypeList;
		return paramList;
	} else {
		return paramTypeList;
	}
}

treeNode* makeParamTypeList(treeNode* typeSpecifier, treeNode* paramIdList) 
{
	treeNode *t = paramIdList; 
	while (t != NULL)
	{
		t->type = typeSpecifier->type;
		t = t->sibling;
	}
	return paramIdList;
}

treeNode *makeParamIdList(treeNode* paramIdList, treeNode* paramId) 
{
	treeNode* t = paramIdList;
	if (t != NULL)
	{
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = paramId;
		return paramIdList;
	} else {
		return paramId;
	}
}

treeNode *makeParam(char* id, int isArray) 
{
	treeNode *n = newNode();
	n->kind = Param;
	n->val.id = id;
	n->isArray = isArray;
	return n;
}

treeNode *makeIntType()
{
	treeNode *n = newNode();
	n->type = IntType;
	return n;
}

treeNode *makeBoolType()
{
	treeNode *n = newNode();
	n->type = BoolType;
	return n;
} 

treeNode *makeCharType()
{
	treeNode *n = newNode();
	n->type = CharType;
	return n;
} 

treeNode *makeRecordType()
{
	treeNode *n = newNode();
	n->type = RecordType;
	return n;
}

treeNode *makeCall(char *id, treeNode *args) 
{
	treeNode *n = newNode();
	n->type = Call;
	n->val.id = strdup(id);
	n->children[0] = args;
	return n;
}

treeNode *addStatementList(treeNode *statementList, treeNode *statement)
{
	treeNode* t = statementList;
	if (t != NULL)
	{
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = statement;
		return statementList;
	} else {
		return statement;
	}
}

treeNode *makeMatchedStatement( treeNode* simpleExpression, treeNode* matched)
{
	treeNode* n = newNode();
	n->type = If;
	n->children[0] = simpleExpression;
	n->children[1] = matched;
	return n;
}

treeNode *makeUnmatchedStatement( treeNode* simpleExpression, treeNode* matched, treeNode* unmatched)
{
	treeNode* n = newNode();
	n->type = If;
	if (matched == NULL && unmatched != NULL) {
		n->children[0] = simpleExpression;
		n->children[1] = unmatched;
	}
	if (matched != NULL && unmatched == NULL) {
		n->children[0] = simpleExpression;
		n->children[1] = matched;	
	}
	if (matched != NULL && unmatched != NULL) {
		n->children[0] = simpleExpression;
		n->children[1] = matched;
		n->children[2] = unmatched;
	}
	return n;
}

treeNode *makeIterationStatement(treeNode* simpleExpression, treeNode* statement) 
{
	treeNode* t = newNode();
	t->type = While;
	t->children[0] = simpleExpression;
	t->children[1] = statement;
	return t;
}

treeNode *makeMutableID(char *id)
{
	treeNode* n = newNode();
	n->kind = Id;
	n->val.id = id;
	return n;

}

treeNode *makeMutableBracketExpression(treeNode* mutable, treeNode* expression)
{
	mutable->children[0] = expression;
	mutable->isArray = 1;
	return mutable_t; 
}

treeNode *makeMutableDotId(treeNode* mutable, char *id)
{
	mutable->children[0]->val.id = id;
	return mutable_t;
}

<<<<<<< dec84110e2c3cef90c7223cd010aa9bbe85fb37d
treeNode *makeSumExpression(*treeNode sumExpression, *treeNode sumop, *treeNode term) 
{
	treeNode* t = sumop;
	if (t != NULL) {
		t->children[0] = sumExpression;
		t->children[1] = term; 
	} else {
		t = term;
	}
	return t;
}

treeNode *makeAddOp() 
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Add;
	return n;
}

treeNode *makeSubOp()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Sub;
	return n;
}

treeNode *makeTerm(treeNode* term, treeNode* mulop, treeNode* unaryExpression)
{
	treeNode* t = mulop;
	if (t != NULL) {
		t->children[0] = term;
		t->children[1] = unaryExpression; 
	} else {
		t = unaryExpression;
	}
	return t;	
}

treeNode *makeMulOp()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Mul;
	return n;
}

treeNode *makeDivOp()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Div;
	return n;
}


treeNode *makeModOp()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Mod;
	return n;
}

treeNode *makeArgList(treeNode* arglist, treeNode* expression) 
{
	treeNode* t = arglist; 
	if (t != NULL) {
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = arglist;
		t = t->sibling;
		t->sibling = expression;
		return t;
	} else {
		return expression;
	}
}

treeNode *makeImmutable(treeNode *child) {
	treeNode *n = newNode();
	n->children[0] = child;
	return n;
}


treeNode *makeUnaryExpression(treeNode* unaryop, treeNode* unaryExpression)
{
	unaryop->sibling = unaryExpression;
	return unaryop;
}

treeNode *makeSUB()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Sub;
}

treeNode *makeMUL()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Mul;
}

treeNode *makeRAND()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Rand;
}

treeNode *makeNEG()
{
	treeNode* n = newNode();
	n->kind = Op;
	n->opType = Neg;
}