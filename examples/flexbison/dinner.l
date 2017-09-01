%{
#include <stdio.h>
#include <string.h>
extern int yylex();

#include "dinner.tab.h"

int linenumber = 1;
%}

%%
diNNeR          { return DINNER; }
type            { return TYPE; }
end             { return END; }
[ \t]         ;
[0-9]+\.[0-9]+  { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+          { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+    { yylval.sval = strdup(yytext); return STRING; }
\n              { ++linenumber; }
.               ;
%%