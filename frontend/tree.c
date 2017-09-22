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

	if(siblingNum == -1 && treeLevel == 0)
		if(parseTree.kind == Var)
			printVar(output, parseTree.val.id), parseTree.type;
		else if (parseTree.kind == Func)
			printFunc(output, parseTree.val.id, parseTree.type);
		else if (parseTree.kind == Param)
			printParam(output, parseTree.val.id, parseTree.type);
		else if (parseTree.kind == Compound)
			printCompound(output);
		else if (parseTree.kind == Const)
			printConst(output, parseTree.val.Const);
		else if (parseTree.kind == Id)
			printId(output, parseTree.val.Id);
		else if (parseTree.kind == Op)
			printOp(output, parseTree.val.Op);
		else if (parseTree.kind == Assign)
			printAssign(output, parseTree.val.Assign);
		else if (parseTree.kind == If)
			printIf(output, parseTree.val.If);
		else if (parseTree.kind == Break)
			printBreak(output, parseTree.val.Break);
		else if (parseTree.kind == Call)
			printCall(output, parseTree.val.Call);
		else if (parseTree.kind == Return)
			printReturn(parseTree.kind == parseTree.val.)
}

void printVar(FILE *output, char *name, int type)
{

}

void printFunc(File *output, char *name, int type)
{

}
void printParam(File *output, char *name, int type)
{

}
void printCompound(File *output)
{

}
void printConst(File *output, TreeNode *parseTree, int type)
{

}
void printId(File *output, char *name)
{

}
void printOp(File *output, char *op)
{

}
void printAssign(File *output)
{

}
void printIf(File *output)
{

}
void printBreak(File *output)
{

}
void printCall(File *output, char *name)
{

}
void printReturn(File *output)
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




