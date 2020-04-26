#ifndef STMT_LIST_H
#define STMT_LIST_H

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

extern void print_intermediate_code (char* file_string);

#endif