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
"if"                { return IF;}
"else"				{ return ELSE;}
"int"               { return <ival> INT;}
"bool"				{ return BOOL;}	
"not"				{ return NOT;}
"and"				{ return AND;}
"or"				{ return OR;}
"true"				{ return BOOLT;}
"false"				{ return BOOLF;}
"return"			{ return RETURN;}
"("					{ return PARL;}
")"					{ return PARR;}
"=="				{ return EQUIV;}
"!="				{ return NEQUIV;}
"="                 { return EQUALS;}
"<"                 { return LSS;}
">"                 { return GSS;}
"<="                { return LEQ;}
">="                { return GEQ;}   	
"+="				{ return ADDE;}
"-="				{ return SUBE;}
"*="				{ return MULE;}
"/="				{ return DIVE;}
"--"				{ return DEC;}
"++"				{ return INC;}
"*"					{ return MUL;}
"/"					{ return DIV;}
"+"					{ return ADD;}
"-"					{ return SUB;}
"%"					{ return MOD;}
"?"					{ return COND;}
"["					{ return BRACR;}
"]"					{ return BRACL;}
";"					{ return SEMI;}
"."					{ return DOT;}
"record"			{ return RECORD;}
"{"					{ return "{";}
"}"					{ return "}";}
","					{ return ",";}
{letter}({letter}|{digit})*		{ return ID;}	
{digit}+						{ return NUMCONST;}
'([^\n]|\\[^(0|\\n])'			{ return CHARCONST;}	
.					;
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