%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scanType.h"
char* filename = "testDataA1/scannerTest.c-";

// Stuff from flex that bison needs
extern int yylex();
extern char* yytext;
extern int newline;
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

extern int newline;
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
%token <keyWordToken> COND
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
        NUMCONST { printf("Line %d Token: %s Value: %d  Input: %s\n",
		yylval.numConstToken.lineNumber,
		yylval.numConstToken.tokenType,
		yylval.numConstToken.numericalValue,
		yylval.numConstToken.stringThatWasTyped); }
	| CHARCONST { printf("Line %d Token: %s Value: '%c'  Input: %s\n",
		yylval.charConstToken.lineNumber,
		yylval.charConstToken.tokenType,
		yylval.charConstToken.letterData,
		yylval.charConstToken.stringThatWasTyped); }
	| BOOLCONST { printf("Line %d Token: %s Value: %d  Input: %s\n",
		yylval.boolConstToken.lineNumber,
		yylval.boolConstToken.tokenType,
		yylval.boolConstToken.numericalValue,
		yylval.boolConstToken.stringThatWasTyped); }
	| RECTYPE { printf("Line %d Token: %s\n",
		yylval.recordToken.lineNumber,
		yylval.recordToken.tokenType); }
	| ID { printf("Line %d Token: %s Value: %s\n",
		yylval.idToken.lineNumber,
		yylval.idToken.tokenType,
		yylval.idToken.IDvalue); }
	| IF { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
        | ELSE { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
        | INT { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
        | BOOL { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| CHAR { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| NOT { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| AND { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| OR { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| RETURN { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| RECORD { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| BREAK { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| WHILE { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| STATIC { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| PARL { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| PARR { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| NOTEQ { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| EQ { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| EQUALS { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| LSS { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| GSS { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| LEQ { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| GEQ { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| ADDE { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| SUBE { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| MULE { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| DIVE { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| DEC { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| INC { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.tokenType); }
	| MUL { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| DIV { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| ADD { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| SUB { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| MOD { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| COND { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| NEG { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| BRACL { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| BRACR { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| SEMI { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| COLON { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| DOT { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| CURLL { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| CURLR { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	| COMMA { printf("Line %d Token: %s\n",
		yylval.keyWordToken.lineNumber,
		yylval.keyWordToken.KWTvalue); }
	;

%%

int main (int argc, char** argv)
{
	if(argc > 1) {
	    char *filename = argv[1];
	   	FILE *myfile = fopen(filename, "r");
	   	if (!myfile) {
	      	printf("I can't open the file\n");
	      	exit(-1);
	   	}
	    yyin = myfile;
	}
    do {
        if(yyparse() != 0) {
        	printf("exit\n");
        	exit(-1);
        }
    } while (!feof(yyin));
}

void yyerror(const char *s)
{
    printf("ERROR(%d): %s: \"%s\"\n", newline, s, yytext);
    // might as well halt now
    //exit(-1);
}
