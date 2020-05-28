#include "machinecode.h"
#include "stmt_list.h"

#include <stdio.h>

void print_global_elements_as_mc (FILE *fp)
{
    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        if (currentElement->scope == 0 && currentElement->id != NULL) 
        {

            if (currentElement->type == SYMBOL_TYPE_VAR)
            {
                fprintf(fp, "int %s;\n", currentElement->id);
            }

            if (currentElement->type == SYMBOL_TYPE_ARRAY)
            {
                fprintf(fp, "int %s[%d];\n", currentElement->id, currentElement->length);
            }

            if (currentElement->type == SYMBOL_TYPE_FUNC)
            {
                print_function_header_mc (fp, currentElement);
                fprintf(fp, ";\n");
            }
        }

        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }
}

void print_functions_mc (FILE *fp)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        if (currentElement->id != NULL && currentElement->type == SYMBOL_TYPE_FUNC) 
        {

            print_function_header_mc (fp, currentElement);
            fprintf(fp, "\n{\n");

            // print local variables
            print_local_variables_mc(fp, currentElement->function_scope);

            // print statements
            print_statements_mc(fp, currentElement->function_scope, &first_stmt);

            fprintf(fp, "}\n");

        }

        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

}

void print_machine_code (char* file_string)
{

    reverse_stmt_list(&first_stmt);

    FILE *fp = 0;
    fp = fopen(file_string, "w+");

    print_global_elements_as_mc(fp);
    fprintf(fp, "\n");
    print_functions_mc(fp);

    fclose(fp);

}