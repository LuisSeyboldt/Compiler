#ifndef TYPE_CHECKS_H
#define TYPE_CHECKS_H

#include "symbol_table.h"

#include <stdbool.h>

typedef enum value_type_enum 
{
    VALUE_TYPE_VALUE, VALUE_TYPE_SYMBOL, VALUE_TYPE_FUNCTION_CALL, VALUE_TYPE_ARR_ELEMENT
} value_type;

typedef union value_union 
{
    int rval;
    symbol_table_element *element;
} simple_value;

typedef struct value_struct
{
    simple_value value;
    value_type valueType;

    // pointer to the next element
    // (used for parameter list evaluation)
    struct value_struct *next;

    // for statement list
    char* stmt_operator;
    bool isTemp;
    struct value_struct* next_expr;
} value;

void caseLval(value expr);
void err(char* msg);
extern void checkReturnType(func_return_type rType, char* id);
extern value valueFromId(char* id);
extern value valueFromNum(int num);
extern value valueFromArray(char *id);
extern value valueFromFunction(char *id);
extern void checkExpr(value expr1, value expr2);
extern void checkRVal(value expr1, value expr2);
extern void checkSingleExpr(value expr);
extern void checkIfNotVoid(func_return_type type);
extern value valueFromFunctionWithParameterList(char *id, value *first_param_list_element);
extern void checkFuncReturn(value returnExpr);
extern void checkVoidReturn();
extern void checkParams(char* id, value* params);
extern void checkZeroParams(char* id);
bool compareParameters (symbol_table_element *definiedParameter, value *callParameter);
extern value *allocFunctionParameter (value someValue);
void cleanFunctionParameterMemory(value *firstFunctionCallParameter);

#endif