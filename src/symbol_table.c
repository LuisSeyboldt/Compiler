#include "symbol_table.h"

void add_var (symbol_type type, char* id)
{
    symbol_table new_symbol;
    new_symbol.type = type;
    new_symbol.id = strdup(id);

    
}