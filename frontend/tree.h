#define MAXCHILDREN 2048

typedef struct treeNode {
    //connecivity in the tree

    struct treeNode *child[MAXCHILDREN]; //children of the node

    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    //NodeKind nodeKind;       //type of node

    //extra properties about the node depending on type of the node

    enum {id, intconst, operators} kind;

    union {                   //relevent data to type -> attr
        char* id;
        int intconst;
        struct {treeNode *left, *right} addE;
    } val;
    ExpType expType;      //used when ExpK for type checking

    int isArray;         //is this an array
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} TreeNode;

void printTree(FILE *output, TreeNode parseTree);

TreeNode *newNode();

TreeNode *makeID(char* ID);

TreeNode *makeExpression(TreeNode* left, TreeNode* right);