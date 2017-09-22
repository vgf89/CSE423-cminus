#include <stdio.h>
#define MAXCHILDREN 2048

typedef struct treeNode {
    //connecivity in the tree

    struct treeNode *children[MAXCHILDREN]; //children of the node

    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    //NodeKind nodeKind;       //type of node

    //extra properties about the node depending on type of the node

    enum {Var, Func, Param, Compound, Const, Id, Op, Assign, If, Break, Call, Return} kind;

    enum {IntType, VoidType, CharType, BoolType, RecordType} type;

    union {                   //relevent data to type -> attr
        char* id;
        char* op;
        int boolconst;
        int intconst;
        char charconst;
        struct { struct treeNode *left, *right; } equE;
        struct { struct treeNode *left, *right; } addE;
        struct { struct treeNode *left, *right; } subE;
        struct { struct treeNode *left, *right; } mulE;
        struct { struct treeNode *left, *right; } divE;
        struct { struct treeNode *left; } incE;
        struct { struct treeNode *left; } decE;
    } val;

    //ExpType expType;      //used when ExpK for type checking

    int linenum;

    int isArray;         //is this an array
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} TreeNode;

void printTree(FILE *output, TreeNode parseTree);
void printTreeSibling(FILE *output, TreeNode parseTree, int siblingNum, int treeLevel);
void printVar(FILE *output, char *name, int type);
void printFunc(FILE *output, char *name, int type);
void printParam(FILE *output, char *name, int type);
void printCompound(FILE *output);
void printConst(FILE *output, TreeNode *parseTree, int type);
void printId(FILE *output, char *name);
void printOp(FILE *output, char *op);
void printAssign(FILE *output);
void printIf(FILE *output);
void printBreak(FILE *output);
void printCall(FILE *output, char *name);
void printReturn(FILE *output);


TreeNode *newNode();

TreeNode *makeID(char* ID);
TreeNode *makeEquExpression(TreeNode* left, TreeNode* right);
TreeNode *makeAddEExpression(TreeNode* left, TreeNode* right);
TreeNode *makeSubEExpression(TreeNode* left, TreeNode* right);
TreeNode *makeMulEExpression(TreeNode* left, TreeNode* right);
TreeNode *makeDivEExpression(TreeNode* left, TreeNode* right);
TreeNode *makeIncExpression(TreeNode* left);
TreeNode *makeDecExpression(TreeNode* left);

TreeNode *makeintConst(int i);
TreeNode *makeboolConst(int b);
TreeNode *makeCharConst(char c);


