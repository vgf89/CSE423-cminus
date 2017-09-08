%{
#include <stdio.h>
#include <stdlib.h>
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
    int ival;
    char cval;
    int bval;

    NCT numConstToken;
    IDT idToken;
    KWT keyWordToken;
}


// Terminal symbols, based on crazy union stuff
%token <ival> INT
%token <bval> BOOL
%token <cval> CHAR
%token <numConstToken> NUMCONST
%token CHARCONST
%token BOOLCONST
%token <keyWordToken> IF
%token <keyWordToken> ELSE
%token <keyWordToken> NOT
%token <keyWordToken> AND
%token <keyWordToken> OR
%token <keyWordToken> RETURN
%token <keyWordToken> PARL
%token <keyWordToken> PARR
%token <keyWordToken> EQUIV
%token <keyWordToken> NEQUIV
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
%token <keyWordToken> BRACR
%token <keyWordToken> BRACL
%token <keyWordToken> SEMI
%token <keyWordToken> DOT
%token <keyWordToken> ID
%token RECTYPE
%token <keyWordToken> CURLL
%token <keyWordToken> CURLR
%token <keyWordToken> COMMA
%token <keyWordToken> BREAK
%token <keyWordToken> RECORD
%token <keyWordToken> STATIC
%token <keyWordToken> WHILE
%token <keyWordToken> COLON

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
        IF {}
        | ELSE {}
        | INT {}
        | BOOL | CHAR | NOT | AND | OR | RETURN | PARL | PARR | EQUIV | NEQUIV | EQUALS | LSS | GSS | LEQ | GEQ | ADDE | SUBE | MULE | DIVE | DEC | INC | MUL | DIV | ADD | SUB
        | MOD | COND | BRACR | BRACL | SEMI | DOT | ID | NUMCONST | CHARCONST | RECTYPE | CURLL | CURLR | COMMA | BREAK | RECORD | STATIC | WHILE | COLON
        ;

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
    printf("ERROR(%d): %s: \"%s\"\n", newline, s, yytext);
    // might as well halt now
    //exit(-1);
}