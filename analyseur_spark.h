#include <stdio.h>
#include <stdbool.h>

typedef enum {
 CASE ,
 WHEN ,
 WHENOTHERS ,
 IMPLIQUE ,
 ENDCASE ,
 WITH ,
 PACKAGE ,
 CSTE ,
 USE ,
 PROCEDURE ,
 IDF ,
 IS ,
 END ,
 LEFT_PARENTHESIS ,
 STRING ,
 INTEGER,
 FLOAT,
 RIGHT_PARENTHESIS ,
 PVIRG ,
 V_BEGIN ,
 TYPE ,
 COMMA ,
 DBPOINT ,
 EQUALS ,
 ELSE,
 NUMERIC ,
 DECIMAL ,
 STRING_LITERAL ,
 IF,
 ELSIF,
 ENDIF,
 THEN,
 XOR,
 AND,
 OR,
 DIFF,
 TWOPOINTS,
 EQ,
 EXIT,
 LESS_THAN,
 GREATER_THAN,
 LESS_THAN_EQ,
 GREATER_THAN_EQ,
 PLUS_SIGN,
 HYPHEN_MINUS,
 AMPERSAND,
 V_NULL,
 ASSIGNMENT,
 FOR,
 WHILE,
 IN,
 REVERSE,
 ENDLOOP,
 TO,
 LOOP,
 PUT,
 GET
} typetoken;

extern int yylex();

void _read_token() ;
bool _if_statement();
bool _ifaux_statement();
bool _elsif_statement();
bool _condition();
bool _condition_aux();
bool _relation();
bool _relation_aux();
bool _operation_aux();
bool _op_aux();
bool _simple_expression();
bool _simple_expression_pre();
bool _simple_expression_aux();
bool _term();
bool _factor();
bool _primary();
bool _suquence_of_statement();
bool _null_statement();
bool _assignement_statement();
bool _exit_statement();
bool _case_statement();
bool _when_statement();
bool _when_others_statement();
bool _put_statement();
bool _get_statement();

bool _boucle_statements();
bool _for_statements();
bool _loop_statements();
bool _while_statements();
bool _program();
bool _use_clause();
bool _body_program();
bool _program_specification();
bool _basic_declaration();
bool _type_declaration();


void set_idf_attribute(char * idf);
void set_line_attribute(int line);
void set_numeric_attribute(int numeric);
void set_decimal_attribute(float decimal);
void set_string_attribute(char* string);

