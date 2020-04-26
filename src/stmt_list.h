#ifndef STMT_LIST_H
#define STMT_LIST_H

#include <stdlib.h>
#include <string.h>

#include "type_checks.h"

typedef union stmt_union
{
    value* expr;
    struct stmt_cond_struct* stmt_cond;
    struct stmt_loop_struct* stmt_loop;
    struct stmt_return_struct* stmt_return;
} stmt;


typedef enum stmt_enum 
{
    STMT_TYPE_EMPTY,
    STMT_TYPE_COND,
    STMT_TYPE_LOOP,
    STMT_TYPE_EXPR,
    STMT_TYPE_RETURN
} stmt_type;

typedef struct stmt_list_struct
{
    struct stmt_list_struct* next;
    stmt_type type;
    stmt stmt;
    int scope;
} stmt_list_element;

typedef struct stmt_cond_struct
{
    value* cond_expr;
    stmt_list_element* true_list;
    stmt_list_element* false_list;
} stmt_cond;

typedef struct stmt_loop_struct
{
    bool do_while;
    value*  cond_expr;
    stmt_list_element* loop_list;
} stmt_loop;

typedef struct stmt_return_struct
{
    value* return_expr;
} stmt_return;

typedef struct stmt_expr_struct
{
    char* dest;
    char* op;
    char* operand1;
    char* operand2;
} stmt_expr;


typedef struct stmt_list_struct
{
    stmt_list_element* next;
    stmt_type type;
    stmt stmt;
    int scope;
} stmt_list_element;

typedef union stmt_union
{
    value* expr;
    stmt_expr* stmt_expr;
    stmt_cond* stmt_cond;
    stmt_loop* stmt_loop;
    stmt_return* stmt_return;
} stmt;


extern stmt_list_element* stmt_from_expr(value* expr);
extern stmt_list_element* stmt_from_cond(value* cond_expr, stmt_list_element* true_list, stmt_list_element* false_list);
extern stmt_list_element* stmt_from_loop(value* cond_expr, stmt_list_element* loop_list, bool doWhile);
extern stmt_list_element* stmt_from_return(value* expr);
extern void set_expr_details(char* op, value* currentExpr, value* nextExpr);

extern void print_intermediate_code (char* file_string);

#endif