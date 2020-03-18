#include "symbol_table.h"

void add_var (char* id)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->id = strdup(id);
    
    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;
}

symbol_table_element *get_last_table_element()
{

    symbol_table_element *currentElement = &first_element;
    
    do 
    {
        if (currentElement->next == 0)
            return currentElement;
        
        currentElement = currentElement->next;

    }
    while (currentElement->next != 0);

    return 0;

}