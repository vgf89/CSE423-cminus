/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */
%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scanType.h"
#include "printtree.h"

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
    SPT superToken;
}

%union {
    int num;
    char* string;
    struct treeNode * node;
}


// Terminal symbols, based on crazy union stuff
%token <superToken> NUMCONST
%token <superToken> CHARCONST
%token <superToken> BOOLCONST
%token <superToken> RECTYPE
%token <superToken> ID

// KeyWord tokens
// Includes Math operators
%token <superToken> IF
%token <superToken> ELSE
%token <superToken> BOOL
%token <superToken> CHAR
%token <superToken> INT
%token <superToken> NOT
%token <superToken> AND
%token <superToken> OR
//TRUE/FALSE?
%token <superToken> RETURN
%token <superToken> RECORD
%token <superToken> BREAK
%token <superToken> WHILE
%token <superToken> STATIC
%token <superToken> PARL
%token <superToken> PARR
%token <superToken> EQ
%token <superToken> NOTEQ
%token <superToken> EQUALS
%token <superToken> LSS
%token <superToken> GSS
%token <superToken> LEQ
%token <superToken> GEQ
%token <superToken> ADDE
%token <superToken> SUBE
%token <superToken> MULE
%token <superToken> DIVE
%token <superToken> DEC
%token <superToken> INC
%token <superToken> MUL
%token <superToken> DIV
%token <superToken> ADD
%token <superToken> SUB
%token <superToken> MOD
%token <superToken> RAND
%token <superToken> NEG
%token <superToken> BRACL
%token <superToken> BRACR
%token <superToken> SEMI
%token <superToken> COLON
%token <superToken> DOT
%token <superToken> CURLL
%token <superToken> CURLR
%token <superToken> COMMA

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
%type<node> varDeclInitialize
%type<node> typeSpecifier
%type<node> returnTypeSpecifier
%type<node> simpleExpression
%type<node> varDeclId
%type<node> paramList
%type<node> paramTypeList
%type<node> paramIdList
%type<node> paramId
%type<node> params
%type<node> statement
%type<node> statementList
%type<node> matched
%type<node> unmatched
%type<node> otherStmt
%type<node> expressionStmt
%type<node> compoundStmt
%type<node> iterationStmt
%type<node> returnStmt
%type<node> breakStmt
%type<node> Mutable
%type<node> immutable
%type<node> expression
%type<node> relExpression
%type<node> unaryRelExpression
%type<node> andExpression
%type<node> sumExpression
%type<node> relop
%type<node> unaryExpression
%type<node> unaryop
%type<node> factor
%type<node> term
%type<node> sumop
%type<node> mulop
%type<node> call
%type<node> argList
%type<node> args
%type<node> constant

//Rules following
%%
program:
    declarationList {
        root = $1; 
        //printf("at root %x\n", root);
    }

    declarationList:
        declarationList declaration { $$ = makeDeclaration($1, $2); }
        | declaration { $$ = $1; }
        ;

    declaration:
        varDeclaration { $$ = $1; }
        | funDeclaration { $$ = $1; }
        | recDeclaration { $$ = $1; }
        ;
    
    recDeclaration:
        RECORD ID CURLL localDeclarations CURLR
            { $$ = makeRecordDeclaration($2.IDvalue, $4, $1.lineNumber); }
        ;
    
    varDeclaration:
        typeSpecifier varDeclList SEMI { $$ = makeVarDeclaration($1, $2); }
        ;
    
    scopedVarDeclaration:
        scopedTypeSpecifier varDeclList SEMI { $$ = makeScopedVarDeclaration($1, $2); }
        ;
    
    varDeclList:
        varDeclList COMMA varDeclInitialize { $$ = addVarDeclarationInitialize($1, $3); }
        | varDeclInitialize { $$ = $1; }
        ;

    varDeclInitialize:
        varDeclId   { $$ = $1; }
        | varDeclId COLON simpleExpression   { $$ = addSimpleExpressionToVarDeclarationID($1, $3); }
        ;
    
    varDeclId:
        ID  { $$ = makeVarDeclarationId($1.IDvalue, 0, 0, $1.lineNumber); }
        | ID BRACL NUMCONST BRACR     {
            //printf("%s\n", $1.IDvalue);
            $$ = makeVarDeclarationId($1.IDvalue, 1, $3.numericalValue, $1.lineNumber); 
                                        $$->arrayLength = $3.numericalValue; }
        ;
    
    scopedTypeSpecifier:
        STATIC typeSpecifier { 
            $2->isStatic = 1;
            $$ = $2;
        }
        | typeSpecifier { $$ = $1; }
        ;

    typeSpecifier:
        returnTypeSpecifier { $$ = $1; }
        | RECTYPE       { $$ = makeRecordType($1.lineNumber); }
        ;
    
    returnTypeSpecifier:
        INT             { $$ = makeIntType($1.lineNumber); }
        | BOOL          { $$ = makeBoolType($1.lineNumber); }
        | CHAR          { $$ = makeCharType($1.lineNumber); }
        ;
    
    funDeclaration:
        typeSpecifier ID PARL params PARR statement { $$ = makeFuncStatement( $1, $2.IDvalue, $4, $6, $2.lineNumber); }
        | ID PARL params PARR statement             { $$ = makeFuncStatement( NULL, $1.IDvalue, $3, $5, $1.lineNumber); }
        ;
    
    params:
        paramList   { $$ = $1; }   
        |           { $$ = NULL; }
        ;
    
    paramList:
        paramList SEMI paramTypeList { $$ = makeParamList($1, $3); }
        | paramTypeList {$$ = makeParamList(NULL, $1); }
        ;
    
    paramTypeList:
        typeSpecifier paramIdList { $$ = makeParamTypeList($1, $2); }
        ;

    paramIdList:
        paramIdList COMMA paramId   { $$ = makeParamIdList($1, $3); }
        | paramId                   { $$ = makeParamIdList(NULL, $1); }
        ;
    
    paramId:
        ID                      { $$ = makeParam($1.IDvalue, 0, $1.lineNumber); }
        | ID BRACL BRACR        { $$ = makeParam($1.IDvalue, 1, $1.lineNumber); }
        ;
    
    statementList:
        statementList statement { $$ = addStatementList($1, $2); }  
        |   { $$ = NULL; }
        ;
    
    statement:
        matched { $$ = $1; }
        | unmatched { $$ = $1; }
        ;

    matched:

        IF PARL simpleExpression PARR matched ELSE matched      { $$ = makeMatchedStatement($3, $5, $7, $1.lineNumber); }
        | otherStmt                                             { $$ = $1; }
        ;

    unmatched:
        IF PARL simpleExpression PARR matched                   { $$ = makeUnmatchedStatement($3, $5, NULL, $1.lineNumber); }
        | IF PARL simpleExpression PARR unmatched               { $$ = makeUnmatchedStatement($3, NULL, $5, $1.lineNumber); }
        | IF PARL simpleExpression PARR matched ELSE unmatched  { $$ = makeUnmatchedStatement($3, $5, $7, $1.lineNumber); }
        ;

    otherStmt:
        expressionStmt  { $$ = $1; }
        | compoundStmt  { $$ = $1; }
        | iterationStmt { $$ = $1; }
        | returnStmt    { $$ = $1; }
        | breakStmt     { $$ = $1; }
        | SEMI          { $$ = NULL; }
        ;
    
    compoundStmt:
        CURLL localDeclarations statementList CURLR    { $$ = makeCompound($2, $3, $1.lineNumber); }
        ;
    
    localDeclarations:
        localDeclarations scopedVarDeclaration  { $$ = makeLocalDeclaration($1, $2); }
        |   { $$ = NULL; }
        ;                                       
    
    expressionStmt:
        expression SEMI  { $$ = $1; }
        |   { $$ = NULL; }
        ;
    
    iterationStmt:
        WHILE PARL simpleExpression PARR statement { $$ = makeIterationStatement($3, $5, $1.lineNumber); }
        ;

    returnStmt:
        RETURN SEMI                 { $$ = makeReturnStatement(NULL, $1.lineNumber); }
        | RETURN expression SEMI    { $$ = makeReturnStatement($2, $1.lineNumber); }
        ;
    
    breakStmt:
        BREAK SEMI  { $$ = makeBreakStatement($1.lineNumber); }
        ;

    expression:
        Mutable EQUALS expression   { $$ = makeEquExpression($1, $3, $2.lineNumber); }
        | Mutable ADDE expression   { $$ = makeAddEExpression($1, $3, $2.lineNumber); }
        | Mutable SUBE expression   { $$ = makeSubEExpression($1, $3, $2.lineNumber); }
        | Mutable MULE expression   { $$ = makeMulEExpression($1, $3, $2.lineNumber); }
        | Mutable DIVE expression   { $$ = makeDivEExpression($1, $3, $2.lineNumber); }
        | Mutable INC               { $$ = makeIncExpression($1, $2.lineNumber); }
        | Mutable DEC               { $$ = makeDecExpression($1, $2.lineNumber); }
        | simpleExpression          { $$ = $1; }
        ;
    
    simpleExpression:
        simpleExpression OR andExpression { $$ = makeSimpleExpression($1, $3, $2.lineNumber); }
        | andExpression { $$ = $1; }
        ;
    
    andExpression:
        andExpression AND unaryRelExpression { $$ = makeAndExpression($1, $3, $2.lineNumber); }
        | unaryRelExpression { $$ = $1; }
        ;
    
    unaryRelExpression:
        NOT unaryRelExpression { $$ = makeNotExpression($2, $1.lineNumber); }
        | relExpression { $$ = $1; }
        ;
    
    relExpression:
        sumExpression relop sumExpression   { $$ = makeRelExpression ($1, $2, $3); }
        | sumExpression   { $$ = $1; }
        ;
    
    relop:
        LEQ     { $$ = makeLEQ($1.lineNumber); }
        | GEQ   { $$ = makeGEQ($1.lineNumber); }
        | LSS   { $$ = makeLSS($1.lineNumber); }
        | GSS   { $$ = makeGSS($1.lineNumber); }
        | EQ    { $$ = makeEQ($1.lineNumber); }
        | NOTEQ { $$ = makeNOTEQ($1.lineNumber); }
        ;
    
    sumExpression:
        sumExpression sumop term    { $$ = makeSumExpression($1, $2, $3); }
        | term                      { $$ = $1; }
        ;
    
    sumop:
        ADD	{ $$ = makeAddOp($1.lineNumber); }
        | SUB	{ $$ = makeSubOp($1.lineNumber); }
        ;
    
    term:
        term mulop unaryExpression	{ $$ = makeTerm($1, $2, $3); }
        | unaryExpression		{ $$ = $1; }
        ;
    
    mulop:
        MUL	{ $$ = makeMulOp($1.lineNumber); }
        | DIV	{ $$ = makeDivOp($1.lineNumber); }
        | MOD	{ $$ = makeModOp($1.lineNumber); }
        ;
    
    unaryExpression:
        unaryop unaryExpression { $$ = makeUnaryExpression($1, $2); }
        | factor { $$ = $1; }
        ;
    
    unaryop:
        SUB     { $$ = makeSUB($1.lineNumber); }
        | MUL   { $$ = makeMUL($1.lineNumber); }
        | RAND  { $$ = makeRAND($1.lineNumber); }
        | NEG   { $$ = makeNEG($1.lineNumber); }
        ;
    
    factor:
        immutable   { $$ = $1; }
        | Mutable   { $$ = $1; }
        ;
    
    Mutable:
        ID  { $$ = makeMutableID($1.IDvalue, $1.lineNumber); }
        | Mutable BRACL expression BRACR  { 
            $$ = makeMutableBracketExpression($1, $3, $2.lineNumber); }
        | Mutable DOT ID  { $$ = makeMutableDotId($1, $3.IDvalue, $3.lineNumber); }
        ;
    
    immutable:
        PARL expression PARR  { $$ = $2; }
        | call  { $$ = $1; }
        | constant  { $$ = $1; }
        ;

    call:
        ID PARL args PARR       { $$ = makeCall($1.IDvalue, $3, $1.lineNumber); }
        ;

    args:
        argList { $$ = $1; }
        | { $$ = NULL; }
        ;
    
    argList:
        argList COMMA expression    { $$ = makeArgList($1, $3); }
        | expression                { $$ = $1; }
        ;
    
    constant:
        NUMCONST          { $$ = makeIntConst($1.numericalValue, $1.lineNumber); }
        | CHARCONST       { $$ = makeCharConst($1.letterData, $1.lineNumber); }
        | BOOLCONST       { $$ = makeBoolConst($1.numericalValue, $1.lineNumber); }
        ;
%%

int main (int argc, char** argv)
{
    char *filename = NULL;
    int i = 0;
    int c = 0;
    int canPrint = 0;

    //handling for debug and any future options
    while(c != -1) {
        c = getopt(argc, argv, "dpP");

        //catches case where file name is first and args are after
        if(i == 0 && c == -1) {
            if(argv[optind] != NULL)
                filename = argv[optind];
            i++;
            optind++;

            //reset c if we have more arguments
            if(argc > 1)
                c = 0;
        }
        else {
            switch (c) {
                case 'd':
                        yydebug = 1;
                        if(optarg != NULL)
                            filename = optarg;
                        break;

                case 'p':
                        canPrint = 1;
                        if(optarg != NULL)
                            filename = optarg;
                        break;

                case 'P':
                        canPrint = 2;
                        if(optarg != NULL)
                            filename = optarg;            
                        break;

                case '\?':
                        printf("c-: Invalid option \n");
                        return -1;
                        break;
                case ':':
                        printf("c-: %s option requires argument\n", optarg);
                        return -1;
                        break;
                default:
                        break;
            }  
        }

    }

    printFormat p;
    if(filename == NULL)
        printf("invalid file name\n");
    else
        parseFile(filename);

    if(canPrint == 1) {
        p = SIMPLE;
        //print AST
        printTree(root, p);
    }

    if(canPrint == 2) {
        //print AST with types
        p = DETAILED;
        printTree(root, p);
    }

    printf("Number of warnings: %d\n", numwarn);
    printf("Number of errors: %d\n", numerror);
}

void parseFile(char *filename)
{
    //open file
    if(filename != NULL) {
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
            exit(-1);
        }
    } while (!feof(yyin));
}

void yyerror(const char *s)
{
    printf("ERROR(%d): %s: \"%s\"\n", newline, s, yytext);
}
