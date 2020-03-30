/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{
	// System includes
	#include <stdbool.h>
	
	// Project-specific includes
	#include "diag.h"
%}

%code requires {#include "symbol_table.h"}

%union {
  int i;
  char *id;
  func_return_type rtype;
}
// Verbose messages on parser error
%define parse.error verbose

%locations
%start program

// Compile in parser tracing
%debug

// Enable parser tracing output
%initial-action
{
	yydebug = true;
};

/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */
%expect 1

%token DO WHILE
%token IF ELSE
%token INT VOID
%token RETURN

%token COMMA SEMICOLON

%token BRACE_OPEN BRACE_CLOSE
%token BRACKET_OPEN BRACKET_CLOSE
%token PARA_OPEN PARA_CLOSE

%token ID
%token NUM

%token ASSIGN 
%token LOGICAL_OR
%token LOGICAL_AND
%token EQ NE
%token LS LSEQ GTEQ GT 
%token SHIFT_LEFT SHIFT_RIGHT
%token PLUS MINUS     
%token MUL DIV MOD
%token LOGICAL_NOT UNARY_MINUS UNARY_PLUS

%right ASSIGN
%left LOGICAL_OR
%left LOGICAL_AND
%left EQ NE
%left LS LSEQ GTEQ GT
%left SHIFT_LEFT SHIFT_RIGHT
%left PLUS MINUS
%left MUL DIV MOD
%right UNARY_MINUS UNARY_PLUS LOGICAL_NOT
%left PARA_OPEN PARA_CLOSE BRACKET_OPEN BRACKET_CLOSE 

%type<rtype> type INT VOID
%type<id>    identifier_declaration ID
%type<i>     NUM function_parameter_list

%%

program
     : program_element_list 
     ;

program_element_list
     : program_element_list program_element 
     | program_element
     ;

program_element
     : variable_declaration SEMICOLON
     | function_declaration SEMICOLON
     | function_definition
     | SEMICOLON
     ;
									
type
     : INT     { $$ = FUNC_RETURN_TYPE_INT; }
     | VOID    { $$ = FUNC_RETURN_TYPE_VOID; }
     ;

variable_declaration
     : variable_declaration COMMA identifier_declaration
     | type identifier_declaration                       

identifier_declaration
     : ID BRACKET_OPEN NUM BRACKET_CLOSE { add_arr($1, $3); }
     | ID                                { add_var($1); }
     ;

function_definition
     : type ID PARA_OPEN PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE                           { add_fun($2, $1, 0); }
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE   { add_fun($2, $1, $4); }
     ;

function_declaration
     : type ID PARA_OPEN PARA_CLOSE                                                            { add_fun($2, $1, 0); }
     | type ID PARA_OPEN function_parameter_list PARA_CLOSE                                    { add_fun($2, $1, $4); }
     ;

function_parameter_list
     : function_parameter                                   { $$ = 1; }
     | function_parameter_list COMMA function_parameter     { $$ = $1 + 1; }
     ;
	
function_parameter
     : type identifier_declaration
     ;
									
stmt_list
     : /* empty: epsilon */
     | stmt_list stmt
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON
     | expression SEMICOLON
     | stmt_conditional
     | stmt_loop
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt
     ;
									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON
     ;
									
expression
     : expression ASSIGN expression
     | expression LOGICAL_OR expression
     | expression LOGICAL_AND expression
     | LOGICAL_NOT expression
     | expression EQ expression
     | expression NE expression
     | expression LS expression 
     | expression LSEQ expression 
     | expression GTEQ expression 
     | expression GT expression
     | expression PLUS expression
     | expression MINUS expression
     | expression SHIFT_LEFT expression
     | expression SHIFT_RIGHT expression
     | expression MUL expression
     | expression DIV expression
     | MINUS expression %prec UNARY_MINUS
     | PLUS expression %prec UNARY_PLUS
     | ID BRACKET_OPEN primary BRACKET_CLOSE
     | PARA_OPEN expression PARA_CLOSE 
     | function_call
     | primary
     ;

primary
     : NUM
     | ID
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE
      | ID PARA_OPEN function_call_parameters PARA_CLOSE
      ;

function_call_parameters
     : function_call_parameters COMMA expression
     | expression
     ;

%%

void yyerror (const char *msg)
{
	COMPILER_ERROR(INVALID_SYNTAX, 0, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
