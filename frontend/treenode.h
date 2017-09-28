/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#define MAXCHILDREN 2048

typedef struct treeNode {
    //connecivity in the tree

    struct treeNode *children[MAXCHILDREN]; //children of the node

    struct treeNode *sibling;               //siblings for the node 

    //extra properties about the node depending on type of the node
    enum {Var, Func, Rec, Param, Compound, Const, Id, Op, Assign, If, Break, Call, Return, While} kind;

    //all data types
    enum {IntType, VoidType, CharType, BoolType, RecordType, Undefined} type;

    //all operator types
    enum {Or, And, Not, Leq, Geq, Lss, Gss, Eq, AddE, SubE, MulE, DivE, Noteq, Add, Sub, Mul, Div, Mod, Rand, Neg, Inc, Dec, Dot, Bracl, EEq} opType;
    
    //holds relevent values
    union {
        char* id;
        char* op;
        int boolconst;
        int intconst;
        char charconst;
    } val;

    int linenum;

    int isArray;         //is this an array
    int arrayLength;
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} treeNode;

