%{
#include <stdio.h>
#include <stdlib.h>
char* filename = "cminus_example.cm";

// Stuff from flex that bison needs
extern int yylex();
extern char* yytext;
extern int newline;
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

extern int newline;
%}

// The union will figure out the yystype void pointer shit
// as they come back in arbitraily typed tokens
%union {
    int ival;
    char cval;
    int bval;
}

// Terminal symbols, based on crazy union stuff
%token IF
%token ELSE
%token <ival> INT
%token <bval> BOOL
%token <cval> CHAR
%token NOT
%token AND
%token OR
%token RETURN
%token PARL
%token PARR
%token EQUIV
%token NEQUIV
%token EQUALS
%token LSS
%token GSS
%token LEQ
%token GEQ
%token ADDE
%token SUBE
%token MULE
%token DIVE
%token DEC
%token INC
%token MUL
%token DIV
%token ADD
%token SUB
%token MOD
%token COND
%token BRACR
%token BRACL
%token SEMI
%token DOT
%token ID
%token NUMCONST
%token CHARCONST
%token BOOLCONST
%token RECTYPE
%token CURLL
%token CURLR
%token COMMA
%token BREAK
%token RECORD
%token STATIC
%token WHILE
%token COLON

//Rules following
//Parsing top-down
//Not doing much here, just printing back what bison sees
//make sure to make left-recursive as to do action before going to the next
//so as not to overflow the stack with large programs
%%
// The first rule is the highest-level rule, which in out case is
// just the concept of a whole "dinner menu":
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
        expressionStmt | compoundStmt | selectionStmt | iterationStmt | returnStmt | breakStmt;
    
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
        mutable EQUALS | mutable ADDE | mutable SUBE | mutable MULE | mutable DIVE | mutable INC | mutable DEC | simpleExpression;
    
    simpleExpression:
        simpleExpression OR andExpression | andExpression;
    
    andExpression:
        andExpression AND unaryRelExpression | unaryRelExpression;
    
    unaryRelExpression:
        NOT unaryRelExpression | relExpression;
    
    relExpression:
        sumExpression relop sumExpression | sumExpression;
    
    relop:
        LEQ | GEQ | LSS | GSS | EQUIV | NEQUIV;
    
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
        SUB | MUL | COND;
    
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
        NUMCONST | CHARCONST | BOOLCONST;

%%

int main (int argc, char** argv)
{
    FILE *myfile = fopen(filename, "r");
    if (!myfile) {
        printf("I can't open the file\n");
        return -1;
    }

    yyin = myfile;

    do {
        yyparse();
    } while (!feof(yyin));

}

void yyerror(const char *s)
{
    printf("Parsing error (%d) token(%s): %s\n", newline, yytext, s);
    // might as well halt now
    exit(-1);
}