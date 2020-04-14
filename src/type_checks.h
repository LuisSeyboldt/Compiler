#ifndef TYPE_CHECKS_H
#define TYPE_CHECKS_H

#include "symbol_table.h"

#include <stdbool.h>

typedef union value_union 
{
    int rval;
    symbol_table_element *element;
} simple_value;

typedef struct value_struct
{
    simple_value value;
    bool isSymbol;
} value;

#endif