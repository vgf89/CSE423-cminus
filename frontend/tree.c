void printTree(FILE *output, TreeNode parseTree) 
{
	
}

void printTree(FILE *output, TreeNode parseTree, int siblingNum) 
{
	//print tree here   
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
	TreeNode *node = (TreeNode*) malloc (sizeof(TreeNode));
	return node;
}

TreeNode *makeID(char* ID) {
	TreeNode *n = newNode();
	n->kind = idType;
	n->val.ID = ID;
	return n;
}

TreeNode *makeExpression(TreeNode* left, TreeNode* right) {
	TreeNode *n = newNode();
	n->val.exp.left = left;
	n->val.exp.right = right;
	return n;
}