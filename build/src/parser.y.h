/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_HOME_VAGRANT_GITHUB_COMPILER_BUILD_SRC_PARSER_Y_H_INCLUDED
# define YY_YY_HOME_VAGRANT_GITHUB_COMPILER_BUILD_SRC_PARSER_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 13 "/home/vagrant/GitHub/Compiler/src//parser.y" /* yacc.c:1909  */
#include "symbol_table.h"

#line 47 "/home/vagrant/GitHub/Compiler/build/src//parser.y.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    DO = 258,
    WHILE = 259,
    IF = 260,
    ELSE = 261,
    INT = 262,
    VOID = 263,
    RETURN = 264,
    COMMA = 265,
    SEMICOLON = 266,
    BRACE_OPEN = 267,
    BRACE_CLOSE = 268,
    BRACKET_OPEN = 269,
    BRACKET_CLOSE = 270,
    PARA_OPEN = 271,
    PARA_CLOSE = 272,
    ID = 273,
    NUM = 274,
    ASSIGN = 275,
    LOGICAL_OR = 276,
    LOGICAL_AND = 277,
    EQ = 278,
    NE = 279,
    LS = 280,
    LSEQ = 281,
    GTEQ = 282,
    GT = 283,
    SHIFT_LEFT = 284,
    SHIFT_RIGHT = 285,
    PLUS = 286,
    MINUS = 287,
    MUL = 288,
    DIV = 289,
    MOD = 290,
    LOGICAL_NOT = 291,
    UNARY_MINUS = 292,
    UNARY_PLUS = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "/home/vagrant/GitHub/Compiler/src//parser.y" /* yacc.c:1909  */

  int i;
  char *id;
  func_return_type rtype;

#line 104 "/home/vagrant/GitHub/Compiler/build/src//parser.y.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_HOME_VAGRANT_GITHUB_COMPILER_BUILD_SRC_PARSER_Y_H_INCLUDED  */
