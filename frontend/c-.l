%{
#include <stdio.h>
#include <string.h>
#include "scanType.h"
extern int yylex();

#include "c-.tab.h"

int newline = 1;

int i;
char* rectable[10];
int recvar = 0;
int reccount = 0;

NCT numConstToken;
IDT IDToken;
KWT KeyWordToken;

%}

digit [0-9]
letter	[a-zA-Z]
letdig [{digit}|{letter}]

%%
[ \t\r]       		;
"//"[^\n]*			; 
\n 					{ newline++; }
"if"                { printf("Line %d Token: %s\n", newline, yytext); return IF;}
"else"				{ printf("Line %d Token: %s\n", newline, yytext); return ELSE;}
"int"               { printf("Line %d Token: %s\n", newline, yytext); return INT;}
"bool"				{ printf("Line %d Token: %s\n", newline, yytext); return BOOL;}
"char"				{ printf("Line %d Token: %s\n", newline, yytext); return CHAR;}	
"not"				{ printf("Line %d Token: NOT\n", newline); return NOT;}
"and"				{ printf("Line %d Token: AND\n", newline); return AND;}
"or"				{ printf("Line %d Token: OR", newline); return OR;}
"true"				{ printf("Line %d Token: BOOLCONST Value: 1 Input: %s\n", newline, yytext); yylval.bval = 1; return BOOLCONST;}
"false"				{ printf("Line %d Token: BOOLCONST Value: 0 Input: %s\n", newline, yytext); yylval.bval = 0; return BOOLCONST;}
"return"			{ printf("Line %d Token: %s\n", newline, yytext); return RETURN;}
"record"			{ printf("Line %d Token: %s\n", newline, yytext); recvar = 1; return RECORD;}
"break"				{ printf("Line %d Token: %s\n", newline, yytext); return BREAK; }
"while"				{ printf("Line %d Token: %s\n", newline, yytext); return WHILE; }
"static"			{ printf("Line %d Token: %s\n", newline, yytext); return STATIC; }
"("					{ printf("Line %d Token: %s\n", newline, yytext); return PARL;}
")"					{ printf("Line %d Token: %s\n", newline, yytext); return PARR;}
"=="				{ printf("Line %d Token: %s\n", newline, yytext); return EQUIV;}
"!="				{ printf("Line %d Token: %s\n", newline, yytext); return NEQUIV;}
"="                 { printf("Line %d Token: %s\n", newline, yytext); return EQUALS;}
"<"                 { printf("Line %d Token: %s\n", newline, yytext); return LSS;}
">"                 { printf("Line %d Token: %s\n", newline, yytext); return GSS;}
"<="                { printf("Line %d Token: %s\n", newline, yytext); return LEQ;}
">="                { printf("Line %d Token: %s\n", newline, yytext); return GEQ;}   	
"+="				{ printf("Line %d Token: %s\n", newline, yytext); return ADDE;}
"-="				{ printf("Line %d Token: %s\n", newline, yytext); return SUBE;}
"*="				{ printf("Line %d Token: %s\n", newline, yytext); return MULE;}
"/="				{ printf("Line %d Token: %s\n", newline, yytext); return DIVE;}
"--"				{ printf("Line %d Token: %s\n", newline, yytext); return DEC;}
"++"				{ printf("Line %d Token: %s\n", newline, yytext); return INC;}
"*"					{ printf("Line %d Token: %s\n", newline, yytext); return MUL;}
"/"					{ printf("Line %d Token: %s\n", newline, yytext); return DIV;}
"+"					{ printf("Line %d Token: %s\n", newline, yytext); return ADD;}
"-"					{ printf("Line %d Token: %s\n", newline, yytext); return SUB;}
"%"					{ printf("Line %d Token: %s\n", newline, yytext); return MOD;}
"?"					{ printf("Line %d Token: %s\n", newline, yytext); return COND;}
"["					{ printf("Line %d Token: %s\n", newline, yytext); return BRACL;}
"]"					{ printf("Line %d Token: %s\n", newline, yytext); return BRACR;}
";"					{ printf("Line %d Token: %s\n", newline, yytext); return SEMI;}
":"					{ printf("Line %d Token: %s\n", newline, yytext); return COLON;}
"."					{ printf("Line %d Token: %s\n", newline, yytext); return DOT;}
"{"					{ printf("Line %d Token: %s\n", newline, yytext); return CURLL;}
"}"					{ printf("Line %d Token: %s\n", newline, yytext); return CURLR;}
","					{ printf("Line %d Token: %s\n", newline, yytext); return COMMA;}
{letter}({letter}|{digit})*		{ 
									/*
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
									*/
									printf("Line %d Token: ID Value: %s\n", newline, yytext);
									yylval.idToken.tokenType = "ID";
									yylval.idToken.IDvalue = strdup(yytext);
									yylval.idToken.lineNumber = newline;
									return ID;
								}
{digit}+						{ 
									yylval.numConstToken.numericalValue = atoi(yytext);
									yylval.numConstToken.stringThatWasTyped = strdup(yytext);
									yylval.numConstToken.tokenType = "NUMCONST";
									yylval.numConstToken.lineNumber = newline;
									printf("Line %d Token: NUMCONST Value: %s\n", newline, yytext);
									return NUMCONST;
								}
'([^\n|\\']|\\[^\0|\n])'		{ 
									yylval.cval = yytext[1];
									if (yytext[1] == '\\') {
										switch (yytext[2]) {
											case 'n':
												yylval.cval = '\n';
												break;
											case 'r':
												yylval.cval = '\r';
												break;
											case '0':
												yylval.cval = '\0';
												break;
											default:
												yylval.cval = yytext[2];
												break; 
										} 
									}
									printf("Line %d Token: CHARCONST Value: %d Input: %s\n", newline, yylval.cval, yytext);
									return CHARCONST;
								}
.								{ yyerror("Invalid or misplaced input character");}
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