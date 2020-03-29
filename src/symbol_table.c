#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>


void add_var (char* id)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_VAR;
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length = 0;
    new_symbol->param_count = 0;
    new_symbol->return_type = FUNC_RETURN_TYPE_NONE;
    
    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;

}

void add_fun (char* id, func_return_type rtype, unsigned int param_count)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_FUNC;
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length = 0;
    new_symbol->param_count = param_count;
    new_symbol->return_type = rtype;

    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;
}

void add_arr (char* id, unsigned int length)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_ARRAY;
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length =length;
    new_symbol->param_count = 0;
    new_symbol->return_type = FUNC_RETURN_TYPE_NONE;

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
        else
            currentElement = currentElement->next;

    }
    while (currentElement->next != 0);

    return 0;

}

void print_symbol_table ()
{

    symbol_table_element *currentElement = &first_element;
    
    FILE *fp;
    fp = fopen("symbol_table.txt", "w+");

    fprintf(fp, "Start symbol table:\n");

    do 
    {
        
        switch (currentElement->type)
        {
            case SYMBOL_TYPE_VAR:
            fprintf(fp, "Type: Variable, ID: %s\n", currentElement->id);
            break;

            case SYMBOL_TYPE_FUNC:
            fprintf(fp, "Type: Function, ID: %s, ", currentElement->id);
            
            if (currentElement->return_type == FUNC_RETURN_TYPE_INT)
                fprintf(fp, "Return type: int, ");

            if (currentElement->return_type == FUNC_RETURN_TYPE_VOID)
                fprintf(fp, "Return type: void, ");

            if (currentElement->return_type == FUNC_RETURN_TYPE_NONE)
                fprintf(fp, "Return type: ERROR, ");

            fprintf(fp, "Parameter count: %d\n", currentElement->param_count);
            break;

            case SYMBOL_TYPE_ARRAY:
            fprintf(fp, "Type: Array, ID: %s, Array length: %d\n", currentElement->id, currentElement->length);
            break;

            default:
                fprintf (fp, "Error! Unrecognized type!\n");
            break;

        };

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }
    while (currentElement->next != 0);

    fclose(fp);
    return;

}