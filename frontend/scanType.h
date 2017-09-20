#define MAXCHILDREN 2048

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

typedef struct treeNode {
    //connecivity in the tree
    struct treeNode *child[MAXCHILDREN]; //children of the node
    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    //NodeKind nodeKind;       //type of node
    union {
        NCT numType;
        CCT charType;
        BCT boolType;
        RECT rectType;
        IDT idType;
        KWT keywordType;
    } kind;

    //extra properties about the node depending on type of the node
    /*
    union {                   //relevent data to type -> attr
        OpKind op;            //type of token (same as in bison)
        int value;            //used when an integer constant or boolean
        unsigned char cvalue; //used when a character
        char *string;         //used when a sring constant
        char *name;           //used when Idk
    } attr;
    ExpType expType;      //used when ExpK for type checking
    */
    int isArray;         //is this an array
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} TreeNode;
