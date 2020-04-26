#ifndef STMT_LIST_H
#define STMT_LIST_H

#include <stdlib.h>
#include <string.h>

#include "type_checks.h"

typedef union stmt_union
{
    struct stmt_expr_struct* stmt_expr;
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
    STMT_TYPE_RETURN,
    STMT_TYPE_TMP_DEC
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
    stmt_list_element* cond_stmt_list;
    stmt_list_element* true_list;
    stmt_list_element* false_list;
    char* cond_id;
} stmt_cond;

typedef struct stmt_loop_struct
{
    bool do_while;
    stmt_list_element* cond_stmt_list;
    stmt_list_element* loop_list;
    char* cond_id;
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


extern void add_to_list(stmt_list_element* list, stmt_list_element* element);
extern stmt_list_element* stmt_from_expr(value* expr);
extern stmt_list_element* stmt_from_cond(value* cond_expr, stmt_list_element* true_list, stmt_list_element* false_list);
extern stmt_list_element* stmt_from_loop(value* cond_expr, stmt_list_element* loop_list, bool doWhile);
extern stmt_list_element* stmt_from_return(value* expr);
extern void set_expr_details(char* op, value* currentExpr, value* nextExpr);

extern void print_intermediate_code (char* file_string);

#endif