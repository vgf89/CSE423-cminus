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

    enum {OR, AND, NOT, LEQ, GEQ, LSS, GSS, EQ, NOTEQ, ADD, SUB, MUL, DIV, MOD, RAND, NEG} opType;

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
        struct { struct treeNode *left, *right; } and;
        struct { struct treeNode *left, *right; } or;
        struct { struct treeNode *left; } not;
    } val;

    //ExpType expType;      //used when ExpK for type checking

    int linenum;

    int isArray;         //is this an array
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} TreeNode;

char* getType(int type);
void printTree(FILE *output, TreeNode parseTree);
void printTreeSibling(FILE *output, TreeNode *parseTree, int siblingNum, int treeLevel);
void printNode(FILE *output, TreeNode *parseTree);
void printVar(FILE *output, TreeNode *parseTree, char *name, int type, int linenum);
void printFunc(FILE *output, char *name, int type, int linenum);
void printParam(FILE *output, char *name, int type, int linenum);
void printCompound(FILE *output, int linenum);
void printConst(FILE *output, TreeNode *parseTree, int type, int linenum);
void printId(FILE *output, char *name, int linenum);
void printOp(FILE *output, char *op, int linenum);
void printAssign(FILE *output, int linenum);
void printIf(FILE *output, int linenum);
void printBreak(FILE *output, int linenum);
void printCall(FILE *output, char *name, int linenum);
void printReturn(FILE *output, int linenum);


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


