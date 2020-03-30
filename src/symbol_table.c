#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>


void add_var (char* id)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_VAR;
    new_symbol->id = malloc(sizeof(id));
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length = 0;
    new_symbol->param_count = 0;
    new_symbol->return_type = FUNC_RETURN_TYPE_NONE;
    
    // if the element is already in the namsepace: do not add to symbol table 
    if (element_in_namespace(new_symbol))
    {
        fprintf(stderr, "Duplicate definition of variable!\n");
        free(new_symbol);
        return;
    }

    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;

    print_symbol_table();
}

void add_fun (char* id, func_return_type rtype, unsigned int param_count)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_FUNC;
    new_symbol->id = malloc(sizeof(id));
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length = 0;
    new_symbol->param_count = param_count;
    new_symbol->return_type = rtype;

    // if the element is already in the namsepace: do not add to symbol table 
    if (element_in_namespace(new_symbol))
    {
        fprintf(stderr, "Duplicate definition of function!\n");
        free(new_symbol);
        return;
    }

    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;

    print_symbol_table();
}

void add_arr (char* id, unsigned int length)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_ARRAY;
    new_symbol->id = malloc(sizeof(id));
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length =length;
    new_symbol->param_count = 0;
    new_symbol->return_type = FUNC_RETURN_TYPE_NONE;

    // if the element is already in the namsepace: do not add to symbol table 
    if (element_in_namespace(new_symbol))
    {
        fprintf(stderr, "Duplicate definition of variable!\n");
        free(new_symbol);
        return;
    }

    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;

    print_symbol_table();
}

symbol_table_element *get_last_table_element()
{

    symbol_table_element *currentElement = &first_element;
    
    do 
    {

        if (currentElement->next != 0)
            currentElement = currentElement->next;

        if (currentElement->next == 0)
            return currentElement;

    }
    while (currentElement->next != 0);

    return 0;

}

bool element_in_namespace(symbol_table_element *element)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {
        
        if (currentElement->id != 0)
        {
            if (!strcmp(currentElement->id, element->id) && 
            currentElement->param_count == element->param_count && 
            currentElement->return_type == element->return_type)
            {

                if (((element->type == SYMBOL_TYPE_VAR || element->type == SYMBOL_TYPE_ARRAY) &&
                (currentElement->type == SYMBOL_TYPE_VAR || currentElement->type == SYMBOL_TYPE_ARRAY)) ||
                (element->type == SYMBOL_TYPE_FUNC && currentElement->type == SYMBOL_TYPE_FUNC))
                    return true;

            }
        }

        if (currentElement->next == 0)
            return false;

        if (currentElement->next != 0)
            currentElement = currentElement->next; 

    }
    
    return false;

}

void print_symbol_table ()
{

    symbol_table_element *currentElement = &first_element;
    
    FILE *fp;
    fp = fopen("symbol_table.txt", "w+");

    fprintf(fp, "Start symbol table:\n");

    while (true)
    {
        
        if (currentElement->id != 0)
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
        }

        if (currentElement->next == 0)
        {
            fclose(fp);
            return;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

    fclose(fp);
    return;

}


// funktion to free allocated memory of the symbol table
void cleanMem()
{
    symbol_table_element* current_element = &first_element;

    while(true)
    {
        if(current_element->next != NULL)
        {
            symbol_table_element* tmp = current_element->next;

            // check for first element
            if (current_element->id != NULL)
            {
                free(current_element);
            }
            current_element = tmp;
        }
        else
        {
            free(current_element);
            return;
        }
    }

    return;
}