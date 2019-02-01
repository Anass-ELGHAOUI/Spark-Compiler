#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "analyseur_spark.h"
#include "tabSymb.h"
#include "error.h"
#define debug true

typetoken token;
bool follow_token;

int main() {
  _read_token();
   if (_program()) {
   	puts("\n---Valide_Syntax --- \n");
   } else {
    puts("\n---Invalide_Syntax  --- \n");
   }
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
	bool result = false;
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
	if(token == USE){
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
	if(_program_specification()){
		_read_token();
		if(token == IS){
			_read_token();
			while(token != V_BEGIN){
				result = _basic_declaration();
				_read_token();
				if(!result) break;	
			}
			if(result){
				if(token == V_BEGIN){
					_read_token();
					if(_suquence_of_statement() ){
						_read_token();
						if(token == END){
							_read_token();
							if(token == IDF){
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
			result = true;		
		}
	}
	if (debug) printf("out_program_specification_statement \n");
	return result;
}

/* 
 _basic_declaration -> id : _type_declaration ; _basic_declaration | $
*/
bool _basic_declaration(){
	if (debug) printf("in_basic_declaration_statement \n");
	bool result = false;
	if(token == IDF){
		_read_token();
		if(token == TWOPOINTS){
			_read_token();
			if(_type_declaration()){
				_read_token();
				if(token == PVIRG){
					result = true;	
//verification autre declaration	
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
	return result;
	if(debug) printf("out_condition \n");
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
	return result;
	if(debug) printf("out_condition_aux");
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
//simple_expression:: _term _simple_expression_aux;
bool _simple_expression(){
	if(debug) printf("in_simple_expression \n");
	bool result=false;
	if(_term()){
		_read_token();
		if(_simple_expression_aux()){
			_read_token();
			follow_token=true;
			if(token == PVIRG){
				result = true;
			}
		}
		
	}
	return result;
	if(debug) printf("out_simple_expression \n");
}
//_simple_expression_aux :: ("+"|"-"|"&")_term | $
bool _simple_expression_aux(){
	if(debug) printf("in_simple_expression_aux\n");
	bool result=false;
	if(token == PLUS_SIGN || token == HYPHEN_MINUS || token == AMPERSAND){
		_read_token();
		if(_term()){
			result = true;
		}
	}else if(token == PVIRG){
		result = true;
		follow_token = true;
	}
	return result;
	if(debug) printf("out_simple_expression_aux\n");
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
				_read_token();
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
/*_case_statement:: "case" IDF "IS" _when_statements _when_others_statment "end Case"  ;
*/

bool _case_statement(){
	if(debug) printf("in_case_statement \n");
	bool result = false;
	if(token == CASE){
		_read_token();
		if(token== IDF){
			_read_token();
			if(token== IS){
				_read_token();
				if(_when_statement()){
					_read_token();
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
	return false;
}

 
