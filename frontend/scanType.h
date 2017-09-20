//NUMCONST Type
typedef struct NCT {
    int lineNumber;
    char *tokenType;
    char* stringThatWasTyped;

    long long numericalValue;
} NCT;

//CHARCONST Type
typedef struct CCT {
    int lineNumber;
    char *tokenType;
    char *stringThatWasTyped;

    char letterData;
} CCT;

//BOOLCONST Type
typedef struct BCT {
    int lineNumber;
    char *tokenType;
    char *stringThatWasTyped;

    int numericalValue;
} BCT;

//Record Type
typedef struct RECT {
    int lineNumber;
    char *tokenType;
    char *stringThatWasTyped;
} RECT;

//ID Type
typedef struct IDT {
    int lineNumber;
    char* tokenType;
    char* stringThatWasTyped;

    char* IDvalue;
} IDT;

//KeyWord Type
typedef struct KWT {
    int lineNumber;
    char* tokenType;
    char* KWTvalue;
} KWT;
/*
typedef struct treeNode {
    //connecivity in the tree
    struct treeNode *child[MAXCHILDREN]; //children of the node
    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    int lineno;              //linenum 
    NodeKind nodeKind;       //type of node
    union {
        DeclKind decl;       //used when DeclK
        StmtKind stmt;       //used when StmtK
        ExpKind exp;         //used when ExpK
    } kind;

    //extra properties about the node depending on type of the node
    union {                   //relevent data to type -> attr
        OpKind op;            //type of token (same as in bison)
        int value;            //used when an integer constant or boolean
        unsigned char cvalue; //used when a character
        char *string;         //used when a sring constant
        char *name;           //used when Idk
    } attr;

    ExpType expType;      //used when ExpK for type checking
    bool isArray;         //is this an array
    bool isRecord;        //is staticly allocated
    bool isStatic;        //is staticly allocated 
} TreeNode;
*/