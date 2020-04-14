#include "type_checks.h"
#include "symbol_table.h"

value valueFromId(char* id) 
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id); 
    new_val.isSymbol = true;

    return new_val;
}

value valueFromNum(int num)
{
    value new_val;
    new_val.value.rval = num;
    new_val.isSymbol = false;

    return new_val;
}