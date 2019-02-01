# Spark-Compiler

<h4>Grammar</h4>
<p>
_program -> _use_clause _program_body <br>
_use_clause -> ("use" | "with")  id _use_clause | $; <br>
_program_body -> _program_specification "is" _basic_declaration "begin" _sequence_of_statements "end" id; <br>
_program_specification -> "procedure" id | "function" id "return" id; <br>
_basic_declaration -> id : _type_declaration ; _basic_declaration | $ <br>
_sequence_of_statements -> _statement _sequence_of_statements | $ <br>
_statement -> _assignement_statement | _if_statement | _boucle_statement | _switch_statement | _exit_statement | _null_statement <br>
_assignement_statement -> IDF := _simple_expression ; <br>
_if_statement -> "if" _condition "then" _sequence_of_statements _elsif_statements _if_aux "end if" <br>
_elsif_statements -> "elsif" _condition "then" _sequence_of_statements _elsif_statements | $ <br>
_if_aux  ->  "else" _sequence_of_statements | $< br>
_condition -> _relation _relation_aux <br>
_relation-> simple_expression (=, /=, >, >=, <, <=) simple_expression 
		   | simple_expression [not] "in" (range | subtype_mark) <br>
_relation_aux-> "and" _relation | "and" "then" _relation | "or" _relation | "or" "then" _relation | "xor" _relation | $ <br>
_simple_expression-> _term _simple_expression_aux <br>
_simple_expression_aux ---> ("+" | "-" | "&") _term | epsilon <br>
_term -> V_NULL | NUMERIC | DECIMAL | STRING_LITERAL | IDF <br>
_boucle_statements -> _for_statements | _while_statements | _loop_statements <br>
_for_statements -> "for" IDF "in" [REVERSE] NUMERIC "to" NUMERIC _loop_statements <br>
_loop_statements -> "loop" _sequence_of_statements "end loop" ";" <br>
_while_statements -> "while" _condition loop_statements <br>
_null_statement-> "null" ";" <br>
_exit_statement-> "exit" _exit_aux ";" <br>
_exit_aux -> _when_statement | $ <br>
_when_statement -> "when" _condition ";" <br>

</p>
<h4>Commands for executing the project</h4>
<p> 
  flex -l Scanner.l <br>
  gcc -c lex.yy.c <br>
  repete gcc -c for all c files <br>
  gcc -o test lex.yy.o (all .o files) -lfl<br>
  ./test <br>
</p>
