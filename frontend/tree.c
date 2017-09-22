#include "tree.h"
#include "scanType.h"

void printTree(FILE *output, TreeNode parseTree) 
{
	if(parseTree.children[0] == 0)
		return;
	else {
		int i = 0;
		int siblingNum = -1;
		int treeLevel = 0;
		while(children[i] != 0 && i < MAXCHILDREN) {
			printTreeSibling(output, parseTree.children[i], siblingNum, treeLevel);
			i++;
			siblingNum++;
		}
	}
}

void printTreeSibling(FILE *output, TreeNode parseTree, int siblingNum, int treeLevel) 
{
	for (int i = 0; i < treeLevel; i++) {
		printf("!\t");
	}

	if(siblingNum == -1 && treeLevel == 0)
		if(parseTree.kind == Var)
			printVar(output, parseTree.val.id);
		else if(parseTree.kind == )

		else if(parseTree.kind == )

		else if(parseTree.kind == )
			
		else if(parseTree.kind == )
}

void printVar(FILE *output, char *name)
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
	n->val.ID = ID;
	return n;
}

TreeNode *makeEquExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.exp.left = left;
	n->val.exp.right = right;
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





