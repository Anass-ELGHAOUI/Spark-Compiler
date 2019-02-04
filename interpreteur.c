#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "interpreteur.h"
#define debug true

pseudotoken token;


stringvaluetype stringvalue;
numericvaluetype numericvalue;
decimalvaluetype decimalvalue;

/* grammar 
pseudocode : pseudo_Item pseudo_Aux END
pseudo_Aux : pseudocode | epsilon
pseudoItem : data_item | data_attribute_item
data_item :  "AND" | "XOR" | "ANDN" | "HLT" | "LEQ" | "GEQ" | "LSS" | "GTR" | "NE" | "EQL" | "STO" | "ADD" | "SUB" | "MUL" | "DIV" | "LDV" | "PRN" |  | "INN"

data_attribute_item : ("BZE" | "BRN" | "INT" | "LDA" | "LDI") type_operation
		
type_operation : NUMERICOP | DECIMALOP | STRINGOP
*/


int main() {
	_read_token();
	if (_pseudocode()) {
		printf("\n---Valide_Pseudo_Code --- \n");
	} else {
		printf("\n---Invalide_Pseudo_Code  --- \n");
	}
	
	return 0;
}

void _read_token(){
	
	token=(pseudotoken)yylex();
}

bool _pseudocode(){
	bool result = false;
	bool resulttmp = true;
	if(token == NUMERICOP){
		_read_token();
		while(token != HLTOP){
			resulttmp = _pseudo_item();
			_read_token();
			if(token != NUMERICOP) resulttmp = false;
			_read_token();
			if(!resulttmp) break;
		}
	}
	if(resulttmp)
		result = true;

	return result;
}

bool _pseudo_item(){
	bool result = false;
	result = _data_item();
	if(!result)
		result = _data_attribute_item();
	
	return result;
}

bool _data_item(){
	bool result = false;
	if(token == ANDOP || token == XOROP || token == ANDNOP  || token == LEQOP || token == GEQOP || token == LSSOP || token == GTROP || token == NEOP || token == EQLOP || token == STOOP || token == ADDOP || token == SUBOP || token == MULOP || token == DIVOP || token == LDVOP || token == PRNOP || token == INNOP)
		result = true;
	return result;
}
/*
data_attribute_item : ("BZE" | "BRN" | "INT" | "LDA" | "LDI") type_operation
		
type_operation : NUMERICOP | DECIMALOP | STRINGOP
*/
bool _data_attribute_item(){
	bool result = false;
	if(token == BZEOP || token == BRNOP || token == INTOP || token == LDAOP || token == LDIOP){
		_read_token();
		if(_type_operation())
			result = true;
	}
	return result;
}

bool _type_operation(){
	bool result = false;
	if(token == NUMERICOP || token == DECIMALOP || token == STRINGOP)
		result = true;
	return result;
}

void set_numeric_attribute(int numeric){
	if(debug) printf("in_set_numeric_att\n");

	if(debug) printf("out_set_numeric_att\n");
}
void set_decimal_attribute(float decimal){
	if(debug) printf("in_set_decimal_att\n");

	if(debug) printf("out_set_decimal_att\n");
}
void set_string_attribute(char* string){
	if(debug) printf("in_set_string_att\n");
	
	if(debug) printf("out_set_string_att\n");
}

