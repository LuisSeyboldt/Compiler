#include "stmt_list.h"
#include "symbol_table.h"

#include <string.h>

stmt_list_element* stmt_from_expr(value* expr)
{
    if(expr->next_expr == NULL)
    {
        stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
        new_element->next = NULL;
        new_element->scope = numberOfScopes;
        new_element->type = STMT_TYPE_EXPR;
        new_element->stmt.expr = expr;

        return new_element;
    }
    
    value* current_value = expr;
    int numberOfTemps = 0;
    while(true)
    {
        if(current_value->next_expr != NULL)
        {
            if(current_value->next_expr->next_expr != NULL)
            {
                current_value = current_value->next_expr;
            }
            else // no more expressions 
            {
                // create statement for temp variable
                stmt_list_element* tmp_stmt = malloc(sizeof(stmt_list_element));
                tmp_stmt->next = NULL;
                tmp_stmt->scope = numberOfScopes;
                tmp_stmt->type = STMT_TYPE_EXPR;
                strcpy(tmp_stmt->stmt.stmt_expr->dest, strcat("temp_", itoa(numberOfTemps)));
                strcpy(tmp_stmt->stmt.stmt_expr->op, current_value->stmt_operator);


                if(current_value->valueType == VALUE_TYPE_VALUE)
                {
                    strcpy(tmp_stmt->stmt.stmt_expr->operand1, itoa(current_value->value.rval));
                }
                else if(current_value->valueType == VALUE_TYPE_SYMBOL)
                {
                    strcpy(tmp_stmt->stmt.stmt_expr->operand1, itoa(current_value->value.element->id));
                }

                if(current_value->next_expr->valueType == VALUE_TYPE_VALUE)
                {
                    strcpy(tmp_stmt->stmt.stmt_expr->operand2, itoa(current_value->next_expr->value.rval));
                }
                else if(current_value->valueType == VALUE_TYPE_SYMBOL)
                {
                    strcpy(tmp_stmt->stmt.stmt_expr->operand2, itoa(current_value->next_expr->value.element->id));
                }

                
            }
        }
    }
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

stmt_list_element* stmt_from_loop(value* cond_expr, stmt_list_element* loop_list, bool doWhile)
{
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_LOOP;
    new_element->stmt.stmt_loop->cond_expr = cond_expr;
    new_element->stmt.stmt_loop->loop_list = loop_list;
    new_element->stmt.stmt_loop->do_while = doWhile;

    return new_element;
}

stmt_list_element* stmt_from_return(value* expr)
{
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_RETURN;
    if(expr != NULL)
    {
        new_element->stmt.stmt_return->return_expr = expr;
    }
    else
    {
        new_element->stmt.stmt_return->return_expr = NULL;
    }


    return new_element;
}

void set_expr_details (char* op, value* currentExpr, value* nextExpr)
{
    if(op != NULL)
    {
        currentExpr->stmt_operator = op;
    }

    currentExpr->next_expr = nextExpr;
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

void print_stmt_expr (FILE *fp, stmt_expr* stmt_expr)
{

    if (stmt_expr->operand2 != NULL && stmt_expr->op != NULL && strcmp(stmt_expr->op, "="))
        fprintf (fp, "%s = %s %s %s;\n", stmt_expr->dest, stmt_expr->operand1, stmt_expr->op, stmt_expr->operand2);

    if (stmt_expr->operand2 == NULL && stmt_expr->op != NULL && strcmp(stmt_expr->op, "="))
        fprintf (fp, "%s = %s %s;\n", stmt_expr->dest, stmt_expr->op, stmt_expr->operand1);

    if (stmt_expr->operand2 == NULL &&  stmt_expr->op != NULL && !strcmp(stmt_expr->op, "="))
        fprintf (fp, "%s = %s;\n", stmt_expr->dest, stmt_expr->operand1);

}

void print_while_loop (FILE *fp, stmt_loop* stmt_loop, int function_scope)
{

    print_statements(fp, function_scope, stmt_loop->cond_stmt_list);

    int top_label = label_counter++;
    int loop_body = label_counter++;
    int after_loop = label_counter++;

    fprintf(fp, "label%d:\n", top_label);

    fprintf(fp, "if (%s)\n{\ngoto label%d;\n}\ngoto label%d;\n", stmt_loop->cond_id, loop_body, after_loop);
    
    fprintf(fp, "label%d\n", loop_body);
    print_statements(fp, function_scope, stmt_loop->loop_list);
    fprintf(fp, "goto label%d;\n", top_label);
    
    fprintf(fp, "label%d\n", after_loop);

}

void print_do_while_loop (FILE *fp, stmt_loop* stmt_loop, int function_scope)
{

    print_statements(fp, function_scope, stmt_loop->cond_stmt_list);

    int loop_body = label_counter++;
    int cond_label = label_counter++;
    int after_loop = label_counter++;

    fprintf(fp, "label%d\n", loop_body);
    print_statements(fp, function_scope, stmt_loop->loop_list);
    fprintf(fp, "goto label%d;\n", cond_label);
    
    fprintf(fp, "label%d:\n", cond_label);

    fprintf(fp, "if (%s)\n{\ngoto label%d;\n}\ngoto label%d;\n", stmt_loop->cond_id, loop_body, after_loop);

    fprintf(fp, "label%d\n", after_loop);

}

void print_cond (FILE *fp, stmt_cond *stmt_cond, int function_scope)
{

    print_statements(fp, function_scope, stmt_cond->cond_stmt_list);

    int true_label = label_counter++;
    int false_label = label_counter++;
    int after_label = label_counter++;

    fprintf(fp, "if (%s)\n{\ngoto label%d;\n}\ngoto label%d;\n", stmt_cond->cond_id, true_label, false_label);

    fprintf(fp, "label%d\n", true_label);
    print_statements(fp, function_scope, stmt_cond->true_list);
    fprintf(fp, "goto label%d;\n", after_label);

    fprintf(fp, "label%d\n", false_label);
    print_statements(fp, function_scope, stmt_cond->false_list);
    fprintf(fp, "goto label%d;\n", after_label);

    fprintf(fp, "label%d\n", after_label);

}

void print_statements (FILE *fp, int function_scope, stmt_list_element *first_element)
{

    stmt_list_element *current_stmt = first_element;

    while (true)
    {

        if (current_stmt->scope == function_scope)
        {

            switch (current_stmt->type) {

                STMT_TYPE_EMPTY:
                break;

                STMT_TYPE_COND:
                print_cond (fp, current_stmt->stmt.stmt_cond, function_scope);
                break;

                STMT_TYPE_LOOP:
                if (current_stmt->stmt.stmt_loop->do_while)
                    print_do_while_loop(fp, current_stmt->stmt.stmt_loop, function_scope);
                else 
                    print_while_loop(fp, current_stmt->stmt.stmt_loop, function_scope);
                break;

                STMT_TYPE_EXPR:
                print_stmt_expr(fp, current_stmt->stmt.stmt_expr);
                break;

                STMT_TYPE_RETURN:
                fprintf (fp, "return %s;\n", current_stmt->stmt.stmt_return->return_id);
                break;

                STMT_TYPE_TMP_DEC:
                fprintf (fp, "int %s;\n", current_stmt->stmt.stmt_expr->dest);
                print_stmt_expr(fp, current_stmt->stmt.stmt_expr);
                break;

            };

        }

        if (current_stmt->next == 0)
        {
            break;
        }

        if (current_stmt->next != 0)
            current_stmt = current_stmt->next;

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
            print_statements(fp, currentElement->function_scope, &first_stmt);

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

void print_intermediate_code (char* file_string)
{

    FILE *fp = 0;
    fp = fopen(file_string, "w+");

    print_global_elements(fp);
    fprintf(fp, "\n");
    print_functions(fp);

    fclose(fp);

}
