%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scanType.h"

// Stuff from flex that bison needs
extern int yylex();
extern char* yytext;
extern int newline;
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

TreeNode *tree;

extern int newline;
int numwarn = 0;
int numerror = 0;
%}

// The union will figure out the yystype void pointer stuff
// as they come back in arbitraily typed tokens
%union {
    NCT numConstToken;
    BCT boolConstToken;
    CCT charConstToken;
    RECT recordToken;
    IDT idToken;
    KWT keyWordToken;
}


// Terminal symbols, based on crazy union stuff
%token <numConstToken> NUMCONST
%token <charConstToken> CHARCONST
%token <boolConstToken> BOOLCONST
%token <recordToken> RECTYPE
%token <idToken> ID

// KeyWord tokens
// Includes Math operators
%token <keyWordToken> IF
%token <keyWordToken> ELSE
%token <keyWordToken> BOOL
%token <keyWordToken> CHAR
%token <keyWordToken> INT
%token <keyWordToken> NOT
%token <keyWordToken> AND
%token <keyWordToken> OR
//TRUE/FALSE?
%token <keyWordToken> RETURN
%token <keyWordToken> RECORD
%token <keyWordToken> BREAK
%token <keyWordToken> WHILE
%token <keywordtoken> STATIC
%token <keyWordToken> PARL
%token <keyWordToken> PARR
%token <keyWordToken> EQ
%token <keyWordToken> NOTEQ
%token <keyWordToken> EQUALS
%token <keyWordToken> LSS
%token <keyWordToken> GSS
%token <keyWordToken> LEQ
%token <keyWordToken> GEQ
%token <keyWordToken> ADDE
%token <keyWordToken> SUBE
%token <keyWordToken> MULE
%token <keyWordToken> DIVE
%token <keyWordToken> DEC
%token <keyWordToken> INC
%token <keyWordToken> MUL
%token <keyWordToken> DIV
%token <keyWordToken> ADD
%token <keyWordToken> SUB
%token <keyWordToken> MOD
%token <keyWordToken> RAND
%token <keyWordToken> NEG
%token <keyWordToken> BRACL
%token <keyWordToken> BRACR
%token <keyWordToken> SEMI
%token <keyWordToken> COLON
%token <keyWordToken> DOT
%token <keyWordToken> CURLL
%token <keyWordToken> CURLR
%token <keyWordToken> COMMA

//Rules following
//Parsing top-down
//Not doing much here, just printing back what bison sees
//make sure to make left-recursive as to do action before going to the next
//so as not to overflow the stack with large programs
%%
program:

    declarationList

    declarationList:
        declarationList declaration | declaration
        ;

    declaration:
        varDeclaration | funDeclaration | recDeclaration
        ;
    
    recDeclaration:
        RECORD ID CURLL localDeclarations CURLR
        ;
    
    varDeclaration:
        typeSpecifier varDeclList SEMI
        ;
    
    scopedVarDeclaration:
        scopedTypeSpecifier varDeclList SEMI
        ;
    
    varDeclList:
        varDeclList COMMA varDeclInitialize | varDeclInitialize
        ;

    varDeclInitialize:
        varDeclId | varDeclId COLON simpleExpression
        ;
    
    varDeclId:
        ID | ID BRACL NUMCONST BRACR
        ;
    
    scopedTypeSpecifier:
        STATIC typeSpecifier | typeSpecifier
        ;

    typeSpecifier:
        returnTypeSpecifier | RECTYPE
        ;
    
    returnTypeSpecifier:
        INT | BOOL | CHAR
        ;
    
    funDeclaration:
        typeSpecifier ID PARL params PARR statement | ID PARL params PARR statement
        ;
    
    params:
        paramList | 
        ;
    
    paramList:
        paramList SEMI paramTypeList | paramTypeList;
    
    paramTypeList:
        typeSpecifier paramIdList;

    paramIdList:
        paramIdList COMMA paramId | paramId;
    
    paramId:
        ID | ID BRACL BRACR;
    
    statement:
        expressionStmt | compoundStmt | selectionStmt | iterationStmt | returnStmt | breakStmt | SEMI;
    
    compoundStmt:
        CURLL localDeclarations statementList CURLR;
    
    localDeclarations:
        localDeclarations scopedVarDeclaration | ;
    
    statementList:
        statementList statement | ;
    
    expressionStmt:
        expression SEMI | ;
    
    selectionStmt:
        IF PARL simpleExpression PARR statement | IF PARL simpleExpression PARR statement ELSE statement;
    
    iterationStmt:
        WHILE PARL simpleExpression PARR statement;

    returnStmt:
        RETURN SEMI | RETURN expression SEMI;
    
    breakStmt:
        BREAK SEMI;

    expression:
        mutable EQUALS expression | mutable ADDE expression | mutable SUBE expression | mutable MULE expression | mutable DIVE expression | mutable INC | mutable DEC | simpleExpression;
    
    simpleExpression:
        simpleExpression OR andExpression | andExpression;
    
    andExpression:
        andExpression AND unaryRelExpression | unaryRelExpression;
    
    unaryRelExpression:
        NOT unaryRelExpression | relExpression;
    
    relExpression:
        sumExpression relop sumExpression | sumExpression;
    
    relop:
        LEQ | GEQ | LSS | GSS | EQ | NOTEQ;
    
    sumExpression:
        sumExpression sumop term | term;
    
    sumop:
        ADD | SUB;
    
    term:
        term mulop unaryExpression | unaryExpression;
    
    mulop:
        MUL | DIV | MOD;
    
    unaryExpression:
        unaryop unaryExpression | factor;
    
    unaryop:
        SUB | MUL | RAND | NEG;
    
    factor:
        immutable | mutable;
    
    mutable:
        ID | mutable BRACL expression BRACR | mutable DOT ID;
    
    immutable:
        PARL expression PARR | call | constant;

    call:
        ID PARL args PARR;

    args:
        argList | ;
    
    argList:
        argList COMMA expression | expression;
    
    constant:
        NUMCONST | CHARCONST | BOOLCONST

%%

int main (int argc, char** argv)
{
	char *filename = argv[1];
	int c = 0;
	while ((c = getopt(argc, argv, "d")) != -1) { 
    	switch (c) {
    	case 'd':
    			if(optarg != NULL)
					filename = optarg;

    			yydebug = 1;
            	break;  
		case '\?':
                printf("c-: Invalid option \n");
                return -1;
                break;
        case ':':
                printf("c-: Requires a filename argument\n");
                return -1;
                break;
       	default:   
       			printf("incorrect input\n");
                return -1;
        }
    }
	if(argc > 1) {
	   	FILE *myfile = fopen(filename, "r");
	   	if (!myfile) {
	      	printf("I can't open the file\n");
	      	exit(-1);
	   	}
	    yyin = myfile;
	}
    do {
        if(yyparse() != 0) {
        	printf("Number of warnings: %d\n", numwarn);
			printf("Number of errors: %d\n", numerror);
		//printf("exit\n");
        	exit(-1);
        }
    } while (!feof(yyin));
    printf("Number of warnings: %d\n", numwarn);
    printf("Number of errors: %d\n", numerror);
}

void yyerror(const char *s)
{
    printf("ERROR(%d): %s: \"%s\"\n", newline, s, yytext);
}

void printTree(FILE *output, TreeNode parseTree) 
{
	//print tree here   
}
/*
        NCT numType;
        CCT charType;
        BCT boolType;
        RECT rectType;
        IDT idType;
        KWT keywordType;
*/
TreeNode *makeNCT(NCT numType) {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	node->kind.numType = numType;
	return node;
}
TreeNode *makeCCT(CCT charType) {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	node->kind.charType = charType;
	return node;
}
TreeNode *makeBCT(BCT boolType) {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	node->kind.boolType = boolType;
	return node;
}
TreeNode *makeRECT(RECT rectType) {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	node->kind.rectType = rectType;
	return node;
}
TreeNode *makeIDT(IDT idType) {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	node->kind.idType = idType;
	return node;
}
TreeNode *makeKWT(KWT keywordType) {
	TreeNode *node = (TreeNode*) calloc (1, sizeof(TreeNode));
	node->kind.keywordType = keywordType;
	return node;
}