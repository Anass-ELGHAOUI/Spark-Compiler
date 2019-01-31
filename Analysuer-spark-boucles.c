#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "analyseur_spark.h"
#define debug true

typetoken token;
bool follow_token;

int main() {
  _read_token();
   if (_boucle_statements()) {
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
_if_aux  ->  "else" _sequence_of_statements 
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
				}	
			}
		}
	}else if(token == ELSE){
		follow_token = true;
		return true;
	}else if(token == ENDIF){
		follow_token = true;
		return true;
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
		return true;
	}
	if (debug) printf("out_ifaux_statement \n");
	return result;
}

/* 
condition:: _relation relation_aux 
relation_aux:: {"and" relation}  | {"and" "then" relation} | {"or" relation} | {"or" "then" relation} | 	       {"xor" relation} | epsilon
*/
bool _condition() {
	if (debug) printf("in_condition \n");
	bool result = false;
		if(_relation()){
		/* 7di m3a hada */
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
	if(_null_statement() || _assignement_statement() || _exit_statement()){
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

 
