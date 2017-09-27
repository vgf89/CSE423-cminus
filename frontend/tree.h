#include <stdio.h>
#define MAXCHILDREN 2048

typedef struct treeNode {
    //connecivity in the tree

    struct treeNode *children[MAXCHILDREN]; //children of the node

    struct treeNode *sibling;            //siblings for the node 

    //what kind of node
    //NodeKind nodeKind;       //type of node

    //extra properties about the node depending on type of the node

    enum {Var, Func, Rec, Param, Compound, Const, Id, Op, Assign, If, Break, Call, Return, While} kind;

    enum {IntType, VoidType, CharType, BoolType, RecordType} type;

    enum {Or, And, Not, Leq, Geq, Lss, Gss, Eq, AddE, SubE, MulE, DivE, Noteq, Add, Sub, Mul, Div, Mod, Rand, Neg, Inc, Dec, Dot, Bracl, EEq} opType;
    
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
void printOp(treeNode *parseTree, int linenum);
void printAssign(treeNode *parseTree, int linenum);
void printIf(int linenum);
void printWhile(int linenum);
void printBreak(int linenum);
void printCall(char *name, int linenum);
void printReturn(int linenum);


treeNode *newNode(int linenum);

treeNode *makeRecordDeclaration(char* id, treeNode* localDeclarations, int linenum);
treeNode *makeLocalDeclaration(treeNode* localDeclarations, treeNode* scopedVarDeclaration);
treeNode *makeScopedVarDeclaration(treeNode *scopedTypedSpecifier, treeNode *varDeclList);
treeNode *addVarDeclarationInitialize(treeNode *varDeclList, treeNode *varDeclInitialize);

treeNode *makeDeclaration(treeNode* declarationList, treeNode* declaration);
treeNode *makeVarDeclaration(treeNode* typeSpecifier, treeNode* varDeclList);
treeNode *makeVarDeclarationId(char* id, int isArray, int arraylength, int linenum);
treeNode *addSimpleExpressionToVarDeclarationID(treeNode *varDeclId, treeNode *simpleExpression);

treeNode *makeFuncStatement(treeNode* typeSpecifier, char* id, treeNode* params, treeNode* statement, int linenum);
treeNode *makeParamList(treeNode* paramList, treeNode* paramTypeList);
treeNode *makeParamTypeList(treeNode* typeSpecifier, treeNode* paramIdList);
treeNode *makeParamIdList(treeNode* paramIdList, treeNode* paramId);
treeNode *makeParam(char* id, int isArray, int linenum);

treeNode *addStatementList(treeNode *statementList, treeNode *statement);
treeNode *makeMatchedStatement( treeNode* simpleExpression, treeNode* matched, int linenum);
treeNode *makeUnmatchedStatement( treeNode* simpleExpression, treeNode* matched, treeNode* unmatched, int linenum);
treeNode *makeIterationStatement(treeNode* simpleExpression, treeNode* statement, int linenum);

treeNode *makeRelExpression(treeNode *sumExpressionl, treeNode *relop, treeNode *sumExpressionr);
treeNode *makeLEQ();
treeNode *makeGEQ();
treeNode *makeLSS();
treeNode *makeGSS();
treeNode *makeEQ();
treeNode *makeNOTEQ();

treeNode *makeUnaryExpression(treeNode* unaryop, treeNode* unaryExpression);
treeNode *makeSUB();
treeNode *makeMUL();
treeNode *makeRAND();
treeNode *makeNEG();

treeNode *makeId(char* id, int isArray, int linenum);
treeNode *makeEquExpression(treeNode* left, treeNode* right, int linenum);
treeNode *makeAddEExpression(treeNode* left, treeNode* right, int linenum);
treeNode *makeSubEExpression(treeNode* left, treeNode* right, int linenum);
treeNode *makeMulEExpression(treeNode* left, treeNode* right, int linenum);
treeNode *makeDivEExpression(treeNode* left, treeNode* right, int linenum);
treeNode *makeIncExpression(treeNode* left, int linenum);
treeNode *makeDecExpression(treeNode* left, int linenum);

treeNode *makeNotExpression(treeNode *unaryRelExpression, int linenum);
treeNode *makeAndExpression(treeNode *andExpression, treeNode *unaryRelExpression, int linenum);
treeNode *makeSimpleExpression(treeNode *simpleExpression, treeNode *andExpression, int linenum);

treeNode *makeSumExpression(treeNode *sumExpression, treeNode *sumop, treeNode *term);
treeNode *makeAddOp();
treeNode *makeSubOp();

treeNode *makeTerm(treeNode *term, treeNode *mulop, treeNode *unaryExpression);
treeNode *makeMulOp();
treeNode *makeDivOp();
treeNode *makeModOp();

treeNode *makeRecordType(); 
treeNode *makeIntType();
treeNode *makeCharType();
treeNode *makeBoolType();

treeNode *makeBreakStatement( );
treeNode *makeReturnStatement(treeNode *expression, int linenum);
treeNode *makeCompound(treeNode *left, treeNode *right, int linenum);

treeNode *makeMutableID(char *id, int linenum);
treeNode *makeMutableBracketExpression(treeNode* mutable_t, treeNode* expression, int linenum);
treeNode *makeMutableDotId(treeNode* mutable, char *id, int linenum);

treeNode *makeCall(char *id, treeNode *args, int linenum);
treeNode *makeArgList(treeNode* arglist, treeNode* expression);

treeNode *makeBoolConst(int b, int linenum);
treeNode *makeIntConst(int i, int linenum);
treeNode *makeCharConst(char c, int linenum);
