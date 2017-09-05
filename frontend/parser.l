%{
#include <stdio.h>
#include <string.h>
extern int yylex();

#include "parser.tab.h"

int newline = 1;

int i;
char* rectable[10];
int recvar = 0;
int reccount = 0;
%}

digit [0-9]
letter	[a-zA-Z]
letdig [{digit}|{letter}]

%%
[ \t\r]       		;
"//"[^\n]*			; 
\n 					{ newline++; }
"if"                { return IF;}
"else"				{ return ELSE;}
"int"               { return INT;}
"bool"				{ return BOOL;}
"char"				{ return CHAR;}	
"not"				{ return NOT;}
"and"				{ return AND;}
"or"				{ return OR;}
"true"				{ yylval.bval = 1; return BOOLCONST;}
"false"				{ yylval.bval = 0; return BOOLCONST;}
"return"			{ return RETURN;}
"record"			{ recvar = 1; return RECORD;}
"break"				{ return BREAK; }
"while"				{ return WHILE; }
"static"			{ return STATIC; }
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
"["					{ return BRACL;}
"]"					{ return BRACR;}
";"					{ return SEMI;}
":"					{ return COLON;}
"."					{ return DOT;}
"{"					{ return CURLL;}
"}"					{ return CURLR;}
","					{ return COMMA;}
{letter}({letter}|{digit})*		{ 
									if (recvar == 1) {
										//printf("adding \"%s\" to record list\n", yytext);
										rectable[reccount] = strdup(yytext);
										reccount++;
										recvar = 0;
										return ID;
									}
									for(i = 0; i < reccount; i++) {
										//printf("looking for a match of \"%s\"...\n", yytext);
										//printf("%s\n", rectable[i]);
										if (strcmp(yytext, rectable[i]) == 0) {
											//printf("found \"%s\" in record types!\n", yytext);
											return RECTYPE; 
											} 
									}
									return ID;
								}
{digit}+						{ yylval.ival = atoi(yytext); return NUMCONST;}
'([^\n|\\']|\\[^\0|\n])'		{ yylval.cval = yytext[0]; return CHARCONST;}	
.					;
%%

/*
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
} */