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

            // print global variable declaration in machine code
            if (currentElement->type == SYMBOL_TYPE_VAR)
            {
                fprintf(fp, "%s:\n  .word 0\n  .align4\n", currentElement->id);
            }

            // print global array declaration in machine code
            if (currentElement->type == SYMBOL_TYPE_ARRAY)
            {
                fprintf(fp, "%s:\n  .word 0 : %d\n  .align4\n", currentElement->id, currentElement->length);
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

/*void print_functions_mc (FILE *fp)
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

}*/

void print_machine_code (char* file_string)
{

    reverse_stmt_list(&first_stmt);

    FILE *fp = 0;
    fp = fopen(file_string, "w+");

    fprintf(fp, ".data\n");

    print_global_elements_as_mc(fp);
    fprintf(fp, "\n");
    //print_functions_mc(fp);

    fclose(fp);

}