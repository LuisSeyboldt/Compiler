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

#endif