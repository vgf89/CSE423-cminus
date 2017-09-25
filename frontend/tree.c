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
		treeNode *root = parseTree;//->children[0];

		//Special case for printing of root so we don't write "Child" before it
		printNode(root);
		
		while(root->children[childNum + 1] != NULL) {
			childNum++;
			printSubTree(root->children[childNum], siblingNum, childNum, treeLevel + 1);
		}

		childNum = -1;
		if(root->sibling != NULL) {
			printSubTree(root->sibling, siblingNum + 1, childNum, treeLevel);
		}
	}
}

void printSubTree(treeNode *curNode, int siblingNum, int childNum, int treeLevel) 
{
	for (int i = 0; i < treeLevel; i++) {
		printf("!   ");
	}

	if(childNum > -1) {
		printf("Child: %d  ", childNum);
		printNode(curNode);
		siblingNum = -1;
	}
	else {
		printf("Sibling: %d  ", siblingNum);
		printNode(curNode);
	}

	childNum = -1;
	while(curNode->children[childNum + 1] != NULL) {
		//we only want to increase tree depth for the first child of a new depth
		if(childNum == 0) {
			treeLevel++;
		}
		childNum++;
		printSubTree(curNode->children[childNum], siblingNum, childNum, treeLevel + 1);
	}
	if(curNode->sibling != NULL) {
		printSubTree(curNode->sibling, siblingNum + 1, childNum, treeLevel);
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
/*
        NCT numType;
        CCT charType;
        BCT boolType;
        RECT rectType;
        IDT idType;
        KWT keywordType;
*/

treeNode *newNode() {
	treeNode *node = (treeNode*) calloc (1, sizeof(treeNode));
	node->kind = Incomplete;
	return node;
}




treeNode *makeRecordDeclaration(char* id, treeNode* localDeclarations) {
	//printf("In makeRecordDeclaration\n");
	treeNode *n = newNode();
	n->kind = Rec;
	n->val.id = id;
	n->children[0] = localDeclarations;
	return n;
}

treeNode *makeDeclaration(treeNode* declarationList, treeNode* declaration) {
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

treeNode *makeScopedVarDeclaration(treeNode *scopedTypedSpecifier, treeNode *varDeclList) {
	//Iterate over each sibling in varDeclList and set its type to scopedTypeSpecifier
	treeNode *t = varDeclList;
	while (t != NULL)
	{
		t->type = scopedTypedSpecifier->type;
		t = t->sibling;
	}
	return varDeclList;
}

treeNode *addVarDeclarationInitialize(treeNode *varDeclList, treeNode *varDeclInitialize) {
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





treeNode *makeId(char* id, int isArray) {
	treeNode *n = newNode();
	n->kind = Id;
	n->val.id = id;
	n->isArray = isArray;
	return n;
}

treeNode *makeEquExpression(treeNode* left, treeNode* right) {
	treeNode *n = newNode();
	n->kind = Assign;
	n->val.equE.left = left;
	n->val.equE.right = right;
	n->opType = Eq;
	return n;
}
treeNode *makeAddEExpression(treeNode* left, treeNode* right) {
	treeNode *n = newNode();
	n->val.addE.left = left;
	n->val.addE.right = right;
	n->opType = Add;
	return n;
}
treeNode *makeSubEExpression(treeNode* left, treeNode* right) {
	treeNode *n = newNode();
	n->val.subE.left = left;
	n->val.subE.right = right;
	n->opType = Sub;
	return n;
}
treeNode *makeMulEExpression(treeNode* left, treeNode* right) {
	treeNode *n = newNode();
	n->val.mulE.left = left;
	n->val.mulE.right = right;
	n->opType = Mul;
	return n;
}
treeNode *makeDivEExpression(treeNode* left, treeNode* right) {
	treeNode *n = newNode();
	n->val.divE.left = left;
	n->val.divE.right = right;
	n->opType = Div;
	return n;
}
treeNode *makeIncExpression(treeNode* left) {
	treeNode *n = newNode();
	n->val.incE.left = left;
	n->opType = Inc;
	return n;
}
treeNode *makeDecExpression(treeNode* left) {
	treeNode *n = newNode();
	n->val.decE.left = left;
	n->opType = Dec;
	return n;
}

treeNode *makeBoolConst(int b) {
	treeNode *n = newNode();
	n->type = BoolType;
	n->kind = Const;
	n->val.boolconst = b;
	return n;
}

treeNode *makeIntConst(int i) {
	treeNode *n = newNode();
	n->type = IntType;
	n->kind = Const;
	n->val.intconst = i;
	return n;
}

treeNode *makeCharConst(char c) {
	treeNode *n = newNode();
	n->type = CharType;
	n->kind = Const;
	n->val.charconst = c;
	return n;
}

treeNode *makeOrExpression(treeNode *left, treeNode *right) {
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Or;
	n->val.or.left = left;
	n->val.or.right = right;
	return n;
}

treeNode *makeAndExpression(treeNode *left, treeNode *right) {
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = And;
	n->val.and.left = left;
	n->val.and.right = right;
	return n;
}

treeNode *makeNotExpression(treeNode *left) {
	treeNode *n = newNode();
	n->kind = Op;
	n->opType = Not;
	n->val.not.left = left;
	return n;
}

treeNode *makeCompound(treeNode *left, treeNode *right) {
	treeNode *n = newNode();
	n->kind = Compound;
	n->val.compound.left = left;
	n->val.compound.right = right;
	return n;
}

treeNode *makeReturnStatement(treeNode *expression) {
	treeNode *n = newNode();
	n->kind = Return;
	n->val.returnStatement.expression = expression;
	return n;
}

treeNode *makeBreakStatement( ) {
	treeNode *n = newNode();
	n->kind = Break;
	return n; 
}

treeNode *makeFuncStatement( treeNode* typeSpecifier, char* id, treeNode* params, treeNode* statement ) {
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

treeNode* makeParamList(treeNode* paramList, treeNode* paramTypeList) {
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

treeNode* makeParamTypeList(treeNode* typeSpecifier, treeNode* paramIdList) {
	treeNode *t = paramIdList; 
	while (t != NULL)
	{
		t->type = typeSpecifier->type;
		t = t->sibling;
	}
	return paramIdList;
}

treeNode *makeParamIdList(treeNode* paramIdList, treeNode* paramId) {
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

treeNode *makeParam(char* id, int isArray) {
	treeNode *n = newNode();
	n->kind = Param;
	n->val.id = id;
	n->isArray = isArray;
	return n;
}

treeNode *makeIntType() {
	treeNode *n = newNode();
	n->type = IntType;
	return n;
}

treeNode *makeBoolType() {
	treeNode *n = newNode();
	n->type = BoolType;
	return n;
} 

treeNode *makeCharType() {
	treeNode *n = newNode();
	n->type = CharType;
	return n;
} 

treeNode *makeRecordType() {
	treeNode *n = newNode();
	n->type = RecordType;
	return n;
}

treeNode *makeCall(treeNode *id, treeNode *args) {
	treeNode *n = newNode();
	n->type = Call;
	n->val.id = id->val.id;
	return n;
}




