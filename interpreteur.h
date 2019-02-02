#include <stdio.h>
#include <stdbool.h>

/* grammar 
pseudocode : pseudo_Item pseudo_Aux END
pseudo_Aux : pseudocode | epsilon
pseudoItem : data_item | data_attribute_item
data_item :  "AND" | "XOR" | "ANDN" | "HLT" | "LEQ" | "GEQ" | "LSS" | "GTR" | "NE" | "EQL" | "STO" | "ADD" | "SUB" | "MUL" | "DIV" | "LDV" | "PRN" |  | "INN"

data_attribute_item : ("BZE" | "BRN" | "INT" | "LDA" | "LDI") type_operation
		
type_operation : NUMERICOP | DECIMALOP | STRINGOP
*/

typedef enum {

	ANDOP,
	XOROP,
	ANDNOP,
	HLTOP,
	OLEQOP,
	GEQOP,
	LSSOP,
	LEQOP,
	GTROP,
	NEOP,
	EQLOP,
	STOOP,
	ADDOP,
	SUBOP,
	MULOP,
	DIVOP,
	LDVOP,
	PRNOP,
	INNOP,
	BZEOP,
	BRNOP,
	INTOP,
	LDAOP,
	LDIOP,
	NUMERICOP,
	DECIMALOP,
	STRINGOP

} pseudotoken;

typedef struct{
	char* value;
}stringvaluetype;

typedef struct{
	int value;
}numericvaluetype;

typedef struct{
	float value;
}decimalvaluetype;

extern int yylex();

void _read_token();
bool _pseudocode();
bool _pseudo_item();
bool _data_item();
bool _data_attribute_item();
bool _type_operation();

void set_numeric_attribute(int numeric);
void set_decimal_attribute(float decimal);
void set_string_attribute(char* string);
