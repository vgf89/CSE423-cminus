#include <stdio.h>
#define MAXCHILDREN 2048

typedef struct treeNode {
    //connecivity in the tree

    struct treeNode *children[MAXCHILDREN]; //children of the node

    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    //NodeKind nodeKind;       //type of node

    //extra properties about the node depending on type of the node

    enum {Var, Func, Rec, Param, Compound, Const, Id, Op, Assign, If, Break, Call, Return, While, Incomplete} kind;

    enum {IntType, VoidType, CharType, BoolType, RecordType} type;

    enum {Or, And, Not, Leq, Geq, Lss, Gss, Eq, Noteq, Add, Sub, Mul, Div, Mod, Rand, Neg, Inc, Dec} opType;
    
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
        struct { struct treeNode *left, *right; } compound;
        struct { struct treeNode *expression; } returnStatement;
        struct { struct treeNode *typeSpecifier, *parameterList, *statement; char* id;} Func;
        //struct { struct treeNode *args; char* id; } // What is this supposed to be?
    } val;

    //ExpType expType;      //used when ExpK for type checking

    int linenum;

    int isArray;         //is this an array
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} treeNode;

char* getType(int type);
void printTree(treeNode *parseTree);
void printSubTree(treeNode *parseTree, int siblingNum, int childNum, int treeLevel);
void printNode(treeNode *parseTree);
void printRec(treeNode *parseTree, char *name, int linenum);
void printVar(treeNode *parseTree, char *name, int type, int linenum);
void printFunc(char *name, int type, int linenum);
void printParam(char *name, int type, int linenum);
void printCompound(int linenum);
void printConst(treeNode *parseTree, int type, int linenum);
void printId(char *name, int linenum);
void printOp(char *op, int linenum);
void printAssign(int linenum);
void printIf(int linenum);
void printBreak(int linenum);
void printCall(char *name, int linenum);
void printReturn(int linenum);


treeNode *newNode();

treeNode *makeRecordDeclaration(char* id, treeNode* localDeclarations);
treeNode *makeLocalDeclaration(treeNode* localDeclarations, treeNode* scopedVarDeclaration);

treeNode *makeID(char* ID, int isArray);
treeNode *makeEquExpression(treeNode* left, treeNode* right);
treeNode *makeAddEExpression(treeNode* left, treeNode* right);
treeNode *makeSubEExpression(treeNode* left, treeNode* right);
treeNode *makeMulEExpression(treeNode* left, treeNode* right);
treeNode *makeDivEExpression(treeNode* left, treeNode* right);
treeNode *makeIncExpression(treeNode* left);
treeNode *makeDecExpression(treeNode* left);

treeNode *makeNotExpression(treeNode *left);
treeNode *makeAndExpression(treeNode *left, treeNode *right);
treeNode *makeOrExpression(treeNode *left, treeNode *right);

treeNode *makeintConst(int i);
treeNode *makeboolConst(int b);
treeNode *makeCharConst(char c);

treeNode *makeRecordType(); 
treeNode *makeIntType();
treeNode *makeCharType();
treeNode *makeBoolType();

treeNode *makeBreakStatement( );
treeNode *makeReturnStatement(treeNode *expression);
treeNode *makeCompound(treeNode *left, treeNode *right);



