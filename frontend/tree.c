#include "tree.h"
#include "scanType.h"
#include <stdlib.h>

void printTree(FILE *output, TreeNode parseTree) 
{
	if(parseTree.children[0] == 0)
		return;
	else {
		int i = 0;
		int siblingNum = -1;
		int treeLevel = 0;
		while(parseTree.children[i] != 0 && i < MAXCHILDREN) {
			printTreeSibling(output, parseTree.children[i], siblingNum, treeLevel);
			i++;
			siblingNum++;
		}
	}
}

void printTreeSibling(FILE *output, TreeNode parseTree, int siblingNum, int treeLevel) 
{
	int i;
	for (i = 0; i < treeLevel; i++) {
		printf("!\t");
	}

	//Special case for initial print so we don't write Child before it
	if(siblingNum == -1 && treeLevel == 0) {
		printNode(output, parseTree);
	}
	else if(siblingNum)
			
}

/*
 * 
 */
void printNode(FILE *output, TreeNode *parseTree)
{
		if(parseTree->kind == Var)
			printVar(output, parseTree, parseTree->val.id, parseTree->type, parseTree->linenum);

		else if (parseTree.kind == Func)
			printFunc(output, parseTree.val.id, parseTree.type, parseTree.linenum);

		else if (parseTree.kind == Param)
			printParam(output, parseTree.val.id, parseTree.type, parseTree.linenum);

		else if (parseTree.kind == Compound)
			printCompound(output, parseTree.linenum);

		else if (parseTree.kind == Const)
			printConst(output, parseTree, parseTree.linenum);

		else if (parseTree.kind == Id)
			printId(output, parseTree.val.id, parseTree.linenum);

		else if (parseTree.kind == Op)
			printOp(output, parseTree.val.op, parseTree.linenum);

		else if (parseTree.kind == Assign)
			printAssign(output, parseTree.linenum);

		else if (parseTree.kind == If)
			printIf(output, parseTree.linenum);

		else if (parseTree.kind == Break)
			printBreak(output, parseTree.linenum);

		else if (parseTree.kind == Call)
			printCall(output, parseTree.val.id, parseTree.linenum);

		else if (parseTree.kind == Return)
			printReturn(output, parseTree.linenum);
}

void printType(int type)
{
	switch(type){
		case IntType:
			printf("int");
		case VoidType:
		case CharType:
		case BoolType:
		case RecordType:
		default:
			printf(" ");
	}
}
void printVar(FILE *output, TreeNode *parseTree, char *name, int type, int linenum)
{
	printf("Var %s ", name);
	if (parseTree->isArray) {
		printf("is array ");
	}
	printf("of type ");
	printType(type);
	printf(" [line: %d]\n", linenum);

}
void printFunc(FILE *output, char *name, int type, int linenum)
{

}
void printParam(FILE *output, char *name, int type, int linenum)
{

}
void printCompound(FILE *output, int linenum)
{

}
void printConst(FILE *output, TreeNode *parseTree, int type, int linenum)
{

}
void printId(FILE *output, char *name, int linenum)
{

}
void printOp(FILE *output, char *op, int linenum)
{

}
void printAssign(FILE *output, int linenum)
{

}
void printIf(FILE *output, int linenum)
{

}
void printBreak(FILE *output, int linenum)
{

}
void printCall(FILE *output, char *name, int linenum)
{

}
void printReturn(FILE *output, int linenum)
{

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
	n->val.equE.left = left;
	n->val.equE.right = right;
	return n;
}
TreeNode *makeAddEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.addE.left = left;
	n->val.addE.right = right;
	return n;
}
TreeNode *makeSubEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.subE.left = left;
	n->val.subE.right = right;
	return n;
}
TreeNode *makeMulEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.mulE.left = left;
	n->val.mulE.right = right;
	return n;
}
TreeNode *makeDivEExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.divE.left = left;
	n->val.divE.right = right;
	return n;
}
TreeNode *makeIncExpression(TreeNode* left) {
	TreeNode *n = newNode();
	n->val.incE.left = left;
	return n;
}
TreeNode *makeDecExpression(TreeNode* left) {
	TreeNode *n = newNode();
	n->val.decE.left = left;
	return n;
}

TreeNode *makeBoolConst(int b) {
	TreeNode *n = newNode();
	n->kind = Const;
	n->val.boolconst = b;
	return n;
}

TreeNode *makeIntConst(int i) {
	TreeNode *n = newNode();
	n->kind = Const;
	n->val.intconst = i;
	return n;
}

TreeNode *makeCharConst(char c) {
	TreeNode *n = newNode();
	n->kind = Const;
	n->val.charconst = c;
	return n;
}

TreeNode *makeOrExpression(TreeNode *left, TreeNode *right) {
	TreeNode *n = newNode();
	n->kind = Op;
	n->val.or.left = left;
	n->val.or.right = right;
	return n;
}

TreeNode *makeAndExpression(TreeNode *left, TreeNode *right) {
	TreeNode *n = newNode();
	n->kind = Op;
	n->val.and.left = left;
	n->val.and.right = right;
	return n;
}

TreeNode *makeNotExpression(TreeNode *left) {
	TreeNode *n = newNode();
	n->kind = Op;
	n->val.not.left = left;
	return n;
}





