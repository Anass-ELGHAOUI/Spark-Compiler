#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "analyseur_spark.h"
#include "error.h"
#include "tabSymb.h"
#define debug true

typetoken token;
bool follow_token;

idfvaluetype idfvalue;
stringvaluetype stringvalue;
linevalue linenumber;
numericvaluetype numericvalue;
decimalvaluetype decimalvalue;


char* nameProc;
//variable used to save a new symbole
static tabSymb symbTmp;
//variable used to save a new error
static error* errorTmp;
//_program()
int main() {
	linenumber.line = 1;
	errorTmp = (error*)malloc(sizeof(error));

	_read_token();
	if (_program() && getNbrErrors() == 0) {
		puts("\n---Valide_Syntax --- \n");
	} else {
		puts("\n---Invalide_Syntax  --- \n");
		showErrors();
	}
	showAllSymbols();
	return 0;
}

void _read_token(){
	if(follow_token){
		follow_token=false;
	}
	else {
		token=(typetoken)yylex();
	}
}
//_program -> _use_clause _program_body
bool _program(){
	if (debug) printf("in_program_statement \n");
	bool result = true;
	while(token != PROCEDURE){
		result = _use_clause();
		_read_token();
		if(!result) break;
	}
	if(result){
		if(_body_program()){
			result = true;		
		}
	}
	if (debug) printf("out_program_statement \n");
	return result;
}
//_use_clause -> ("use" | "with")  id _use_clause | $;
bool _use_clause(){
	if (debug) printf("in_use_clause_statement \n");
	bool result = false;
	if(token == USE || token == WITH){
		_read_token();
		if(token == IDF){
			_read_token();
			if(token == PVIRG){
				result = true;	
				//_use_clause();
			}		
		}
	
	}else if(token == PROCEDURE){
		follow_token = true;
		result = true;
	}
	if (debug) printf("out_use_clause_statement \n");
	return result;
}
//_program_body -> _program_specification "is" _basic_declaration "begin" _sequence_of_statements "end" id;
bool _body_program(){
	if (debug) printf("in_body_program_statement \n");
	bool result = false;
	bool resulttmp = true;
	if(_program_specification()){
		_read_token();
		if(token == IS){
			_read_token();
			while(token != V_BEGIN){
				resulttmp = _basic_declaration();
				_read_token();
				if(!resulttmp) break;	
			}
			if(resulttmp){
				if(token == V_BEGIN){
					_read_token();
					if(_suquence_of_statement()){
						_read_token();
						if(token == END){
							_read_token();
							if(token == IDF){
								if(debug) printf("name proc : %s\n",nameProc);
								if(strcmp(nameProc,idfvalue.value)!=0){
									strcpy(errorTmp->msgError,"incompatible name of procedure");
									errorTmp->line = linenumber.line;
									errorTmp->next=NULL;
									addOnTabError(errorTmp);
								} 
								_read_token();
								if(token == PVIRG){
									result = true;		
								}	
							}		
						}
					}	
				}
			}		
		}
	}
	if (debug) printf("out_body_program_statement \n");
	return result;
}
//_program_specification -> "procedure" id | "function" id "return" id;
bool _program_specification(){
	if (debug) printf("in_program_specification_statement \n");
	bool result = false;
	if(token == PROCEDURE){
		_read_token();
		if(token == IDF){
			nameProc = (char*)malloc(sizeof(char)*strlen(idfvalue.value)+1);
			strcpy(nameProc,idfvalue.value);
			if(debug) printf("name proc : %s\n",nameProc);
			result = true;		
		}
	}
	if (debug) printf("out_program_specification_statement \n");
	return result;
}

/* 
 _basic_declaration -> id ":" ["constant"] _type_declaration [":=" _term] ";" _basic_declaration | $
*/
bool _basic_declaration(){
	if (debug) printf("in_basic_declaration_statement \n");
	bool result = false;
	symbTmp.isCste = false;
	symbTmp.adresse = getNbrSymb();
	if(token == IDF){
		strcpy(symbTmp.name,idfvalue.value);
		if(isInTabSymb(symbTmp.name)){
			strcpy(errorTmp->msgError,"variable redefined");
			errorTmp->line = linenumber.line;
			errorTmp->next=NULL;
			addOnTabError(errorTmp);
		}
		_read_token();
		if(token == TWOPOINTS){
			_read_token();
			if(token == CSTE){
				_read_token();
				symbTmp.isCste = true;
			}
			if(_type_declaration()){
				symbTmp.type = token;
				_read_token();
				if(token == ASSIGNMENT){
					_read_token();
					if(_term()){
						typetoken tokentmp = token;
						_read_token();
						if(token == PVIRG){
							result = true;
							if(addOnTabSymb(symbTmp)){
								switch(tokentmp){
									case NUMERIC : addIntValueOfSymb(symbTmp,numericvalue.value); break;
									case DECIMAL : addFloatValueOfSymb(symbTmp,decimalvalue.value); break;
									case STRING_LITERAL : addStringValueOfSymb(symbTmp,stringvalue.value); break;
								}
							}			
						}
					}
				}else if(token == PVIRG){
					if(symbTmp.isCste){
						strcpy(errorTmp->msgError,"constant attribute must be initialized");
						errorTmp->line = linenumber.line;
						errorTmp->next=NULL;
						addOnTabError(errorTmp);
					}
					result = true;	
					addOnTabSymb(symbTmp);	
				}	
			}	
		}
	}else if(token == V_BEGIN){
		follow_token = true;
		result = true;
	}
	if (debug) printf("out_basic_declaration_statement \n");
	return result;
}

bool _type_declaration(){
	if (debug) printf("in_type_declaration_statement \n");
	bool result = false;
	if(token == STRING || token == INTEGER || token == FLOAT){
		result = true;
	}
	if (debug) printf("out_type_declaration_statement \n");
	return result;
}

/*
   boucle_statements -> for_statements | while_statements | loop_statements
 */
bool _boucle_statements(){
	if (debug) printf("in_boucle_statement \n");
	bool result = false;
	if(_for_statements() || _while_statements() || _loop_statements()){
		result = true;
	}
	if (debug) printf("out_boucle_statement \n");
	return result;
	
}

/*
   for_statements -> FOR IDF IN [REVERSE] NUMERIC TO NUMERIC loop_statements
 */
bool _for_statements(){
	if (debug) printf("in_for_statement \n");
	bool result = false;
	if(token == FOR){
		_read_token();
		if(token == IDF){
			_read_token();
			if(token == IN){
				_read_token();
				if(token == REVERSE){
					_read_token();
				}
				if(token == NUMERIC){
					_read_token();
					if(token == TO){
						_read_token();
						if(token == NUMERIC){
							_read_token();
							if(_loop_statements()){
								result=true;							
							}
						}
					}				
				}
			}
		}
	}
	if (debug) printf("out_for_statement \n");
	return result;
	
}

/*
   loop_statements -> LOOP _sequence_of_statements END LOOP PVIRG
 */
bool _loop_statements(){
	if (debug) printf("in_loop_statement \n");
	bool result = false;
	if(token == LOOP){
		_read_token();
		if(_suquence_of_statement()){
			_read_token();
			if(token == ENDLOOP){
				_read_token();
				if(token == PVIRG){
						result = true;			
				}
			}
		}
	}
	if (debug) printf("out_loop_statement \n");
	return result;
	
}

/*
   while_statements -> WHILE _condition loop_statements
 */
bool _while_statements(){
	if (debug) printf("in_while_statement \n");
	bool result = false;
	if(token == WHILE){
		_read_token();
		if(_condition()){
			_read_token();
			if(_loop_statements()){
				result = true;
			}
		}
	}
	if (debug) printf("out_while_statement \n");
	return result;
	
}

/* 
_if_statement -> "if" _condition "then" _sequence_of_statements _elsif_statements _if_aux
_elsif_statements -> "elsif" _sequence_of_statements _elsif_statements | $
_if_aux  ->  "else" _sequence_of_statements | "end if"
*/
bool _if_statement() {
	if (debug) printf("in_if_statement \n");
	bool result = false;
	if(token == IF) {
		_read_token();
		if(_condition()) {
			_read_token();
			if(token == THEN) {
				_read_token();
				if(_suquence_of_statement()) {
					_read_token();
//elseis statement
					//if(_if_aux()) {
					if(token == ENDIF) {
						result = true;
					}
				}	
			}
		}
	}
	if (debug) printf("out_if_statement \n");
	return result;
}

/* 
_if_statement -> "if" _condition "then" _sequence_of_statements _elsif_statements _if_aux "end if"
_elsif_statements -> "elsif" _condition "then" _sequence_of_statements _elsif_statements | $
_if_aux  ->  "else" _sequence_of_statements | $
*/
bool _elsif_statement() {
	if (debug) printf("in_elsif_statement \n");
	bool result = false;
	if(token == ELSIF) {
		_read_token();
		if(_condition()) {
			_read_token();
			if(token == THEN) {
				_read_token();
				if(_suquence_of_statement()) {
					result = true;
				}	//if(_elseif_statements)
			}
		}
	}else if(token == ELSE){
		follow_token = true;
		result = true;
	}else if(token == ENDIF){
		follow_token = true;
		result = true;
	}
	if (debug) printf("out_elsif_statement \n");
	return result;
}

/* 
_if_statement -> "if" _condition "then" _sequence_of_statements _elsif_statements _if_aux "end if"
_elsif_statements -> "elsif" _condition "then" _sequence_of_statements _elsif_statements | $
_if_aux  ->  "else" _sequence_of_statements | $
*/

bool _ifaux_statement() {
	if (debug) printf("in_ifaux_statement \n");
	bool result = false;
	if(token == ELSE) {
		_read_token();
		if(_suquence_of_statement()) {
			result = true;
		}
	}else if(token == ENDIF){
		follow_token = true;
		result =  true;
	}
	if (debug) printf("out_ifaux_statement \n");
	return result;
}

/* 
condition:: _relation relation_aux 
relation_aux:: {"and" relation}  | {"and" "then" relation} | {"or" relation} | {"or" "then" relation} | 	       {"xor" relation} | epsilon
*/
/*
bool _condition() {
	if (debug) printf("in_condition \n");
	bool result = false;
		if(_relation()){
		
		result = true;
		if (debug) printf("out_condition \n");
		return result;
		_read_token();
		if(token == AND || token == OR || token == XOR) {
			_read_token();
			if(_relation()) {
				result = true;
			}
			else if(token == THEN) {
				_read_token();
				if(_relation()) {
					result = true;
				}
			}
		}
	}
	if (debug) printf("out_condition \n");
	return result;
}
*/

// condition :: _relation _condition_aux;

bool _condition(){
	if(debug) printf("in_condition \n");
	bool result=false;
	if(_relation()){
		_read_token();
		if(_relation_aux){
			result=true;
		}
	}
	if(debug) printf("out_condition \n");
	return result;
}

// condition_aux :: ("and"["then"]|"or"["then"]|"xor") _relation | $
// follow condition_aux = follow condition = {"then" , ";" , fist(loop)={loop} }
bool _condition_aux(){
	if(debug) printf("in_condition_aux");
	bool result=false;
	if(token == AND || token == OR){
		if(token == THEN){
			_read_token();	
		}
		if(_relation()){
			result = true;
		}
	}else if(token == XOR){
		if(_relation()){
			result = true;
		}
	}else if(token == THEN | token == PVIRG | token == LOOP ){
			result = true;
			follow_token= true;
	}
	if(debug) printf("out_condition_aux");
	return result;
}

/* 
relation:: simple_expression (=, /=, >, >=, <, <=) simple_expression
		   | simple_expression [not] "in" (range | subtype_mark)
*/
bool _relation() {
	if (debug) printf("in_relation \n");
	bool result = false;
	if(_simple_expression()) {
		_read_token();
		if(token == DIFF || token == EQ || token == LESS_THAN || token == GREATER_THAN || 
			token == LESS_THAN_EQ || token == GREATER_THAN_EQ) {
			_read_token();
			if(_simple_expression()) {
				result = true;
			}
		}
	}
	if (debug) printf("out_relation \n");
	return result;
}

/* 
simple_expression:: [("+" | "-")] _term simple_expression_aux ---> {("+" | "-" | "&") term} | epsilon
*/
/*
bool _simple_expression() {
	if (debug) printf("in_simple_expression \n");
	bool result = false;
	//if(token == PLUS_SIGN || token == HYPHEN_MINUS){
		//_read_token();
		if(_term()) {
			result = true;
			if (debug) printf("out_simple_expression \n");
			return result;
			_read_token();
			if(token == PLUS_SIGN || token == HYPHEN_MINUS || token == AMPERSAND) {
				_read_token();
				if(_term()) {
					result = true;
				}
			}
		}
	if (debug) printf("out_simple_expression \n");
	return result;
}
*/
//simple_expression:: _term _simple_expression_aux
bool _simple_expression(){
	if(debug) printf("in_simple_expression \n");
	bool result=false;
	bool resultSimpleExp=true;
	
	if(_term()){
		
		_read_token();
		while(token == PLUS_SIGN || token == HYPHEN_MINUS || token == AMPERSAND){
			resultSimpleExp=_simple_expression_aux(); 
			_read_token();
			if(!resultSimpleExp) break;
		}
		if(resultSimpleExp){
			result=true;
		}
		
	}
	if(debug) printf("out_simple_expression \n");
	return result;
}
/* 
relation:: simple_expression (=, /=, >, >=, <, <=) simple_expression
		   | simple_expression [not] "in" (range | subtype_mark)
*/
//_simple_expression_aux :: ("+"|"-"|"&")_term  _simple_expression_aux| $
//follows simple_expression_aux = follows simple_express ={";"}
bool _simple_expression_aux(){
	if(debug) printf("in_simple_expression_aux\n");
	bool result=false;
	if(token == PLUS_SIGN || token == HYPHEN_MINUS || token == AMPERSAND){
		_read_token();
		if(_term()){
			result = true;
		}
	}else if(token == PVIRG || token == DIFF || token == EQ || token == LESS_THAN || token == LESS_THAN_EQ || token == GREATER_THAN || token == GREATER_THAN_EQ || token == IN){
		result = true;
		follow_token = true;
	}
	if(debug) printf("out_simple_expression_aux\n");
	return result;
}
/*
term:: 
 */
bool _term() {
	if (debug) printf("in_term \n");
		bool result = false ;
	if( token == V_NULL | token == NUMERIC | token == DECIMAL | token == STRING_LITERAL | token == IDF) {
		result = true ;
	}
	if (debug) printf("out_term \n");
	return result;
}

/*
suquence_of_statement:: statement suquence_of_statement | epsilon
 */
 
bool _suquence_of_statement() {
	if (debug) printf("in_suquence_of_statement \n");
	bool result = false;
	if(_null_statement() || _assignement_statement() || _exit_statement() || _boucle_statements()||_case_statement()){
		result = true;
	}
	if (debug) printf("out_suquence_of_statement \n");
	return result;
}

/*
null_statement:: "null" ";"
*/

bool _null_statement() {
	if (debug) printf("in_null_statement \n");
	bool result = false;
	if(token == V_NULL) {
		_read_token();
		if(token == PVIRG) {
			result = true;
		}
	}
	if (debug) printf("out_null_statement \n");
	return result;
}

/*
assignement_statement:: IDF ":=" expression ";"   // expression --> condition
*/

bool _assignement_statement() {
	if (debug) printf("in_assignement_statement \n");
	bool result = false;
	if(token == IDF) {
		_read_token();
		if(token == ASSIGNMENT) {
			_read_token();
			if(_simple_expression()) {
				if(token == PVIRG) {
					result = true;
				}
			}
			
		}
	}
	if (debug) printf("out_assignement_statement \n");
	return result;
}

/*
exit_statement:: "exit" _exit_aux ";"
*/

bool _exit_statement() {
	if (debug) printf("in_exit_statement \n");
	bool result = false;
	if(token == EXIT) {
		_read_token();
		if(token == PVIRG) {
			result = true;
		}
	}
	if (debug) printf("out_exit_statement \n");
	return result;
}
/*_case_statement:: "case" IDF "IS" {_when_statements}* _when_others_statment "end Case"  ;
*/

bool _case_statement(){
	if(debug) printf("in_case_statement \n");
	bool result = false;
	bool resultWhen=true;
	if(token == CASE){
		_read_token();
		if(token== IDF){
			_read_token();
			if(token== IS){
				_read_token();
				while(token == WHEN){ resultWhen=_when_statement();	_read_token();}
				if(resultWhen){
					if(_when_others_statement()){
						_read_token();
						if(token== ENDCASE){
							_read_token();
							if(token==PVIRG){
								result = true;
							}
						}
					}
				}
			}
		}
	}
	if(debug) printf("out_in_case_statment\n");
	return result;
}

/* _when_statement :: "when" _term "=>" _sequence_of_statments | $
*/

bool _when_statement(){
	if(debug) printf("in_when_statment\n");
	bool result=false;
	if(token== WHEN){
		_read_token();
		if(_term()){
			_read_token();
			if(token== IMPLIQUE){
				_read_token();
				if(_suquence_of_statement()){
					result=true;
				}
			}
		}
	}else if(token== WHENOTHERS){
		result=true;
		follow_token=true;
	}
	if(debug) printf("out_when_statement\n");
	return result;
}
// _when_others_statement :: "when others" "=>" _sequence_of_statements _exit_statement
bool _when_others_statement(){
	if(debug) printf("in_when_others_statement\n");
	bool result=false;
	if(token == WHENOTHERS){
		_read_token();
		if(token == IMPLIQUE){
			_read_token();
			if(_suquence_of_statement()){
				_read_token();
				if(_exit_statement()){
					result=true;
				}
			}
		}
	}
	if(debug) printf("out_when_others_statement\n");
	return result;
}


void set_idf_attribute(char * idf){
	if(debug) printf("in_set_idf_att\n");
	idfvalue.value = (char*)malloc(sizeof(char)*strlen(idf)+1);
	strcpy(idfvalue.value ,idf);
	if(debug) printf("out_set_idf_att\n");
}
void set_line_attribute(int line){
	if(debug) printf("in_set_line_att\n");
	linenumber.line = line;
	if(debug) printf("out_set_line_att\n");
}
void set_numeric_attribute(int numeric){
	if(debug) printf("in_set_numeric_att\n");
	numericvalue.value = numeric;
	if(debug) printf("out_set_numeric_att\n");
}
void set_decimal_attribute(float decimal){
	if(debug) printf("in_set_decimal_att\n");
	decimalvalue.value = decimal;
	if(debug) printf("out_set_decimal_att\n");
}
void set_string_attribute(char* string){
	if(debug) printf("in_set_string_att\n");
	stringvalue.value = (char*)malloc(sizeof(char)*strlen(string)+1);
	strcpy(stringvalue.value ,string);
	if(debug) printf("out_set_string_att\n");
}



 
