#include "symbol_table.h"

void add_var (char* id)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = NUM;
    new_symbol->id = strdup(id);
    new_symbol->next = 0;
    new_symbol->length = 0;
    new_symbol->param_count = 0;
    new_symbol->return_type = NONE;
    
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