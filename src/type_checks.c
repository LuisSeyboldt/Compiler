#include "type_checks.h"
#include "symbol_table.h"

value valueFromId(char* id) 
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id); 
    new_val.valueType = VALUE_TYPE_SYMBOL;

    return new_val;
}

value valueFromNum(int num)
{
    value new_val;
    new_val.value.rval = num;
    new_val.valueType = VALUE_TYPE_VALUE;

    return new_val;
}