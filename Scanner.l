%{
	#include <math.h>
	#ifndef ANALYSEUR_SPARK_H
	#define ANALYSEUR_SPARK_H
	#endif
	#define debug true
	#include "analyseur_spark.h"
%}

%option noyywrap

%%
"procedure" 			{if (debug) printf(" proc(%s)",yytext); return PROCEDURE;}
"is" 				{if (debug) printf(" IS "); return IS;}
"case"				{if (debug) printf("CASE"); return CASE;}
"when others"			{if(debug) printf("WHENOTHERS");return WHENOTHERS; }
"when"				{if(debug) printf("WHEN"); return WHEN; }
"=>"				{if(debug) printf("IMPLIQUE"); return IMPLIQUE;}
"end case"			{if(debug) printf("END CASE"); return ENDCASE;}
"+" 				{if (debug) printf("plus_sign "); return PLUS_SIGN;}
"end if"			{if (debug) printf(" ENDIF "); return ENDIF;}
"end loop"			{if (debug) printf(" ENDLOOP "); return ENDLOOP;}
"in" 				{if (debug) printf("IN "); return IN;}
"begin" 			{if (debug) printf(" BEGIN "); return V_BEGIN;}
"end" 				{if (debug) printf(" END "); return END;}
"package" 			{if (debug) printf(" package (%s)",yytext); return PACKAGE;}
"with" 				{if (debug) printf(" with "); return WITH;}
"use" 				{if (debug) printf(" use "); return USE;}
"String" 			{if (debug) printf(" String "); return STRING;}
"Integer" 			{if (debug) printf(" Integer "); return INTEGER;}
"Float" 			{if (debug) printf(" Float "); return FLOAT;}
"constant" 			{if (debug) printf(" constant "); return CSTE;}
"xor"				{if (debug) printf(" xor "); return XOR;}
"and" 				{if (debug) printf(" and "); return AND;}
"then" 				{if (debug) printf(" then "); return THEN;}
"for" 				{if (debug) printf(" for "); return FOR;}
"while" 			{if (debug) printf(" while "); return WHILE;}
"loop" 				{if (debug) printf(" loop "); return LOOP;}
"reverse" 			{if (debug) printf(" reverse "); return REVERSE;}
"or" 				{if (debug) printf(" or "); return OR;}
"else" 				{if (debug) printf(" else "); return ELSE;}
"if" 				{if (debug) printf(" if "); return IF;}
"elsif" 			{if (debug) printf(" elsif "); return ELSIF;}
"null"				{if (debug) printf(" null "); return V_NULL;}
"--"[^/n]			{if (debug) printf(" Comment ");}
[-+]?[0-9]+ 		{if (debug) printf("NUMERIC(%s) ",yytext); return NUMERIC;}
[-+]?[0-9]+\.[0-9]* {if (debug) printf("DECIMAL(%s) ",yytext); return DECIMAL;}
"/=" 				{if (debug) printf(" diff "); return DIFF;}
":=" 				{if (debug) printf(" assignment "); return ASSIGNMENT;}
"=" 				{if (debug) printf(" equals  "); return EQ;}
"<" 				{if (debug) printf("less_than "); return LESS_THAN;}
">" 				{if (debug) printf("greater_than "); return GREATER_THAN;}
">="				{if (debug) printf("greater_than_eq "); return GREATER_THAN_EQ;}
"<="				{if (debug) printf("less_than_eq "); return LESS_THAN_EQ;}
"," 				{if (debug) printf("comma "); return COMMA;}
".." 				{if (debug) printf("to "); return TO;}
"-" 				{if (debug) printf("hyphen_minus "); return HYPHEN_MINUS;}
"&"			     	{if (debug) printf(" ampersand "); return AMPERSAND;}
"exit" 				{if (debug) printf(" exit "); return EXIT;}
";" 				{if (debug) printf(" PVIRG "); return PVIRG;}
":" 				{if (debug) printf(" TWOPOINTS "); return TWOPOINTS;}
"(" 				{if (debug) printf(" left_parenthesis "); return LEFT_PARENTHESIS;}
")" 				{if (debug) printf(" right_parenthesis "); return RIGHT_PARENTHESIS;}
[\"][^\n]*[\"]			{if (debug) printf(" string_literal "); return STRING_LITERAL;}  
[_a-zA-Z][_a-zA-Z0-9]*  	{if (debug) printf(" IDF(%s) ",yytext); return IDF;}
[ \t\n]+ 
.				    printf("ERR_LEX( %s )\n", yytext);

%%
