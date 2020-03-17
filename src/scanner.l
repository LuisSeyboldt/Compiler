/* 
 * scanner.l - Scanner utility for the DHBW compiler
 */

%{
	// Project-specific includes
	#include "parser.y.h"
%}

/* 
 * Default setting for flex which indicates that yytext, which is one of the main connections
 * between the scanner and parser, is handled as pointer of char instead of an array of char. 
*/
%pointer	

/* save the line number of terminals */
%option yylineno

/* finish after scanning the provided file */
%option noyywrap		

%%

int						{ return INT; }
; 						{ return SEMICOLON; }
[do]					{ return DO; }
while					{ return WHILE; }
if						{ return IF; }
else 					{ return ELSE; }
void 					{ return VOID; }
return 					{ return RETURN; }
,						{ return COMMA; }
\(						{ return PARA_OPEN; }
\)						{ return PARA_CLOSE; }
\{						{ return BRACE_OPEN; }
\}						{ return BRACE_CLOSE; }
\[						{ return BRACKET_OPEN; }
\]						{ return BRACKET_CLOSE; }
[a-zA-Z][a-zA-Z0-9]* 	{ yylval.id = strdup(yytext); return ID; }
