#include "symbol_table.h"

void add_var (symbol_type type, char* id)
{
    symbol_table_element new_symbol;
    new_symbol.type = type;
    new_symbol.id = strdup(id);

    
}

symbol_table_element *get_last_table_element(symbol_table_element *first_element)
{

    symbol_table_element *currentElement = first_element;
    
    do 
    {
        if (currentElement->next == 0)
            return currentElement;
        
        currentElement = currentElement->next;

    }
    while (currentElement->next != 0);

    return 0;

}