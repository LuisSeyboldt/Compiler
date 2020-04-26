#include "stmt_list.h"
#include "symbol_table.h"

stmt_list_element* stmt_from_expr(value* expr)
{
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_EXPR;
    new_element->stmt.expr = expr;

    return new_element;
}

stmt_list_element* stmt_from_cond(value* cond_expr, stmt_list_element* true_list, stmt_list_element* false_list)
{
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_COND;
    new_element->stmt.stmt_cond->cond_expr = cond_expr;
    new_element->stmt.stmt_cond->true_list = true_list;
    if(false_list != NULL)
    {
        new_element->stmt.stmt_cond->false_list = false_list;
    }

    return new_element;
}

void print_function_header (FILE *fp, symbol_table_element *currentElement)
{

    if (currentElement->return_type == FUNC_RETURN_TYPE_INT)
        fprintf(fp, "int ");
    if (currentElement->return_type == FUNC_RETURN_TYPE_VOID)
        fprintf(fp, "void ");

    fprintf (fp, "%s (", currentElement->id);
    
    // print param list if there are any
    if (currentElement->param_count > 0)
    {
        bool first_param = true;

        // get first parameter
        symbol_table_element *firstDefinedParameter = get_frist_parameter_of_func(currentElement->function_scope);
        symbol_table_element *currentDefiniedElement = firstDefinedParameter;
        while (true)
        {

            if (currentDefiniedElement->next == NULL || currentDefiniedElement->isParam == false)
                break;
            
            if (!first_param)
                fprintf(fp, ", ");

            fprintf(fp, "int %s", currentDefiniedElement->id);

            if (currentDefiniedElement->type == SYMBOL_TYPE_ARRAY)
            {
                fprintf(fp, "[%d]", currentDefiniedElement->length);
            }

            if (currentDefiniedElement->next != 0)
            {
                first_param = false;
                currentDefiniedElement = currentDefiniedElement->next;
            }

        }
        
    }

    fprintf (fp, ")");

}

void print_global_elements(FILE *fp)
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
                print_function_header (fp, currentElement);
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

void print_functions (FILE *fp)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        if (currentElement->id != NULL && currentElement->type == SYMBOL_TYPE_FUNC) 
        {

            print_function_header (fp, currentElement);
            fprintf(fp, "\n{");

            // print statements

            fprintf(fp, "\n}\n");

        }

        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

}

void print_intermediate_code (char* file_string)
{

    FILE *fp = 0;
    fp = fopen(file_string, "w+");

    print_global_elements(fp);
    fprintf(fp, "\n");
    print_functions(fp);

    fclose(fp);

}