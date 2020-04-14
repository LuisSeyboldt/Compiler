#ifndef TYPE_CHECKS_H
#define TYPE_CHECKS_H

#include "symbol_table.h"

#include <stdbool.h>

typedef enum value_type_enum 
{
    VALUE_TYPE_VALUE, VALUE_TYPE_SYMBOL, VALUE_TYPE_FUNCTION_CALL
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

extern value valueFromId(char* id);
extern value valueFromNum(int num);


#endif