%{
#include <stdio.h>
#include <string.h>
#include "scanType.h"
extern int yylex();

#include "parser.tab.h"

int newline = 1;

int i;
char* rectable[10];
int recvar = 0;
int reccount = 0;

NCT numConstToken;
CCT charConstToken;
BCT boolConstToken;
RECT recordToken;
IDT idToken;
KWT keyWordToken;

%}

digit [0-9]
letter	[a-zA-Z]
letdig [{digit}|{letter}]

%%
[ \t\r]       		;
"//"[^\n]*			;
\'\\n'				{
					yylval.charConstToken.lineNumber = newline;
					yylval.charConstToken.tokenType = "CHARCONST";
					yylval.charConstToken.stringThatWasTyped = strdup("'\\n'");
					yylval.charConstToken.letterData = '\n';
					return CHARCONST;
				}
\n 				{ newline++; }
\'\\0'				{
					yylval.charConstToken.lineNumber = newline;
					yylval.charConstToken.tokenType = "CHARCONST";
					yylval.charConstToken.stringThatWasTyped = strdup("'\\0'");
					yylval.charConstToken.letterData = '\0';
					return CHARCONST;
				}
\'[^\\\n]{1,1}\'		{
					yylval.charConstToken.lineNumber = newline;
					yylval.charConstToken.tokenType = "CHARCONST";
					yylval.charConstToken.stringThatWasTyped = strdup(yytext);
					yylval.charConstToken.letterData = yytext[1];
					return CHARCONST;
				}
\'[\\](.){1,1}\'		{
					yylval.charConstToken.lineNumber = newline;
					yylval.charConstToken.tokenType = "CHARCONST";
					yylval.charConstToken.stringThatWasTyped = strdup(yytext);
					yylval.charConstToken.letterData = yytext[2];
					return CHARCONST;
				}
"if"                		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "IF";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return IF;
				}
"else"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "ELSE";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return ELSE;
				}
"int"           		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "INT";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return INT;
				}
"bool"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "BOOL";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return BOOL;
				}
"char"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "CHAR";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return CHAR;
				}	
"not"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "NOT";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return NOT;
				}
"and"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "AND";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return AND;
				}
"or"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "OR";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return OR;
				}
"true"				{
					yylval.boolConstToken.lineNumber = newline;
					yylval.boolConstToken.tokenType = "BOOLCONST";
					yylval.boolConstToken.stringThatWasTyped = strdup(yytext);
					yylval.boolConstToken.numericalValue = 1;
					return BOOLCONST;
				}
"false"				{
					yylval.boolConstToken.lineNumber = newline;
					yylval.boolConstToken.tokenType = "BOOLCONST";
					yylval.boolConstToken.stringThatWasTyped = strdup(yytext);
					yylval.boolConstToken.numericalValue = 0;
					return BOOLCONST;
				}
"return"			{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "RETURN";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return RETURN;
				}
"record"			{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "RECORD";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					recvar = 1; 
					return RECORD;
				}
"break"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "BREAK";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return BREAK; 
				}
"while"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "WHILE";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return WHILE; 
				}
"static"			{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "STATIC";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return STATIC; 
				}
"("				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "PARL";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return PARL;
				}
")"				{ 
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "PARR";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return PARR;
				}
"!="				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "NEQUIV";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return NEQUIV;
				}
"=="				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "EQUIV";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return EQUIV;
				}
"="                 		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "EQUALS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return EQUALS;
				}
"<"                 		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "LSS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return LSS;
				}
">"                 		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "GSS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return GSS;
				}
"<="                		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "LESSEQ";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return LEQ;
				}
">="                		{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "GRTEQ";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return GEQ;
				}	
"+="				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "ADDASS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return ADDE;
				}
"-="				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "SUBASS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return SUBE;
				}
"*="				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "MULASS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return MULE;
				}
"/="				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "DIVASS";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return DIVE;
				}
"--"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "DEC";
					yylval.keyWordToken.KWTvalue = strdup(yytext);		
					return DEC;
				}
"++"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "INC";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return INC;
				}
"*"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "MUL";
					yylval.keyWordToken.KWTvalue = strdup(yytext);	
					return MUL;
				}
"/"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "DIV";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return DIV;
				}
"+"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "ADD";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return ADD;
				}
"-"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "SUB";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return SUB;
				}
"%"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "MOD";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return MOD;
				}
"?"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "COND";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return COND;
				}
"!"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "NEG";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return NEG;
				}
"["				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "BRACL";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return BRACL;
				}
"]"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "BRACR";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return BRACR;
				}
";"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "SEMI";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return SEMI;
				}
":"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "COLON";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return COLON;
				}
"."				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "DOT";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return DOT;
				}
"{"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "CURLL";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return CURLL;
				}
"}"				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "CULR";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return CURLR;
				}
","				{
					yylval.keyWordToken.lineNumber = newline;
					yylval.keyWordToken.tokenType = "COMMA";
					yylval.keyWordToken.KWTvalue = strdup(yytext);
					return COMMA;
				}
{letter}({letter}|{digit})*	{ 
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
					yylval.idToken.lineNumber = newline;
					yylval.idToken.tokenType = "ID";
					yylval.idToken.IDvalue = strdup(yytext);
					return ID;
				}
{digit}+			{
					yylval.numConstToken.lineNumber = newline;
					yylval.numConstToken.tokenType = "NUMCONST";
					yylval.numConstToken.stringThatWasTyped = strdup(yytext);
					yylval.numConstToken.numericalValue = atoi(yytext);
					return NUMCONST;
				}
.				{ yyerror("Invalid or misplaced input character"); }
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
