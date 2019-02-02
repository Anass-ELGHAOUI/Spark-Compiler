#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "analyseur_spark.h"
#include "error.h"
#include "tabSymb.h"
#include "generateurCode.h"
#define debug true

typetoken token;
bool follow_token;

idfvaluetype idfvalue;
stringvaluetype stringvalue;
linevalue linenumber;
numericvaluetype numericvalue;
decimalvaluetype decimalvalue;

static bool isInBodyPart = false;
static bool isInAssignementPart = false;
static typetoken assignementType;


char* nameProc;
//variable used to save a new symbole
static tabSymb symbTmp;
//variable used to save a new error
static error* errorTmp;
//_program()
int main() {
	linenumber.line = 1;
	errorTmp = 			(error*)malloc(sizeof(error));

	_read_token();
	if (_program()) {
		puts("\n---Valide_Syntax --- \n");
	} else {
		puts("\n---Invalide_Syntax  --- \n");
	}
	showErrors();
	showAllSymbols();
	printf("\t--PCode--\n");
	displayAllPcode();
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
	bool resulttmp = true;
	while(token != PROCEDURE){
		resulttmp = _use_clause();
		_read_token();
		if(!resulttmp) break;
	}
	if(resulttmp){
		if(_body_program()){
			//generer
			genererMiInst(HLT);
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
					isInBodyPart = true;
					_read_token();
					if(_suquence_of_statement()){
						_read_token();
						if(token == END){
							isInBodyPart = false;
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
					if(token == NUMERIC | token == DECIMAL | token == STRING_LITERAL | token == IDF) {
						//generer
						genererInstInt(LDA,symbTmp.adresse);
					}
					if(_term()){
						
						typetoken tokentmp = token;
						if((symbTmp.type == INTEGER && tokentmp != NUMERIC) || (symbTmp.type == FLOAT && tokentmp != DECIMAL) || (symbTmp.type == STRING && 							tokentmp != STRING_LITERAL)){
							strcpy(errorTmp->msgError,"variable type and value affected are different");
							errorTmp->line = linenumber.line;
							errorTmp->next=NULL;
							addOnTabError(errorTmp);
						}
						_read_token();
						if(token == PVIRG){
							//generer
							genererMiInst(STO);
							result = true;
							if(addOnTabSymb(symbTmp)){
								switch(tokentmp){
									case NUMERIC : addIntValueOfSymb(symbTmp.adresse,numericvalue.value); break;
									case DECIMAL : addFloatValueOfSymb(symbTmp.adresse,decimalvalue.value); break;
									case STRING_LITERAL : addStringValueOfSymb(symbTmp.adresse,stringvalue.value); break;
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
/*
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
*/
//_if_statement -> "if" _condition "then" _sequence_of_statment [_elsif_statment]* [_else_statement] "end if" ";"
bool _if_statement(){
	if (debug) printf("in_if_statement");
	bool result = false;
	bool resultTmp = true;
	if(token == IF){
		_read_token();
		if(_condition()){
			_read_token();
			//generer 
			genererMiInst(BZE);
			int sauv=getCurrentIndexPile();
			if(token == THEN){
				_read_token();
				if(_suquence_of_statement()){
					_read_token();
					//generer
					genererMiInst(BRN);
					int sauvEndIf=getCurrentIndexPile();
					tabCode[sauv].type=INTEGER;
					tabCode[sauv].paramI.intValue=getCurrentIndexPile()+1;

					while(token == ELSIF){resultTmp=_elsif_statement();_read_token();}
					if(token == ELSE  && resultTmp ){resultTmp=_ifaux_statement(); _read_token();}
					if(token == ENDIF && resultTmp ){
						_read_token();
							
						if(token == PVIRG){
							//generer
							int i=sauvEndIf;
							for(;i<getCurrentIndexPile()+1;i++){
								if(tabCode[i].inst==BRN){
									tabCode[i].type=INTEGER;
									tabCode[i].paramI.intValue=getCurrentIndexPile()+1;
								}
							}
							
							result=true;

						}
					}
				}
			}
		}
	}
	
	if (debug)printf("out_if_statement");
	return result;
}

//_elsif_statement :: ELSIF _condition then _statement
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
			//generer 
			genererMiInst(BZE);
			int sauv=getCurrentIndexPile();
			if(token == THEN) {
				_read_token();
				if(_suquence_of_statement()) {
					_read_token();
					//generer
					genererMiInst(BRN);
					tabCode[sauv].type=INTEGER;
					tabCode[sauv].paramI.intValue=getCurrentIndexPile()+1;
					
					if(token == ELSE || token == ENDIF || token == ELSIF){
						follow_token = true;
						result = true;
					}
				}	
			}
		}
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
			_read_token();
			if(token == ENDIF){
				follow_token = true;
				result =  true;
			}
		}
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
	
		if(_condition_aux()){
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
		//generer
		typetoken tokenTmp=token;
		_read_token();
		if(token == THEN){
			_read_token();	
		}
		if(_relation()){
			result = true;
		}
		//generer
		if(token == AND){genererMiInst(AD);}
		else{genererMiInst(OR);}
	}else if(token == XOR){
		//generer
		typetoken tokenTmp=token;
		_read_token();
		if(_relation()){
			result = true;
		}
		//generer
		genererMiInst(XR);
	}else if(token == THEN | token == PVIRG | token == LOOP ){
			result = true;
			follow_token= true;
	}
	if(debug) printf("out_condition_aux\n");
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
			//generer
			typetoken tokenTmp=token;
			_read_token();
			if(_simple_expression()) {
				result = true;
				//generer
				if(tokenTmp == DIFF){genererMiInst(NE);}
				else if(tokenTmp == EQ){genererMiInst(EQL);}
				else if(tokenTmp == LESS_THAN){genererMiInst(LSS);}
				else if(tokenTmp == GREATER_THAN){genererMiInst(GTR);}
				else if(tokenTmp == LESS_THAN_EQ){genererMiInst(LEQ);}
				else if(tokenTmp == GREATER_THAN_EQ){genererMiInst(GEQ);}
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
		while( token == PLUS_SIGN || token == HYPHEN_MINUS || token == AMPERSAND ){

			resultSimpleExp=_simple_expression_aux(); 
			_read_token();
			if(!resultSimpleExp) break;
		}
		if(resultSimpleExp){
			result=true;
		}if(token == PVIRG || token == DIFF || token == EQ || token == LESS_THAN || token == GREATER_THAN || 
			token == LESS_THAN_EQ || token == GREATER_THAN_EQ || token == AND || token == OR || token == XOR || token == THEN){
			result=true;
			follow_token=true;
		}
		
	}
	if(debug) printf("out_simple_expression \n");
	return result;
}

/* 
relation:: simple_expression (=, /=, >, >=, <, <=) simple_expression
		   | simple_expression [not] "in" (range | subtype_mark)
*/
//_simple_expression_aux :: ("+"|"-"|"&")_term | $
//follows simple_expression_aux = follows simple_express ={";", "=", "/=" , >, >=, <, <=, not , "in"}

bool _simple_expression_aux(){
	if(debug) printf("in_simple_expression_aux\n");
	bool result=false;
	if(token == PLUS_SIGN || token == HYPHEN_MINUS || token == AMPERSAND){
		//sauvgarde operation
		typetoken tokenTmp=token;
		_read_token();
		if(_term()){
			result = true;
		}
		//generer
		if(tokenTmp==PLUS_SIGN){
			genererMiInst(ADD);
		}else if(tokenTmp == HYPHEN_MINUS){
			genererMiInst(SUB);
		}else{
			genererMiInst(ANDN);
		}

	}else if(token == PVIRG || token == DIFF || token == EQ || token == LESS_THAN || token == GREATER_THAN || 
			token == LESS_THAN_EQ || token == GREATER_THAN_EQ || token == AND || token == OR || token == XOR || token == IN){

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
	bool errorDetected = false;	
	if( token == V_NULL ){result = true ;}
	if(token == NUMERIC ){
		//generer
		genererInstInt(LDI,numericvalue.value);
		if( isInAssignementPart && assignementType!=INTEGER){
			//generer erreur
			errorDetected = true;
		}
		result = true ;
	}
	if(token == DECIMAL){
		//generer
		genererInstFloat(LDI,decimalvalue.value);
		if( isInAssignementPart && (assignementType!=FLOAT || assignementType!=INTEGER)){
			//generer erreur
			errorDetected = true;
		}	
		result = true ;
	}
	if(token == STRING_LITERAL){
		//generer
		genererInstString(LDI,stringvalue.value);
		if( isInAssignementPart && assignementType!=STRING){
			//generer erreur
			errorDetected = true;
		}
		result = true ;
	}
	if(token == IDF){
		//generer
		if(isInTabSymb(idfvalue.value)){
			genererInstInt(LDA,adresseInTabSymb(idfvalue.value));
			genererMiInst(LDV);
			tabSymb symbtmp = getSymbByName(idfvalue.value);
			if( isInAssignementPart && assignementType!=symbtmp.type){
				//generer erreur
				errorDetected = true;
			}
			result = true ;
		}

	}
	if(errorDetected){
		strcpy(errorTmp->msgError,"incompatible type exception");
		errorTmp->line = linenumber.line;
		errorTmp->next=NULL;
		addOnTabError(errorTmp);	
	}
	if (debug) printf("out_term \n");
	return result;
}

/*
suquence_of_statement:: statement suquence_of_statement | epsilon
 */
 
//follows of sequence_of_ stat : END - ELSE - ELSIF - ENDIF - ENDLOOP - WHENOTHERS - EXIT
bool _suquence_of_statement() {
	if (debug) printf("in_suquence_of_statement \n");
	bool result = false;

	bool resulttmp = true;
	while(token != END && token != ELSE && token != ELSIF && token != ENDIF && token != ENDLOOP && token != WHENOTHERS && token != EXIT){
		if(_null_statement() || _assignement_statement() || _exit_statement() || _boucle_statements()||_case_statement()|| _get_statement() || _put_statement() || _if_statement()){
			resulttmp = true;
		}
		_read_token();
		if(!resulttmp) break;
	}
	if(resulttmp && token != END && token != ELSE && token != ELSIF && token != ENDIF && token != ENDLOOP && token != WHENOTHERS && token != EXIT){result=true;}
	if(token == END || token == ELSE || token == ELSIF || token == ENDIF || token == ENDLOOP || token == WHENOTHERS || token == EXIT){
		follow_token=true;
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
		isInAssignementPart = true;
		if(isInTabSymb(idfvalue.value)){
			tabSymb symb = getSymbByName(idfvalue.value);
			assignementType = symb.type;
			if(symb.isCste){
				strcpy(errorTmp->msgError,"constant attribute can not be variable");
				errorTmp->line = linenumber.line;
				errorTmp->next=NULL;
				addOnTabError(errorTmp);
			}
		//generer
		genererInstInt(LDA,adresseInTabSymb(idfvalue.value));
		}
		_read_token();
		if(token == ASSIGNMENT) {
			_read_token();
			if(_simple_expression()) {
				if(token == PVIRG) {
					//generer
					genererMiInst(STO);
					result = true;
				}
			}
			
		}
		isInAssignementPart = false;
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

// _get_statement :: "Get" "(" IDF ")" ;

bool _get_statement(){
	if(debug){printf("in_get_statement\n");}
	bool result= false;
	if(token == GET){
		_read_token();
		if(token == LEFT_PARENTHESIS){
			_read_token();
			if(token == IDF){
				//generer
				if(isInTabSymb(idfvalue.value)){
					genererInstInt(LDA,adresseInTabSymb(idfvalue.value));
					genererMiInst(INN);
					result = true ;
				}
				_read_token();
				
				if(token == RIGHT_PARENTHESIS){
					_read_token();
					if(token == PVIRG){
						result=true;
					}
				}
			}
		}
	}
	if(debug){printf("out_get_statement\n");}
	return result;
}

//_put_statement :: "Put" "(" _term ")" ";"

bool _put_statement(){
	if(debug){printf("in_put_statement\n");}
	bool result = false;
	if(token == PUT){
		_read_token();
		if(token == LEFT_PARENTHESIS){
			_read_token();
			if(_term()){
				_read_token();
				if(token == RIGHT_PARENTHESIS){
					//generer
					genererMiInst(PRN);
					_read_token();
					if(token == PVIRG){
						result=true;
					}
				}
			}
		}
	}
	return result;
	if(debug){printf("out_put_statement\n");}
}


void set_idf_attribute(char * idf){
	if(debug) printf("in_set_idf_att\n");
	idfvalue.value = (char*)malloc(sizeof(char)*strlen(idf)+1);
	strcpy(idfvalue.value ,idf);
	if(isInBodyPart && !isInTabSymb(idfvalue.value)){
			char* msg = (char*)malloc(200*sizeof(char));
			sprintf(msg,"variable %s is undeclared",idfvalue.value);
			strcpy(errorTmp->msgError,msg);
			errorTmp->line = linenumber.line;
			errorTmp->next=NULL;
			addOnTabError(errorTmp);
	}
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



 
