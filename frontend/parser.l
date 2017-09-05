%{
#include <stdio.h>
extern int yylex();
char* filename = "cminus_example.cm";
int newline = 1;
%}

digit [0-9]
letter	[a-zA-Z]
letdig [{digit}|{letter}]

%%
[ \t\r]       		;
"//"[^\n]*			{ printf("Comment ignored on line %d\n", newline);}
\n 					{ newline++; }
"if"                { printf("Found IF on line %d: %s\n", newline, yytext);	}
"else"				{ printf("Found ELSE on line %d: %s\n", newline, yytext); }
"int"               { printf("Found INT on line %d: %s\n", newline, yytext); }
"bool"				{ printf("Found BOOL on line %d: %s\n", newline, yytext); }	
"not"				{ printf("Found NOT on line %d: %s\n", newline, yytext); }
"and"				{ printf("Found AND on line %d: %s\n", newline, yytext); }
"or"				{ printf("Found OR on line %d: %s\n", newline, yytext);}
"true"				{ printf("Found BOOLT on line %d: %s\n", newline, yytext);}
"false"				{ printf("Found BOOLF on line %d: %s\n", newline, yytext);}
"return"			{ printf("Found RETURN on line %d: %s\n", newline, yytext); }
"("					{ printf("Found PARL on line %d: %s\n", newline, yytext); }
")"					{ printf("Found PARR on line %d: %s\n", newline, yytext); }
"=="				{ printf("Found EQUIV on line %d: %s\n", newline, yytext); }
"!="				{ printf("Found NEQUIV on line %d: %s\n", newline, yytext); }
"="                 { printf("Found FOUND EQUALS on line %d: %s\n", newline, yytext); }
"<"                 { printf("Found LSS on line %d: %s\n", newline, yytext); }
">"                 { printf("Found GSS on line %d: %s\n", newline, yytext); }
"<="                { printf("Found LEQ on line %d: %s\n", newline, yytext); }
">="                { printf("Found GEQ on line %d: %s\n", newline, yytext); }   	
"+="				{ printf("Found ADDE on line %d: %s\n", newline, yytext); }
"-="				{ printf("Found SUBE on line %d: %s\n", newline, yytext); }
"*="				{ printf("Found MULE on line %d: %s\n", newline, yytext); }
"/="				{ printf("Found DIVE on line %d: %s\n", newline, yytext); }
"--"				{ printf("Found DEC on line %d: %s\n", newline, yytext); }
"++"				{ printf("Found INC on line %d: %s\n", newline, yytext); }
"*"					{ printf("Found MUL on line %d: %s\n", newline, yytext); }
"/"					{ printf("Found DIV on line %d: %s\n", newline, yytext); }
"+"					{ printf("Found ADD on line %d: %s\n", newline, yytext); }
"-"					{ printf("Found SUB on line %d: %s\n", newline, yytext); }
"%"					{ printf("Found MOD on line %d: %s\n", newline, yytext); }
"?"					{ printf("Found COND on line %d: %s\n", newline, yytext); }
"["					{ printf("Found BRACR on line %d: %s\n", newline, yytext); }
"]"					{ printf("Found BRACL on line %d: %s\n", newline, yytext); }
";"					{ printf("Found SEMIC on line %d: %s\n", newline, yytext); }
"."					{ printf("Found DOT on line %d: %s\n", newline, yytext); }
{letter}({letter}|{digit})*		{ printf("Found ID on line %d: %s\n", newline, yytext); }	
{digit}+						{ printf("Found NUMCONST on line %d: %s\n", newline, yytext); }
'([^\n]|\\[^(0|\\n])'		{ printf("Found CHARCONST on line %d: %s\n", newline, yytext); }	
.				printf("EXTRA TRASH FOUND: %s\n", yytext);
%%

int main(int argc, char** argv) {
	// lex through the input:
	FILE *myfile = fopen(filename, "r");
	if (!myfile) {
		printf("Can't open c- file!\n");
		return(-1);
	}
	yyin = myfile;
	yylex();
	printf("%d lines read\n", newline);
}