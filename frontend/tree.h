#define MAXCHILDREN 2048

typedef struct treeNode {
    //connecivity in the tree

    struct treeNode *child[MAXCHILDREN]; //children of the node

    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    //NodeKind nodeKind;       //type of node

    //extra properties about the node depending on type of the node

    enum {Var, Func, Param, Compound, Const, Id, Op, Assign, If, Break, Call, Return} kind;

    enum {IntType, VoidType, CharType, BoolType, RecordType} type;

    union {                   //relevent data to type -> attr
        char* id;
        int boolconst;
        int intconst;
        char charconst;
        struct {treeNode *left, *right} equE;
        struct {treeNode *left, *right} addE;
        struct {treeNode *left, *right} subE;
        struct {treeNode *left, *right} mulE;
        struct {treeNode *left, *right} divE;
        struct {treeNode *left} incE;
        struct {treeNode *left} decE;
    } val;

    //ExpType expType;      //used when ExpK for type checking

    int linenum;

    int isArray;         //is this an array
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} TreeNode;

void printTree(FILE *output, TreeNode parseTree);

TreeNode *newNode();

TreeNode *makeID(char* ID);
TreeNode *makeEquExpression(TreeNode* left, TreeNode* right);
TreeNode *makeAddExpression(TreeNode* left, TreeNode* right);
TreeNode *makeSubExpression(TreeNode* left, TreeNode* right);
TreeNode *makeMulExpression(TreeNode* left, TreeNode* right);
TreeNode *makeDivExpression(TreeNode* left, TreeNode* right);
TreeNode *makeIncExpression(TreeNode* left);
TreeNode *makeDecExpression(TreeNode* left);

TreeNode *makeintConst(int i);
TreeNode *makeboolConst(int b);
TreeNode *makeCharConst(char c);


