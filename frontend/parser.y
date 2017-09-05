%{
#include <stdio.h>

// Stuff from flex that bison needs
extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

extern int linenumber;
%}

// The union will figure out the yystype void pointer shit
// as they come back in arbitraily typed tokens
%union {
    int ival;
    float fval;
    char *sval;
}

// Terminal symbols, based on crazy union stuff
%token <sval> STRING
%token IF
%token ELSE
%token <ival> INT
%token BOOL
%token NOT
%token AND
%token OR
%token BOOLT
%token BOOLF
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
%token SEMIC
%token DOT
%token ID
%token NUMCONST
%token CHARCONST

%token ENDL

//Rules following
//Parsing top-down
//Not doing much here, just printing back what bison sees
//make sure to make left-recursive as to do action before going to the next
//so as not to overflow the stack with large programs
%%
// The first rule is the highest-level rule, which in out case is
// just the concept of a whole "dinner menu":
dinner:
    header template body_section footer { printf("done with a dinner menu!\n"); }
    
    header:
        DINNER FLOAT    { printf("reading a dinner menu version %f\n", $2); }
        ;
    
    template:
        typelines
        ;
    
    typelines:
        typelines typeline
        | typeline
        ;

    typeline:
        TYPE STRING     { printf("new defined dinner type: %s\n", $2); }
        ;
    
    body_section:
        body_lines
        ;
    
    body_lines:
        body_lines body_line
        | body_line
        ;
    body_line:
        INT INT INT INT STRING  { printf("new dinner: %d %d %d %d %s\n", $1, $2, $3, $4, $5); }
        ;
    
    footer:
        END { printf("found the end\n");}
        ;

%%

int main (int argc, char** argv)
{
    FILE *myfile = fopen("in.dinner", "r");
    if (!myfile) {
        printf("I can't open in.dinner\n");
        return -1;
    }

    yyin = myfile;

    do {
        yyparse();
    } while (!feof(yyin));

}

void yyerror(const char *s)
{
    printf("Parsing error (%d): %s\n", linenumber, s);
    // might as well halt now
    exit(-1);
}