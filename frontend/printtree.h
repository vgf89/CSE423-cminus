/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
#ifndef PRINTTREE_H
#define PRINTTREE_H

#include <stdio.h>
#include "treenode.h"
#include "scanType.h"

 typedef enum {SIMPLE, DETAILED} printFormat;

void parseFile(char *filename);

const char* getType(int type);
void printTree(treeNode *parseTree, printFormat p);
void printSubTree(treeNode *parseTree, int siblingNum, int childNum, int treeLevel, printFormat p);
void printNode(treeNode *parseTree, printFormat p);
void printRec(treeNode *parseTree, char *name, int type, int linenum, printFormat p);
void printVar(treeNode *parseTree, char *name, int type, int linenum, printFormat p);
void printFunc(char *name, int type, int linenum, printFormat p);
void printParam(treeNode *parseTree, char *name, int type, int linenum, printFormat p);
void printCompound(int type, int linenum, printFormat p);
void printConst(treeNode *parseTree, int type, int linenum, printFormat p);
void printId(char *name, int type, int linenum, printFormat p);
void printOp(treeNode *parseTree, int type, int linenum, printFormat p);
void printAssign(treeNode *parseTree, int type, int linenum, printFormat p);
void printIf(int type, int linenum, printFormat p);
void printWhile(int type, int linenum, printFormat p);
void printBreak(int type, int linenum, printFormat p);
void printCall(char *name, int type, int linenum, printFormat p);
void printReturn(int type, int linenum, printFormat p);


treeNode *newNode(int linenum);

treeNode *getTokenType(SPT token);

treeNode *makeRecordDeclaration(char* id, treeNode* localDeclarations, int linenum);
treeNode *makeLocalDeclaration(treeNode* localDeclarations, treeNode* scopedVarDeclaration);
treeNode *makeScopedVarDeclaration(treeNode *scopedTypedSpecifier, treeNode *varDeclList);
treeNode *addVarDeclarationInitialize(treeNode *varDeclList, treeNode *varDeclInitialize);

treeNode *makeDeclaration(treeNode* declarationList, treeNode* declaration);
treeNode *makeVarDeclaration(treeNode* typeSpecifier, treeNode* varDeclList);
treeNode *makeVarDeclarationId(SPT token, char* id, int isArray, int arraylength, int linenum);
treeNode *addSimpleExpressionToVarDeclarationID(treeNode *varDeclId, treeNode *simpleExpression);

treeNode *makeFuncStatement(treeNode* typeSpecifier, char* id, treeNode* params, treeNode* statement, int linenum);
treeNode *makeParamList(treeNode* paramList, treeNode* paramTypeList);
treeNode *makeParamTypeList(treeNode* typeSpecifier, treeNode* paramIdList);
treeNode *makeParamIdList(treeNode* paramIdList, treeNode* paramId);
treeNode *makeParam(char* id, int isArray, int linenum);

treeNode *addStatementList(treeNode *statementList, treeNode *statement);
treeNode *makeMatchedStatement(treeNode* simpleExpression, treeNode* matched1, treeNode* matched2, int linenum);
treeNode *makeUnmatchedStatement( treeNode* simpleExpression, treeNode* matched, treeNode* unmatched, int linenum);
treeNode *makeIterationStatement(treeNode* simpleExpression, treeNode* statement, int linenum);

treeNode *makeRelExpression(treeNode *sumExpressionl, treeNode *relop, treeNode *sumExpressionr);
treeNode *makeLEQ(int linenum);
treeNode *makeGEQ(int linenum);
treeNode *makeLSS(int linenum);
treeNode *makeGSS(int linenum);
treeNode *makeEQ(int linenum);
treeNode *makeNOTEQ(int linenum);

treeNode *makeUnaryExpression(treeNode* unaryop, treeNode* unaryExpression);
treeNode *makeSUB(int linenum);
treeNode *makeMUL(int linenum);
treeNode *makeRAND(int linenum);
treeNode *makeNEG(int linenum);

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
treeNode *makeAddOp(int linenum);
treeNode *makeSubOp(int linenum);

treeNode *makeTerm(treeNode *term, treeNode *mulop, treeNode *unaryExpression);
treeNode *makeMulOp(int linenum);
treeNode *makeDivOp(int linenum);
treeNode *makeModOp(int linenum);

treeNode *makeRecordType(int linenum); 
treeNode *makeIntType(int linenum);
treeNode *makeCharType(int linenum);
treeNode *makeBoolType(int linenum);

treeNode *makeBreakStatement(int linenum);
treeNode *makeReturnStatement(treeNode *expression, int linenum);
treeNode *makeCompound(treeNode *left, treeNode *right, int linenum);

treeNode *makeMutableID(SPT token, char *id, int linenum);
treeNode *makeMutableBracketExpression(treeNode* mutable_t, treeNode* expression, int linenum);
treeNode *makeMutableDotId(treeNode* Mutable, char *id, int linenum);

treeNode *makeCall(char *id, treeNode *args, int linenum);
treeNode *makeArgList(treeNode* arglist, treeNode* expression);

treeNode *makeBoolConst(int b, int linenum);
treeNode *makeIntConst(int i, int linenum);
treeNode *makeCharConst(char c, int linenum);

#endif /* PRINTTREE_H */
