# Spark-Compiler

<h4>Authors:</h4> 
<ul>
	<li>
		<a href="https://www.linkedin.com/in/anas-el-ghaoui/">EL GHAOUI ANASS</a>
	</li>
	<li>
		<a href="https://www.linkedin.com/in/hamza-el-haddaoui-687a3314b/">EL HADDAOUI HAMZA</a>
	</li>
	
</ul>
<br>

<h4>Grammar</h4>
<p>
	
```
_program -> _use_clause _program_body
_use_clause -> ("use" | "with")  id _use_clause | $;
_program_body -> _program_specification "is" _basic_declaration "begin" _sequence_of_statements "end" id;
_program_specification -> "procedure" id | "function" id "return" id; 
_basic_declaration -> id ":" ["constant"] _type_declaration [":=" _term] ";" _basic_declaration | $ 
_sequence_of_statements -> _statement _sequence_of_statements | $
_statement -> _assignement_statement | _if_statement | _boucle_statement | _switch_statement | _exit_statement | _null_statement
_assignement_statement -> IDF := _simple_expression ; 
_if_statement -> "if" _condition "then" _sequence_of_statements _elsif_statements _if_aux "end if" 
_elsif_statements -> "elsif" _condition "then" _sequence_of_statements _elsif_statements | $ 
_if_aux  ->  "else" _sequence_of_statements | $
_condition -> _relation _relation_aux 
_relation-> simple_expression (=, /=, >, >=, <, <=) simple_expression 
		   | simple_expression [not] "in" (range | subtype_mark) 
_relation_aux-> "and" _relation | "and" "then" _relation | "or" _relation | "or" "then" _relation | "xor" _relation | $ 
_simple_expression-> _term _simple_expression_aux 
_simple_expression_aux ---> ("+" | "-" | "&") _term | epsilon 
_term -> V_NULL | NUMERIC | DECIMAL | STRING_LITERAL | IDF 
_boucle_statements -> _for_statements | _while_statements | _loop_statements
_for_statements -> "for" IDF "in" [REVERSE] NUMERIC "to" NUMERIC _loop_statements
_loop_statements -> "loop" _sequence_of_statements "end loop" ";"
_while_statements -> "while" _condition loop_statements
_null_statement-> "null" ";"
_exit_statement-> "exit" _exit_aux ";"
_exit_aux -> _when_statement | $
_when_statement -> "when" _condition ";"

```

</p>
<h4>Commands for executing the project</h4>
<p> 
	
```
  flex -l Scanner.l
  gcc -c lex.yy.c
  repeat gcc -c for all c files
  gcc -o test lex.yy.o (all .o files) -lfl
  ./test
```
</p>
