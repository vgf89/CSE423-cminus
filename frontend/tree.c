#include "tree.h"
#include "scanType.h"
#include <stdlib.h>


/*
 *  prints entire tree or "Tree is empty" if it's empty
 */
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
		while (i < 3 || parseTree->children[i] != NULL) {
			if (parseTree->children[i] != NULL)
				printSubTree(parseTree->children[i], -1, i, treeLevel + 1);
			i++;
		}
		if (parseTree->sibling != NULL) {
			printSubTree(parseTree->sibling, 0, -1, treeLevel);
		}
	}
}

/*
 *  Prints all sub-trees below primary node
 */
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
 * Decides which kind of node needs to be printed
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
			printOp(parseTree, parseTree->linenum);

		else if (parseTree->kind == Assign)
			printAssign(parseTree, parseTree->linenum);

		else if (parseTree->kind == If)
			printIf(parseTree->linenum);

		else if (parseTree->kind == While)
			printWhile(parseTree->linenum);
		
		else if (parseTree->kind == Break)
			printBreak(parseTree->linenum);

		else if (parseTree->kind == Call)
			printCall(parseTree->val.id, parseTree->linenum);

		else if (parseTree->kind == Return)
			printReturn(parseTree->linenum);
}

/*
 * Utility function for printing types when necessary
 */
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

/*
 * Prints variabe declerations
 */
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

/*
 * Prints record declerations
 */
void printRec(treeNode *parseTree, char *name, int linenum)
{
	printf("Record %s ", name);
	printf(" [line: %d]\n", linenum);
}

/*
 * Prints function declerations
 */
void printFunc(char *name, int type, int linenum)
{
	printf("Func %s returns type ", name);
	printType(type);
	printf(" [line: %d]\n", linenum);
}

/*
 * Prints parameter declerations
 */
void printParam(treeNode *parseTree, char *name, int type, int linenum)
{
	printf("Param %s ", name);
	if (parseTree->isArray) printf("is array ");
	printf("of type ");
	printType(type);
	printf(" [line: %d]\n", linenum);
}

/*
 * Prints when compound statement is found
 */
void printCompound(int linenum)
{
	printf("Compound [line: %d]\n", linenum);
}

/*
 * Prints constants
 */
void printConst(treeNode *parseTree, int type, int linenum)
{
	printf("Const: ");
	switch(type) {
		case IntType:
			printf("%d", parseTree->val.intconst);
			break;
		case CharType:
			printf("'%c'", parseTree->val.charconst);
			break;
		case BoolType:
			switch(parseTree->val.boolconst) {
				case 1:
					printf("true");
					break;
				case 0:
					printf("false");
					break;
			}
			break;
		default:
			break;
	}
	printf(" [line: %d]\n", linenum);
}

/*
 * Prints ids
 */
void printId(char *name, int linenum)
{
	printf("Id: %s [line: %d]\n", name, linenum);
}

/*
 * Prints operators
 */
void printOp(treeNode *parseTree, int linenum)
{
	printf("Op: ");
	switch (parseTree->opType) {
		case Or:
			printf("or");
			break;
		case And:
			printf("and");
			break;
		case Not:
			printf("not");
			break;
		case Noteq:
			printf("!=");
			break;
		case EEq:
			printf("==");
			break;
		case Leq:
			printf("<=");
			break;
		case Geq:
			printf(">=");
			break;
		case Lss:
			printf("<");
			break;
		case Gss:
			printf(">");
			break;
		case Add:
			printf("+");
			break;
		case Sub:
			printf("-");
			break;
		case Mul:
			printf("*");
			break;
		case Div:
			printf("/");
			break;
		case Mod:
			printf("%%");
			break;
		case Bracl:
			printf("[");
			break;
		case Rand:
			printf("?");
			break;
		case Dot:
			printf(".");
			break;
		default:
			break;
	}
	printf(" [line: %d]\n", linenum);

}

/*
 * Prints assignments
 */
void printAssign(treeNode *parseTree, int linenum)
{
	printf("Assign: ");
	switch (parseTree->opType) {
		case Eq:
			printf("=");
			break;
		case AddE:
			printf("+=");
			break;
		case SubE:
			printf("-=");
			break;
		case MulE:
			printf("*=");
			break;
		case DivE:
			printf("/=");
			break;
		case Inc:
			printf("++");
			break;
		case Dec:
			printf("--");
			break;
		default:
			break;
	}
	printf(" [line: %d]\n", linenum);
}

/*
 * Prints if statements
 */
void printIf(int linenum)
{
	printf("If [line: %d]\n", linenum);
}

/*
 * Prints while statements
 */
void printWhile(int linenum)
{
	printf("While [line: %d]\n", linenum);
}

/*
 * Prints break statements
 */
void printBreak(int linenum)
{
	printf("Break [line: %d]\n", linenum);
}

/*
 * Prints function call statements
 */
void printCall(char *name, int linenum)
{
	printf("Call: %s [line: %d]\n", name, linenum);
}

/*
 * Prints return statements
 */
void printReturn(int linenum)
{
	printf("Return [line: %d]\n", linenum);
}

/*
 * allocates memories for new tree nodes
 */
treeNode *newNode(int linenum)
{
	treeNode *node = (treeNode*) calloc (1, sizeof(treeNode));
	node->linenum = linenum;
	return node;
}


/*
 * makes a new node for record decleration
 */
treeNode *makeRecordDeclaration(char* id, treeNode* localDeclarations, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Rec;
	n->val.id = id;
	n->children[0] = localDeclarations;
	return n;
}

/*
 * sets sibling pointers of decleration list nodes
 */
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

/*
 * sets sibling pointers of local decleration list nodes
 */
treeNode *makeLocalDeclaration(treeNode* localDeclarations, treeNode* scopedVarDeclaration)
{
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

/*
 * sets sibling pointers of scoped decleration list nodes
 */
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

/*
 * sets sibling pointers of var decleration nodes
 */
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

/*
 * creates a new var decleration node
 */
treeNode *makeVarDeclaration(treeNode* typeSpecifier, treeNode* varDeclList)
{
	treeNode* t = varDeclList;
	while (t != NULL)
	{
		t->type = typeSpecifier->type;
		t = t->sibling;
	}
	return varDeclList;
}

/*
 * creates a new var decleration ID node
 */
treeNode *makeVarDeclarationId(char* id, int isArray, int arraylength, int linenum)
{
	treeNode* t = newNode(linenum);
	t->val.id = id;
	t->kind = Var;
	if (isArray) {
		t->isArray = isArray;
		t->arrayLength = arraylength;
	}
	return t;
}

/*
 * creates a new var decleration node
 */
treeNode *addSimpleExpressionToVarDeclarationID(treeNode *varDeclId, treeNode *simpleExpression)
{
	varDeclId->children[0] = simpleExpression;
	return varDeclId;
}

/*
 * Utility function for making id nodes
 */
treeNode *makeId(char* id, int isArray, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Id;
	n->val.id = id;
	n->isArray = isArray;
	return n;
}

/*
 * creates a new assignment expression node
 */
treeNode *makeEquExpression(treeNode* mutable, treeNode* expression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = Eq;
	return n;
}

/*
 * makes a new assignment expression node for += which will add to the tree
 */
treeNode *makeAddEExpression(treeNode* mutable, treeNode* expression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = AddE;
	return n;
}

/*
 * makes a new assignment expression node for -= which will add to the tree
 */
treeNode *makeSubEExpression(treeNode* mutable, treeNode* expression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = SubE;
	return n;
}

/*
 * makes a new assignment expression node for *= which will add to the tree
 */
treeNode *makeMulEExpression(treeNode* mutable, treeNode* expression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = MulE;
	return n;
}

/*
 * makes a new assignment expression node for /= which will add to the tree
 */
treeNode *makeDivEExpression(treeNode* mutable, treeNode* expression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->children[1] = expression;
	n->opType = DivE;
	return n;
}

/*
 * makes a new assignment expression node for ++ which will add to the tree
 */
treeNode *makeIncExpression(treeNode* mutable, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->opType = Inc;
	return n;
}

/*
 * makes a new assignment expression node for -- which will add to the tree
 */
treeNode *makeDecExpression(treeNode* mutable, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Assign;
	n->children[0] = mutable;
	n->opType = Dec;
	return n;
}

/*
 * makes a new constant node for boolean
 */
treeNode *makeBoolConst(int b, int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = BoolType;
	n->kind = Const;
	n->val.boolconst = b;
	return n;
}

/*
 * makes a new constant node for integers
 */
treeNode *makeIntConst(int i, int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = IntType;
	n->kind = Const;
	n->val.intconst = i;
	return n;
}

/*
 * makes a new constant node for characters
 */
treeNode *makeCharConst(char c, int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = CharType;
	n->kind = Const;
	n->val.charconst = c;
	return n;
}

/*
 * makes a simple expression for the tree 
 */
treeNode *makeSimpleExpression(treeNode *simpleExpression, treeNode *andExpression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Or;
	n->children[0] = simpleExpression;
	n->children[1] = andExpression;
	return n;
}

/*
 * makes a new operator node using AND for the tree 
 */
treeNode *makeAndExpression(treeNode *andExpression, treeNode *unaryRelExpression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = And;
	n->children[0] = andExpression;
	n->children[1] = unaryRelExpression;
	return n;
}

/*
 * makes a new operator node using OR for the tree 
 */
treeNode *makeNotExpression(treeNode *unaryRelExpression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Not;
	n->children[0] = unaryRelExpression;
	return n;
}

/*
 * adds children to relop treenode 
 */
treeNode* makeRelExpression(treeNode *sumExpressionl, treeNode *relop, treeNode *sumExpressionr)
{
	treeNode *n = relop;
	n->children[0] = sumExpressionl;
	n->children[1] = sumExpressionr;
	return n;
}

/*
 * makes a new operator node using <= for the tree 
 */
treeNode* makeLEQ(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Leq;
	return n;
}

/*
 * makes a new operator node using >= for the tree 
 */
treeNode* makeGEQ(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Geq;
	return n;
}

/*
 * makes a new operator node using < for the tree 
 */
treeNode* makeLSS(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Lss;
	return n;
}

/*
 * makes a new operator node using > for the tree 
 */
treeNode* makeGSS(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Gss;
	return n;
}

/*
 * makes a new operator node using EQ for the tree 
 */
treeNode* makeEQ(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = EEq;
	return n;
}

/*
 * makes a new operator node using NOTEQ for the tree 
 */
treeNode* makeNOTEQ(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Op;
	n->opType = Noteq;
	return n;
}

/*
 * makes a new compound statements for the tree 
 */
treeNode *makeCompound(treeNode *localDeclarations, treeNode *statementList, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Compound;
	n->children[0] = localDeclarations;
	n->children[1] = statementList;
	return n;
}

/*
 * makes a return statement node and adds it to tree
 */
treeNode *makeReturnStatement(treeNode *expression, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Return;
	n->children[0] = expression;
	return n;
}

/*
 * makes a break statement node and adds it to tree
 */
treeNode *makeBreakStatement(int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Break;
	return n; 
}

/*
 * makes a func statement node and adds it to tree
 */
treeNode *makeFuncStatement( treeNode* typeSpecifier, char* id, treeNode* params, treeNode* statement , int linenum)
{
	treeNode *n = newNode(linenum);
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

/*
 * adds parameters to paramlist sibling 
 */
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

/*
 * Sets type for paramTypeList
 */
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

/*
 * Add paramId to end of paramId lsit
 */
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

/*
 * make param node
 */
treeNode *makeParam(char* id, int isArray, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Param;
	n->val.id = id;
	n->isArray = isArray;
	return n;
}

/*
 * make int node
 */
treeNode *makeIntType(int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = IntType;
	return n;
}

/*
 * make bool type 
 */
treeNode *makeBoolType(int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = BoolType;
	return n;
} 

/*
 * make char type
 */
treeNode *makeCharType(int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = CharType;
	return n;
} 

/*
 * make record type
 */
treeNode *makeRecordType(int linenum)
{
	treeNode *n = newNode(linenum);
	n->type = RecordType;
	return n;
}

/*
 * make call node
 */
treeNode *makeCall(char *id, treeNode *args, int linenum)
{
	treeNode *n = newNode(linenum);
	n->kind = Call;
	n->val.id = id;
	n->children[0] = args;
	return n;
}

/*
 * add statement to end of statement list
 */
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

/*
 * creates a matches statement such as "expr 'op' expr"
 */
treeNode *makeMatchedStatement(treeNode* simpleExpression, treeNode* matched1, treeNode* matched2, int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = If;
	n->children[0] = simpleExpression;
	n->children[1] = matched1;
	n->children[2] = matched2;
	return n;
}

/*
 * handles unmatched statements. Also protects against dangling else
 */
treeNode *makeUnmatchedStatement( treeNode* simpleExpression, treeNode* matched, treeNode* unmatched, int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = If;
	if (matched == NULL && unmatched != NULL) {
		n->children[0] = simpleExpression;
		n->children[1] = unmatched;
	}
	else if (matched != NULL && unmatched == NULL) {
		n->children[0] = simpleExpression;
		n->children[1] = matched;	
	}
	else if (matched != NULL && unmatched != NULL) {
		n->children[0] = simpleExpression;
		n->children[1] = matched;
		n->children[2] = unmatched;
	}
	else if (matched == NULL && unmatched == NULL) {
		n->children[0] = simpleExpression;
	}
	return n;
}

/*
 * makes and iteration statement node
 */
treeNode *makeIterationStatement(treeNode* simpleExpression, treeNode* statement, int linenum)
{
	treeNode* t = newNode(linenum);
	t->kind = While;
	t->children[0] = simpleExpression;
	t->children[1] = statement;
	return t;
}

/*
 * creates a mutable ID node
 */
treeNode *makeMutableID(char *id, int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Id;
	n->val.id = id;
	return n;

}

/*
 * Makes a new array expression using the [ operator 
 */
treeNode *makeMutableBracketExpression(treeNode* mutable, treeNode* expression, int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Bracl;

	n->children[0] = mutable;
	n->children[1] = expression;
	//mutable->isArray = 1;
	return n; 
}

treeNode *makeMutableDotId(treeNode* mutable, char *id, int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Dot;
	n->children[0] = mutable;
	n->children[1] = makeId(id, 0, linenum);
	return n;
}

/*
 * makes a sum expression and sets the two children on the left and right side 
 */
treeNode *makeSumExpression(treeNode* sumExpression, treeNode* sumop, treeNode* term)
{
	treeNode* t = sumop;
	t->children[0] = sumExpression;
	t->children[1] = term;
	return t;
}

/*
 * makes a new operator node using addition for the tree 
 */
treeNode *makeAddOp(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Add;
	return n;
}

/*
 * makes a new operator node using subtraction for the tree 
 */
treeNode *makeSubOp(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Sub;
	return n;
}

/*
 * makes a mul operation and adds its children 
 */
treeNode *makeTerm(treeNode* term, treeNode* mulop, treeNode* unaryExpression)
{
	treeNode* t = mulop;
	t->children[0] = term;
	t->children[1] = unaryExpression; 
	return t;	
}

/*
 * Makes a muliplication operator node on the tree 
 */
treeNode *makeMulOp(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Mul;
	return n;
}

/*
 * Makes a divide operator node on the tree 
 */
treeNode *makeDivOp(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Div;
	return n;
}

/*
 * makes a modular operator node on the tree 
 */
treeNode *makeModOp(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Mod;
	return n;
}

/*
 * makes an arg list and sets its siblings on the tree 
 */
treeNode *makeArgList(treeNode* arglist, treeNode* expression)
{
	treeNode* t = arglist; 
	if (t != NULL) {
		while (t->sibling != NULL) {
			t = t->sibling;
		}
		t->sibling = expression;
		return arglist;
	} else {
		return expression;
	}
}

/*
 * Makes an unary expression on the tree 
 */
treeNode *makeUnaryExpression(treeNode* unaryop, treeNode* unaryExpression)
{
	unaryop->children[0] = unaryExpression;
	return unaryop;
}

/*
 * Makes a subtraction operator node on the tree
 */
treeNode *makeSUB(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Sub;
	return n;
}

/*
 * Makes a multiply operator node on the tree
 */
treeNode *makeMUL(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Mul;
	return n;
}

/*
 * Makes a ? operator node on the tree
 */
treeNode *makeRAND(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Rand;
	return n;
}

/*
 * Makes a negative operator node on the tree 
 */
treeNode *makeNEG(int linenum)
{
	treeNode* n = newNode(linenum);
	n->kind = Op;
	n->opType = Neg;
	return n;
}
