#include "tree.h"
#include "scanType.h"
#include <stdlib.h>


void printTree(TreeNode *parseTree) 
{
	//If sentinal node has no children the program is empty, else it has one child which is the true root.
	if(parseTree->children[0] == NULL) {
		printf("Tree is empty.\n");
		return;
	}
	else {	
		int siblingNum = -1;
		int childNum = -1;
		int treeLevel = 0;
		TreeNode *root = parseTree->children[0];

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

void printSubTree(TreeNode *curNode, int siblingNum, int childNum, int treeLevel) 
{
	for (int i = 0; i < treeLevel; i++) {
		printf("!\t");
	}

	if(childNum > -1 && siblingNum == -1) {
		printf("Child: %d  ", childNum);
		printNode(curNode);
		siblingNum = -1;
	}
	else {
		printf("Sibling: %d  ", siblingNum);
		printNode(curNode);
		childNum = -1;
	}
	while(curNode->children[childNum + 1] != NULL) {
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
void printNode(TreeNode *parseTree)
{
		if(parseTree->kind == Var)
			printVar(parseTree, parseTree->val.id, parseTree->type, parseTree->linenum);

		else if (parseTree->kind == Func)
			printFunc(parseTree->val.id, parseTree->type, parseTree->linenum);

		else if (parseTree->kind == Param)
			printParam(parseTree->val.id, parseTree->type, parseTree->linenum);

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
void printVar(TreeNode *parseTree, char *name, int type, int linenum)
{
	printf("Var %s ", name);
	if (parseTree->isArray) {
		printf("is array ");
	}
	printf("of type ");
	printType(type);
	printf(" [line: %d]\n", linenum);
}
void printFunc(char *name, int type, int linenum)
{
	printf("Func %s returns type ", name);
	printType(type);
	printf(" [line: %d]\n", linenum);
}
void printParam(char *name, int type, int linenum)
{
	printf("Param %s of type ", name);
	printType(type);
	printf(" [line: %d]\n", linenum);
}
void printCompound(int linenum)
{
	printf("Compound [line: %d]\n", linenum);
}
void printConst(TreeNode *parseTree, int type, int linenum)
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

TreeNode *newNode() {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	return node;
}

TreeNode *makeID(char* ID) {
	TreeNode *n = newNode();
	n->kind = Id;
	n->val.id = ID;
	return n;
}

TreeNode *makeEquExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->kind = Assign;
	n->val.equE.left = left;
	n->val.equE.right = right;
	n->opType = Eq;
	return n;
}
TreeNode *makeAddEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.addE.left = left;
	n->val.addE.right = right;
	n->opType = Add;
	return n;
}
TreeNode *makeSubEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.subE.left = left;
	n->val.subE.right = right;
	n->opType = Sub;
	return n;
}
TreeNode *makeMulEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.mulE.left = left;
	n->val.mulE.right = right;
	n->opType = Mul;
	return n;
}
TreeNode *makeDivEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.divE.left = left;
	n->val.divE.right = right;
	n->opType = Div;
	return n;
}
TreeNode *makeIncExpression(TreeNode* left) {
	TreeNode *n = newNode();
	n->val.incE.left = left;
	n->opType = Inc;
	return n;
}
TreeNode *makeDecExpression(TreeNode* left) {
	TreeNode *n = newNode();
	n->val.decE.left = left;
	n->opType = Dec;
	return n;
}

TreeNode *makeBoolConst(int b) {
	TreeNode *n = newNode();
	n->type = BoolType;
	n->kind = Const;
	n->val.boolconst = b;
	return n;
}

TreeNode *makeIntConst(int i) {
	TreeNode *n = newNode();
	n->type = IntType;
	n->kind = Const;
	n->val.intconst = i;
	return n;
}

TreeNode *makeCharConst(char c) {
	TreeNode *n = newNode();
	n->type = CharType;
	n->kind = Const;
	n->val.charconst = c;
	return n;
}

TreeNode *makeOrExpression(TreeNode *left, TreeNode *right) {
	TreeNode *n = newNode();
	n->kind = Op;
	n->opType = Or;
	n->val.or.left = left;
	n->val.or.right = right;
	return n;
}

TreeNode *makeAndExpression(TreeNode *left, TreeNode *right) {
	TreeNode *n = newNode();
	n->kind = Op;
	n->opType = And;
	n->val.and.left = left;
	n->val.and.right = right;
	return n;
}

TreeNode *makeNotExpression(TreeNode *left) {
	TreeNode *n = newNode();
	n->kind = Op;
	n->opType = Not;
	n->val.not.left = left;
	return n;
}

TreeNode *makeCompound(TreeNode *left, TreeNode *right) {
	TreeNode *n = newNode();
	n->kind = compound;
	n->val.compound.left = left;
	n->val.compound.right = right;
	return n;
}

TreeNode *makeReturnStatement(TreeNode *expression) {
	TreeNode *n = newNode();
	n->kind = Return;
	n->val.returnStatement.expression = expression;
	return n;
}

TreeNode *makeBreakStatement( ) {
	TreeNode *n = newNode();
	n->kind = Break;
	return n; 
}

TreeNode *makeFuncStatement(TreeNode *typeSpecifier, TreeNode *Id, TreeNode *parameterList, TreeNode *statement) {
	TreeNode *n = newNode();
	n->kind = Func;
	n->val.Func.typeSpecifier = typeSpecifier;
	n->val.Func.Id = Id;
	n->val.Func.parameterList = parameterList;
	n->val.Func.statement = statement;
	n->type = typeSpecifier->type;
	return n;
}

TreeNode *makeIntType() {
	TreeNode *n = newNode();
	n->type = IntType;
	return n;
}

TreeNode *makeBoolType() {
	TreeNode *n = newNode();
	n->type = BoolType;
	return n;
} 

TreeNode *makeCharType() {
	TreeNode *n = newNode();
	n->type = CharType;
	return n;
} 

TreeNode *makeRecordType() {
	TreeNode *n = newNode();
	n->type = RecordType;
	return n;
}