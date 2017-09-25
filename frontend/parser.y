%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scanType.h"
#include "tree.h"

// Stuff from flex that bison needs
extern int yylex();
extern char* yytext;
extern int newline;
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

static treeNode *root;

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
%union {
    int num;
    char* string;
    struct treeNode * node;
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

//Types for grammar
%type<node> declarationList
%type<node> declaration
%type<node> localDeclarations
%type<node> varDeclaration
%type<node> funDeclaration
%type<node> recDeclaration
%type<node> scopedVarDeclaration
%type<node> scopedTypeSpecifier
%type<node> varDeclList
%type<node> typeSpecifier
%type<node> returnTypeSpecifier


//Rules following
%%
program:
    declarationList {
        root = $1; 
        printf("at root %x\n", root);
    }

    declarationList:
        declarationList declaration { $$ = makeDeclaration($1, $2); }
        | declaration { $$ = $1; }
        ;

    declaration:
        varDeclaration {printf("2\n");}//  $$ = newNode(); }
        | funDeclaration {printf("3\n");}//  $$ = newNode(); }
        | recDeclaration {printf("4\n"); $$ = $1;}
        ;
    
    recDeclaration:
        RECORD ID CURLL localDeclarations CURLR
            { printf("5\n"); $$ = makeRecordDeclaration($2.IDvalue, $4); }
        ;
    
    varDeclaration:
        typeSpecifier varDeclList SEMI { }
        ;
    
    scopedVarDeclaration:
        scopedTypeSpecifier varDeclList SEMI {
            treeNode *t = newNode();
            t->type = IntType;
            $$ = makeScopedVarDeclaration($1, $2);
        }
        ;
    
    varDeclList:
        varDeclList COMMA varDeclInitialize { $$ = NULL; }
        | varDeclInitialize { $$ = NULL; }
        ;

    varDeclInitialize:
        varDeclId
        | varDeclId COLON simpleExpression
        ;
    
    varDeclId:
        ID
        | ID BRACL NUMCONST BRACR
        ;
    
    scopedTypeSpecifier:
        STATIC typeSpecifier { 
            $2->isStatic = 1;
            $$ = $2; }
        | typeSpecifier { $$ = $1; }
        ;

    typeSpecifier:
        returnTypeSpecifier { $$ = $1; }
        | RECTYPE       { $$ = makeRecordType(); }
        ;
    
    returnTypeSpecifier:
        INT             { $$ = makeIntType(); }
        | BOOL          { $$ = makeBoolType(); }
        | CHAR          { $$ = makeCharType(); }
        ;
    
    funDeclaration:
        typeSpecifier ID PARL params PARR statement     { $$ = makeFuncStatement( $1, $2, $4, $6 ); }
        | ID PARL params PARR statement                 { $$ = makeFuncStatement( NULL, $1, $3, $5 ); }
        ;
    
    params:
        paramList
        | 
        ;
    
    paramList:
        paramList SEMI paramTypeList
        | paramTypeList
        ;
    
    paramTypeList:
        typeSpecifier paramIdList
        ;

    paramIdList:
        paramIdList COMMA paramId
        | paramId
        ;
    
    paramId:
        ID                      { $$ = makeId($1, 0) }
        | ID BRACL BRACR;       { $$ = makeId($1, 1) }
    
    statementList:
        statementList statement
        |
        ;
    
    statement:
        matched
        | unmatched
        ;

    matched:
        IF PARL simpleExpression PARR matched ELSE matched
        | otherStmt
        ;

    unmatched: 
        IF PARL simpleExpression PARR matched 
        | IF PARL simpleExpression PARR unmatched
        | IF PARL simpleExpression PARR matched ELSE unmatched
        ;

    otherStmt:
        expressionStmt
        | compoundStmt
        | iterationStmt
        | returnStmt
        | breakStmt
        | SEMI
        ;
    
    compoundStmt:
        CURLL localDeclarations statementList CURLR;   // { $$ = makeCompound($2, $4); }
    
    localDeclarations:
        localDeclarations scopedVarDeclaration {
                printf("here!   \t%x\t%x\n", $1, $2);
                $$ = makeLocalDeclaration($1, $2);
            }
        |   { $$ = NULL; }
        ;
    
    expressionStmt:
        expression SEMI
        |
        ;
    
    iterationStmt:
        WHILE PARL simpleExpression PARR statement 
        ;

    returnStmt:
        RETURN SEMI                 //{ $$ = makeReturnStatement( NULL ); }
        | RETURN expression SEMI    //{ $$ = makeReturnStatement($2); }
        ;
    
    breakStmt:
        BREAK SEMI//  { $$ = makeBreakStatement(); }
        ;

    expression:
        mutable EQUALS expression//   { $$ = makeEquExpression($1, $3); }
        | mutable ADDE expression//   { $$ = makeAddEExpression($1, $3); }
        | mutable SUBE expression//   { $$ = makeSubEExpression($1, $3); }
        | mutable MULE expression//   { $$ = makeMulEExpression($1, $3); }
        | mutable DIVE expression//   { $$ = makeDivEExpression($1, $3); }
        | mutable INC//   { $$ = makeIncExpression($1); }
        | mutable DEC//   { $$ = makeDecExpression($1); }
        | simpleExpression
        ;
    
    simpleExpression:
        simpleExpression OR andExpression//   { $$ = makeOrExpression($1, 3); }
        | andExpression
        ;
    
    andExpression:
        andExpression AND unaryRelExpression//   { $$ = makeAndExpression($1, 3); }
        | unaryRelExpression
        ;
    
    unaryRelExpression:
        NOT unaryRelExpression//   { $$ = makeNotExpression($2); }
        | relExpression
        ;
    
    relExpression:
        sumExpression relop sumExpression
        | sumExpression
        ;
    
    relop:
        LEQ
        | GEQ
        | LSS
        | GSS
        | EQ
        | NOTEQ
        ;
    
    sumExpression:
        sumExpression sumop term
        | term
        ;
    
    sumop:
        ADD
        | SUB
        ;
    
    term:
        term mulop unaryExpression
        | unaryExpression
        ;
    
    mulop:
        MUL
        | DIV
        | MOD
        ;
    
    unaryExpression:
        unaryop unaryExpression
        | factor
        ;
    
    unaryop:
        SUB
        | MUL
        | RAND
        | NEG
        ;
    
    factor:
        immutable
        | mutable
        ;
    
    mutable:
        ID
        | mutable BRACL expression BRACR
        | mutable DOT ID
        ;
    
    immutable:
        PARL expression PARR
        | call
        | constant
        ;

    call:
        ID PARL args PARR
        ;

    args:
        argList
        |
        ;
    
    argList:
        argList COMMA expression
        | expression
        ;
    
    constant:
        NUMCONST//        { $$ = makeIntConst($1.numericalValue); }
        | CHARCONST//     { $$ = makeCharConst($1.letterData); }
        | BOOLCONST//     { $$ = makeBoolConst($1.numericalValue); }
        ;
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

    printTree(root);

    printf("Number of warnings: %d\n", numwarn);
    printf("Number of errors: %d\n", numerror);
}

void yyerror(const char *s)
{
    printf("ERROR(%d): %s: \"%s\"\n", newline, s, yytext);
}