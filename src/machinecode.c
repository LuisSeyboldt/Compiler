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

void print_functions_mc (FILE *fp)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        if (currentElement->id != NULL && currentElement->type == SYMBOL_TYPE_FUNC) 
        {

            print_function_header_mc (fp, currentElement);

            // print local variables
            print_local_variables_mc(fp, currentElement->function_scope);

            // print statements
            //print_statements_mc(fp, currentElement->function_scope, &first_stmt);

            fprintf(fp, "\n");

        }

        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

}

void print_function_header_mc (FILE *fp, symbol_table_element *currentElement)
{

    fprintf(fp, "%s:\n", currentElement->id);

    fprintf(fp, "### function prolog ###\naddi $sp, $sp, -8\nsw $31, 4($sp)\nsw $fp, 0($sp)\nmove $fp, $sp\n\n");

}

void print_local_variables_mc (FILE *fp, int function_scope)
{

    int spaceNeeded = 0;

    symbol_table_element *currentElement = &first_element;

    // loop through symbol table and calculate how much space is needed for local varibales
    while (true)
    {

        if (currentElement->scope == function_scope && !currentElement->isParam)
        {

            if (currentElement->type == SYMBOL_TYPE_VAR)
                spaceNeeded += 4;

            if (currentElement->type == SYMBOL_TYPE_ARRAY)
                spaceNeeded += currentElement->length*4;

        }


        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

    // Add the space needed for temporary variables
    spaceNeeded += get_function_from_scope(function_scope)->numberOfTmpVars*4;

    // move stack pointer by needed space
    fprintf(fp, "### create local variables ###\n");
    fprintf(fp, "addi, $sp, $sp, -%d\n", spaceNeeded);

    int spaceOccupied = spaceNeeded-4;

    // loop again through symbol table
    // now actually declare the local variables
    currentElement = &first_element;

    while (true)
    {

        if (currentElement->scope == function_scope && !currentElement->isParam)
        {

            // create variables
            if (currentElement->type == SYMBOL_TYPE_VAR)
            {
                fprintf(fp, "li $5, 0\nsw $5, %d($sp)\n", spaceOccupied);
                spaceOccupied -= 4;
            }

            // create arrays
            if (currentElement->type == SYMBOL_TYPE_ARRAY)
            {
                // allocate 4 bytes of memory on the stack for every array element
                for (int i = 0; i < currentElement->length; i++)
                {
                    fprintf(fp, "li $5, 0\nsw $5, %d($sp)\n", spaceOccupied);
                    spaceOccupied -= 4;
                }
            }
        }


        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

    // allocate space on the stack for all temporary variables of the function
    for (int i = 0; i < get_function_from_scope(function_scope)->numberOfTmpVars; i++)
    {
        fprintf(fp, "li $5, 0\nsw $5, %d($sp)\n", spaceOccupied);
        spaceOccupied -= 4;
    }

    // at this point spaceOccupied should be euqal to -4

}

void print_machine_code (char* file_string)
{

    reverse_stmt_list(&first_stmt);

    FILE *fp = 0;
    fp = fopen(file_string, "w+");

    fprintf(fp, ".data\n");

    print_global_elements_as_mc(fp);
    fprintf(fp, "\n.text\n");
    print_functions_mc(fp);

    fclose(fp);

}