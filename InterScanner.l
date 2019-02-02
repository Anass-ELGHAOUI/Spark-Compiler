%{
	#include <math.h>
	#ifndef INTERPRETEUR_SPARK_H
	#define INTERPRETEUR_SPARK_H
	#endif
	#define debug true
	#include "interpreteur.h"
%}

%option noyywrap

%%
"AND" 				{if (debug) printf(" AND "); return ANDOP;}
"XOR" 				{if (debug) printf(" XOR "); return XOROP;}
"ANDN" 				{if (debug) printf(" ANDN "); return ANDNOP;}
"HLT" 				{if (debug) printf(" HLT "); return HLTOP;}
"LEQ" 				{if (debug) printf(" LEQ "); return LEQOP;}
"GEQ"				{if (debug) printf(" GEQ "); return GEQOP;}
"LSS"				{if (debug) printf(" LSS "); return LSSOP;}
[0-9]+ 				{if (debug) printf("NUMERICOP(%s) ",yytext); set_numeric_attribute(atoi(yytext)); return NUMERICOP;}
[0-9]+\.[0-9]* 			{if (debug) printf("DECIMALOP(%s) ",yytext); set_decimal_attribute(atof(yytext)); return DECIMALOP;}
"GTR" 				{if (debug) printf(" GTR "); return GTROP;}
"NE" 				{if (debug) printf(" NE "); return NEOP;}
"EQL" 				{if (debug) printf(" EQL  "); return EQLOP;}
"STO" 				{if (debug) printf(" STO "); return STOOP;}
"ADD" 				{if (debug) printf(" ADD "); return ADDOP;}
"SUB"				{if (debug) printf(" SUB "); return SUBOP;}
"MUL"				{if (debug) printf(" MUL "); return MULOP;}
"DIV" 				{if (debug) printf(" DIV "); return DIVOP;}
"LDV" 				{if (debug) printf(" LDV "); return LDVOP;}
"PRN" 				{if (debug) printf(" PRN "); return PRNOP;}
"INN"			     	{if (debug) printf(" INN "); return INNOP;}
"BZE" 				{if (debug) printf(" BZE "); return BZEOP;}
"BRN" 				{if (debug) printf(" BRN "); return BRNOP;}
"INT" 				{if (debug) printf(" INT "); return INTOP;}
"LDA" 				{if (debug) printf(" LDA "); return LDAOP;}
[\"][^\n]*[\"]			{if (debug) printf(" STRINGOP(%s) ",yytext); set_string_attribute(yytext); return STRINGOP;}  
"LDI"   			{if (debug) printf(" LDI "); return LDIOP;}
[ \t\n]+ 
.				    printf("ERR_LEX( %s )\n", yytext);

%%
