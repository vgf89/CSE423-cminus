/**
 * CSE423 Fall 2017
 * Group: _Za_Worldo_
 */

%{
#include <stdio.h>
#include <string.h>
#include "scanType.h"
extern int yylex();
extern int yyerror();

#include "parser.tab.h"

int newline = 1;

int i;
char* rectable[10];
int recvar = 0;
int reccount = 0;

SPT superToken;

%}

digit [0-9]
letter	[a-zA-Z]
letdig [{digit}|{letter}]

%%
[ \t\r]       		;
"//"[^\n]*			;
\'\\n'				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CHARCONST";
					yylval.superToken.stringThatWasTyped = strdup("'\\n'");
					yylval.superToken.letterData = '\n';
					return CHARCONST;
				}
\n 				{ newline++; }
\'\\0'				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CHARCONST";
					yylval.superToken.stringThatWasTyped = strdup("'\\0'");
					yylval.superToken.letterData = '\0';
					return CHARCONST;
				}
\'[^\\\n]{1,1}\'		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CHARCONST";
					yylval.superToken.stringThatWasTyped = strdup(yytext);
					yylval.superToken.letterData = yytext[1];
					return CHARCONST;
				}
\'[\\](.){1,1}\'		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CHARCONST";
					yylval.superToken.stringThatWasTyped = strdup(yytext);
					yylval.superToken.letterData = yytext[2];
					return CHARCONST;
				}
"if"                		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "IF";
					yylval.superToken.KWTvalue = strdup(yytext);
					return IF;
				}
"else"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "ELSE";
					yylval.superToken.KWTvalue = strdup(yytext);
					return ELSE;
				}
"int"           		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "INT";
					yylval.superToken.KWTvalue = strdup(yytext);
					return INT;
				}
"bool"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "BOOL";
					yylval.superToken.KWTvalue = strdup(yytext);
					return BOOL;
				}
"char"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CHAR";
					yylval.superToken.KWTvalue = strdup(yytext);
					return CHAR;
				}	
"not"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "NOT";
					yylval.superToken.KWTvalue = strdup(yytext);
					return NOT;
				}
"and"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "AND";
					yylval.superToken.KWTvalue = strdup(yytext);
					return AND;
				}
"or"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "OR";
					yylval.superToken.KWTvalue = strdup(yytext);
					return OR;
				}
"true"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "BOOLCONST";
					yylval.superToken.stringThatWasTyped = strdup(yytext);
					yylval.superToken.numericalValue = 1;
					return BOOLCONST;
				}
"false"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "BOOLCONST";
					yylval.superToken.stringThatWasTyped = strdup(yytext);
					yylval.superToken.numericalValue = 0;
					return BOOLCONST;
				}
"return"			{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "RETURN";
					yylval.superToken.KWTvalue = strdup(yytext);
					return RETURN;
				}
"record"			{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "RECORD";
					yylval.superToken.KWTvalue = strdup(yytext);
					recvar = 1; 
					return RECORD;
				}
"break"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "BREAK";
					yylval.superToken.KWTvalue = strdup(yytext);
					return BREAK; 
				}
"while"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "WHILE";
					yylval.superToken.KWTvalue = strdup(yytext);
					return WHILE; 
				}
"static"			{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "STATIC";
					yylval.superToken.KWTvalue = strdup(yytext);
					return STATIC; 
				}
"("				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "PARL";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return PARL;
				}
")"				{ 
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "PARR";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return PARR;
				}
"!="				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "NOTEQ";
					yylval.superToken.KWTvalue = strdup(yytext);
					return NOTEQ;
				}
"=="				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "EQ";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return EQ;
				}
"<="                		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "LESSEQ";
					yylval.superToken.KWTvalue = strdup(yytext);
					return LEQ;
				}
">="                		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "GRTEQ";
					yylval.superToken.KWTvalue = strdup(yytext);
					return GEQ;
				}	
"+="				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "ADDASS";
					yylval.superToken.KWTvalue = strdup(yytext);
					return ADDE;
				}
"-="				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "SUBASS";
					yylval.superToken.KWTvalue = strdup(yytext);
					return SUBE;
				}
"*="				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "MULASS";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return MULE;
				}
"/="				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "DIVASS";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return DIVE;
				}
"="                 		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "EQUALS";
					yylval.superToken.KWTvalue = strdup(yytext);
					return EQUALS;
				}
"<"                 		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "LSS";
					yylval.superToken.KWTvalue = strdup(yytext);
					return LSS;
				}
">"                 		{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "GSS";
					yylval.superToken.KWTvalue = strdup(yytext);
					return GSS;
				}
"--"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "DEC";
					yylval.superToken.KWTvalue = strdup(yytext);		
					return DEC;
				}
"++"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "INC";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return INC;
				}
"*"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "MUL";
					yylval.superToken.KWTvalue = strdup(yytext);	
					return MUL;
				}
"/"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "DIV";
					yylval.superToken.KWTvalue = strdup(yytext);
					return DIV;
				}
"+"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "ADD";
					yylval.superToken.KWTvalue = strdup(yytext);
					return ADD;
				}
"-"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "SUB";
					yylval.superToken.KWTvalue = strdup(yytext);
					return SUB;
				}
"%"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "MOD";
					yylval.superToken.KWTvalue = strdup(yytext);
					return MOD;
				}
"?"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "COND";
					yylval.superToken.KWTvalue = strdup(yytext);
					return RAND;
				}
"!"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "NEG";
					yylval.superToken.KWTvalue = strdup(yytext);
					return NEG;
				}
"["				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "BRACL";
					yylval.superToken.KWTvalue = strdup(yytext);
					return BRACL;
				}
"]"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "BRACR";
					yylval.superToken.KWTvalue = strdup(yytext);
					return BRACR;
				}
";"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "SEMI";
					yylval.superToken.KWTvalue = strdup(yytext);
					return SEMI;
				}
":"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "COLON";
					yylval.superToken.KWTvalue = strdup(yytext);
					return COLON;
				}
"."				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "DOT";
					yylval.superToken.KWTvalue = strdup(yytext);
					return DOT;
				}
"{"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CURLL";
					yylval.superToken.KWTvalue = strdup(yytext);
					return CURLL;
				}
"}"				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "CULR";
					yylval.superToken.KWTvalue = strdup(yytext);
					return CURLR;
				}
","				{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "COMMA";
					yylval.superToken.KWTvalue = strdup(yytext);
					return COMMA;
				}
{letter}({letter}|{digit})*	{ 
					if (recvar == 1) {
						//printf("adding \"%s\" to record list\n", yytext);
						rectable[reccount] = strdup(yytext);
						reccount++;
						recvar = 0;
						yylval.superToken.lineNumber = newline;
						yylval.superToken.tokenType = "ID";
						yylval.superToken.IDvalue = strdup(yytext);
						return ID;
					}
					for(i = 0; i < reccount; i++) {
						//printf("looking for a match of \"%s\"...\n", yytext);
						//printf("%s\n", rectable[i]);
						if (strcmp(yytext, rectable[i]) == 0) {
							//printf("found \"%s\" in record types!\n", yytext);
							yylval.superToken.lineNumber = newline;
							yylval.superToken.tokenType = "RECTYPE";
							yylval.superToken.stringThatWasTyped = strdup(yytext);
							return RECTYPE;
						} 
					}
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "ID";
					yylval.superToken.IDvalue = strdup(yytext);
					return ID;
				}
{digit}+			{
					yylval.superToken.lineNumber = newline;
					yylval.superToken.tokenType = "NUMCONST";
					yylval.superToken.stringThatWasTyped = strdup(yytext);
					yylval.superToken.numericalValue = atoll(yytext);
					return NUMCONST;
				}
.				{ yyerror("Invalid or misplaced input character"); }
%%
