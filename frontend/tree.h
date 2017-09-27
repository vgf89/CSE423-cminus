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
    } val;

    //ExpType expType;      //used when ExpK for type checking

    int linenum;

    int isArray;         //is this an array
    int arrayLength;
    int isRecord;        //is staticly allocated
    int isStatic;        //is staticly allocated 
} treeNode;

void printType(int type);
void printTree(treeNode *parseTree);
void printSubTree(treeNode *parseTree, int siblingNum, int childNum, int treeLevel);
void printNode(treeNode *parseTree);
void printRec(treeNode *parseTree, char *name, int linenum);
void printVar(treeNode *parseTree, char *name, int type, int linenum);
void printFunc(char *name, int type, int linenum);
void printParam(treeNode *parseTree, char *name, int type, int linenum);
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
treeNode *makeScopedVarDeclaration(treeNode *scopedTypedSpecifier, treeNode *varDeclList);
treeNode *addVarDeclarationInitialize(treeNode *varDeclList, treeNode *varDeclInitialize);

treeNode *makeDeclaration(treeNode* declarationList, treeNode* declaration);
treeNode *makeVarDeclaration(char* id);
treeNode *makeVarDeclarationId(char* id, int isArray, int arraylength);
treeNode *addSimpleExpressionToVarDeclarationID(treeNode *varDeclId, treeNode *simpleExpression);

treeNode *makeFuncStatement(treeNode* typeSpecifier, char* id, treeNode* params, treeNode* statement );
treeNode *makeParamList(treeNode* paramList, treeNode* paramTypeList);
treeNode *makeParamTypeList(treeNode* typeSpecifier, treeNode* paramIdList);
treeNode *makeParamIdList(treeNode* paramIdList, treeNode* paramId);
treeNode *makeParam(char* id, int isArray);

treeNode *addStatementList(treeNode *statementList, treeNode *statement);
treeNode *makeMatchedStatement( treeNode* simpleExpression, treeNode* matched);
treeNode *makeUnmatchedStatement( treeNode* simpleExpression, treeNode* matched, treeNode* unmatched);
treeNode *makeIterationStatement(treeNode* simpleExpression, treeNode* statement);

treeNode *makeRelExpression(treeNode *sumExpressionl, treeNode *relop, treeNode *sumExpressionr);
treeNode *makeLEQ();
treeNode* makeGEQ();
treeNode* makeLSS();
treeNode* makeGSS();
treeNode* makeEQ();
treeNode* makeNOTEQ();

treeNode *makeId(char* id, int isArray);
treeNode *makeEquExpression(treeNode* left, treeNode* right);
treeNode *makeAddEExpression(treeNode* left, treeNode* right);
treeNode *makeSubEExpression(treeNode* left, treeNode* right);
treeNode *makeMulEExpression(treeNode* left, treeNode* right);
treeNode *makeDivEExpression(treeNode* left, treeNode* right);
treeNode *makeIncExpression(treeNode* left);
treeNode *makeDecExpression(treeNode* left);

treeNode *makeNotExpression(treeNode *unaryRelExpression);
treeNode *makeAndExpression(treeNode *andExpression, treeNode *unaryRelExpression);
treeNode *makeSimpleExpression(treeNode *simpleExpression, treeNode *andExpression);

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

treeNode *makeMutableID(char *id);
treeNode *makeMutableBracketExpression(treeNode* mutable_t, treeNode* expression);
treeNode *makeMutableDotId(treeNode* mutable_t, char *id);



