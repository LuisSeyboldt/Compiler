#include "stmt_list.h"
#include "symbol_table.h"

#include <string.h>


stmt_list_element* stmt_from_expr(value* expr)
{
    //init return element
    stmt_list_element* expressions = NULL;  
    expressions = malloc(sizeof(stmt_list_element));
    expressions->stmt.stmt_expr = init_stmt_expr();
    expressions->next = NULL;
    expressions->scope = numberOfScopes;
    expressions->type = STMT_TYPE_EMPTY;

    char convert_arr[255];           // array for int to char conversions

    // All cases with assign operation on the left
    if(!strcmp(expr->stmt_operator, "="))
    {
        // check if there is a right side
        if(expr->next_expr != NULL)
        {
            value* current_expr = expr->next_expr;

            // case e.g: i = 0;  || only one element on the right side
            if( current_expr->next_expr == NULL)
            {
                expressions->type = STMT_TYPE_EXPR;

                // set stmt_expr details
                if(expr->valueType == VALUE_TYPE_SYMBOL)
                {
                    strcpy(expressions->stmt.stmt_expr->dest, expr->value.element->id);
                }
                else if(expr->valueType == VALUE_TYPE_ARR_ELEMENT)
                {
                    if(expr->index->valueType == VALUE_TYPE_VALUE)
                    {
                        sprintf(convert_arr, "%s[%d]", expr->value.element->id, expr->index->value.rval);
                        strcpy(expressions->stmt.stmt_expr->dest, convert_arr);
                    }
                    else if (expr->index->valueType == VALUE_TYPE_SYMBOL)
                    {
                        sprintf(convert_arr, "%s[%s]", expr->value.element->id, expr->index->value.element->id);
                        strcpy(expressions->stmt.stmt_expr->dest, convert_arr);
                    }
                }

                // set operator
                strcpy(expressions->stmt.stmt_expr->op, expr->stmt_operator);

                // set operand1 of stmt_expr according to type of right side (var or num)
                if(current_expr->valueType == VALUE_TYPE_SYMBOL)
                {
                    strcpy(expressions->stmt.stmt_expr->operand1, current_expr->value.element->id);
                }
                else if(current_expr->valueType == VALUE_TYPE_VALUE)
                {
                    sprintf(convert_arr, "%d", current_expr->value.rval);
                    strcpy(expressions->stmt.stmt_expr->operand1, convert_arr);
                }
                else if(current_expr->valueType == VALUE_TYPE_ARR_ELEMENT)
                {
                    if(current_expr->index->valueType == VALUE_TYPE_VALUE)
                    {
                        sprintf(convert_arr, "%s[%d]", current_expr->value.element->id, current_expr->index->value.rval);
                        strcpy(expressions->stmt.stmt_expr->operand1, convert_arr);
                    }
                    else if (current_expr->index->valueType == VALUE_TYPE_SYMBOL)
                    {
                        sprintf(convert_arr, "%s[%s]", current_expr->value.element->id, current_expr->index->value.element->id);
                        strcpy(expressions->stmt.stmt_expr->operand1, convert_arr);
                    }
                }
                else if(current_expr->valueType == VALUE_TYPE_FUNCTION_CALL)
                {
                    copy_function_call(&expressions->stmt.stmt_expr->operand1, current_expr);
                }
            }
            else // case: more elements on the right side
            {
                char* last_tmp_var = slice_expressions(current_expr, expressions);
                // new statement to set the initial variable to the last tmp var
                stmt_list_element* new_stmt;
                new_stmt = malloc(sizeof(stmt_list_element));
                new_stmt->stmt.stmt_expr = init_stmt_expr();
                new_stmt->next = NULL;
                new_stmt->scope = numberOfScopes;
                new_stmt->type = STMT_TYPE_EXPR;

                if(expr->valueType == VALUE_TYPE_SYMBOL)
                {
                    strcpy(new_stmt->stmt.stmt_expr->dest, expr->value.element->id);
                }
                else if (expr->valueType == VALUE_TYPE_ARR_ELEMENT)
                {
                    if(expr->index->valueType == VALUE_TYPE_VALUE)
                    {
                        sprintf(convert_arr, "%s[%d]", expr->value.element->id, expr->index->value.rval);
                        strcpy(new_stmt->stmt.stmt_expr->dest, convert_arr);
                    }
                    else if (expr->index->valueType == VALUE_TYPE_SYMBOL)
                    {
                        sprintf(convert_arr, "%s[%s]", expr->value.element->id, expr->index->value.element->id);
                        strcpy(new_stmt->stmt.stmt_expr->dest, convert_arr);
                    }
                }
                strcpy(new_stmt->stmt.stmt_expr->op, "=");
                strcpy(new_stmt->stmt.stmt_expr->operand1, last_tmp_var);
                strcpy(new_stmt->stmt.stmt_expr->operand2, "");

                string_statements_together(expressions, new_stmt);
            }

        }
    }
    else    // not a '=' as first operator
    {
        // check if there is more than one operand
        if(expr->next_expr != NULL)
        {
            value* current_expr = expr;

            // case e.g: i + 1;  || only two operands
            if( current_expr->next_expr == NULL)
            {
                expressions->type = STMT_TYPE_EXPR;

                // set stmt_expr details
                strcpy(expressions->stmt.stmt_expr->dest, expr->value.element->id);
                strcpy(expressions->stmt.stmt_expr->op, expr->stmt_operator);

                // set operand1 of stmt_expr according to type of right side (var or num)
                if(current_expr->valueType == VALUE_TYPE_SYMBOL)
                {
                    strcpy(expressions->stmt.stmt_expr->operand1, current_expr->value.element->id);
                }
                else if(current_expr->valueType == VALUE_TYPE_VALUE)
                {
                    sprintf(convert_arr, "%d", current_expr->value.rval);
                    strcpy(expressions->stmt.stmt_expr->operand1, convert_arr);
                }
                else if(current_expr->valueType == VALUE_TYPE_ARR_ELEMENT)
                {
                    if(current_expr->index->valueType == VALUE_TYPE_VALUE)
                    {
                        sprintf(convert_arr, "%s[%d]", current_expr->value.element->id, current_expr->index->value.rval);
                        strcpy(expressions->stmt.stmt_expr->operand1, convert_arr);
                    }
                    else if (current_expr->index->valueType == VALUE_TYPE_SYMBOL)
                    {
                        sprintf(convert_arr, "%s[%s]", current_expr->value.element->id, current_expr->index->value.element->id);
                        strcpy(expressions->stmt.stmt_expr->operand1, convert_arr);
                    }
                }
                else if(current_expr->valueType == VALUE_TYPE_FUNCTION_CALL)
                {
                    copy_function_call(&expressions->stmt.stmt_expr->operand1, current_expr);
                }
            }
            else // case: more elements on the right side
            {
                slice_expressions(current_expr, expressions);
            }

        }
        else
        {
            if(expr->valueType == VALUE_TYPE_FUNCTION_CALL)
            {
                strcpy(expressions->stmt.stmt_expr->op, "func");
                copy_function_call(&expressions->stmt.stmt_expr->dest, expr);
                expressions->type = STMT_TYPE_EXPR;
            }
        }
    }

    reverse_stmt_list(&expressions);
    return expressions;
}

void copy_function_call(char** destination, value* function_call)
{
    char convert_arr[255];
    char function_call_name[255];
    strcpy(function_call_name, "");
    strcat(function_call_name, function_call->value.element->id);
    strcat(function_call_name, "(");
    value* current_element = function_call;

    bool isFirst = true;
    while(true)
    {
        if(current_element->next == NULL)
        {
            strcat(function_call_name, ")");
            strcpy(*(destination), function_call_name);
            break;
        }
        else
        {
            current_element = current_element->next;
            if(current_element->valueType == VALUE_TYPE_SYMBOL)
            {
                if(!isFirst)
                {
                    strcat(function_call_name, ", ");
                }
                strcat(function_call_name, current_element->value.element->id);
                isFirst = false;
            }
            else if(current_element->valueType == VALUE_TYPE_VALUE)
            {
                if(!isFirst)
                {
                    strcat(function_call_name, ", ");
                }
                sprintf(convert_arr, "%d", current_element->value.rval);
                strcat(function_call_name, convert_arr);
                isFirst = false;
            }
            else if(current_element->valueType == VALUE_TYPE_ARR_ELEMENT)
            {
                if(!isFirst)
                {
                    strcat(function_call_name, ", ");
                }

                if(current_element->index->valueType == VALUE_TYPE_VALUE)
                {
                    sprintf(convert_arr, "%s[%d]", current_element->value.element->id, current_element->index->value.rval);
                }
                else if (current_element->index->valueType == VALUE_TYPE_SYMBOL)
                {
                    sprintf(convert_arr, "%s[%s]", current_element->value.element->id, current_element->index->value.element->id);
                }                    

                strcat(function_call_name, convert_arr);
                isFirst = false;

            }
            else if(current_element->valueType == VALUE_TYPE_FUNCTION_CALL)
            {
                if(!isFirst)
                {
                    strcat(function_call_name, ", ");
                }

                char tmp[255]; 
                copy_function_call(&tmp, current_element);
                strcat(function_call_name, tmp);
                isFirst = false;
            }
        }
    }
    
}

char* slice_expressions(value* rvals, stmt_list_element* stmts)
{
    static int numberOfTmps = 0;
    char convert_arr[255];
    char tmp_name[255];
    strcpy(tmp_name, "temp_");

    if(rvals->next_expr != NULL)
    {
        // new statement
        stmt_list_element* new_stmt;
        new_stmt = malloc(sizeof(stmt_list_element));
        new_stmt->stmt.stmt_expr = init_stmt_expr();
        new_stmt->next = NULL;
        new_stmt->scope = numberOfScopes;
        new_stmt->type = STMT_TYPE_TMP_DEC;

        // create new tmp statement
        // set destination to new tmp var
        sprintf(convert_arr, "%d", numberOfTmps);
        strcat(tmp_name, convert_arr);
        strcpy(new_stmt->stmt.stmt_expr->dest, tmp_name);

        // set operator
        strcpy(new_stmt->stmt.stmt_expr->op, rvals->stmt_operator);

        // set operand1 
        if(rvals->valueType == VALUE_TYPE_VALUE)
        {
            sprintf(convert_arr, "%d", rvals->value.rval);
            strcpy(new_stmt->stmt.stmt_expr->operand1, convert_arr);
        }
        else if (rvals->valueType == VALUE_TYPE_SYMBOL)
        {
            strcpy(new_stmt->stmt.stmt_expr->operand1, rvals->value.element->id);
        }
        else if(rvals->valueType == VALUE_TYPE_ARR_ELEMENT)
        {
            if(rvals->index->valueType == VALUE_TYPE_VALUE)
            {
                sprintf(convert_arr, "%s[%d]", rvals->value.element->id, rvals->index->value.rval);
                strcpy(new_stmt->stmt.stmt_expr->operand1, convert_arr);
            }
            else if (rvals->index->valueType == VALUE_TYPE_SYMBOL)
            {
                sprintf(convert_arr, "%s[%s]", rvals->value.element->id, rvals->index->value.element->id);
                strcpy(new_stmt->stmt.stmt_expr->operand1, convert_arr);
            }
        }
        else if(rvals->valueType == VALUE_TYPE_FUNCTION_CALL)
        {
            copy_function_call(&new_stmt->stmt.stmt_expr->operand1, rvals);
        }

        // set operand2
        if(rvals->next_expr->valueType == VALUE_TYPE_VALUE)
        {
            sprintf(convert_arr, "%d", rvals->next_expr->value.rval);
            strcpy(new_stmt->stmt.stmt_expr->operand2, convert_arr);
        }
        else if (rvals->next_expr->valueType == VALUE_TYPE_SYMBOL)
        {
            strcpy(new_stmt->stmt.stmt_expr->operand2, rvals->next_expr->value.element->id);
        }
        else if(rvals->next_expr->valueType == VALUE_TYPE_ARR_ELEMENT)
        {
            if(rvals->next_expr->index->valueType == VALUE_TYPE_VALUE)
            {
                sprintf(convert_arr, "%s[%d]", rvals->next_expr->value.element->id, rvals->next_expr->index->value.rval);
                strcpy(new_stmt->stmt.stmt_expr->operand2, convert_arr);
            }
            else if (rvals->next_expr->index->valueType == VALUE_TYPE_SYMBOL)
            {
                sprintf(convert_arr, "%s[%s]", rvals->next_expr->value.element->id, rvals->next_expr->index->value.element->id);
                strcpy(new_stmt->stmt.stmt_expr->operand2, convert_arr);
            }
        }
        else if(rvals->next_expr->valueType == VALUE_TYPE_FUNCTION_CALL)
        {
            copy_function_call(&new_stmt->stmt.stmt_expr->operand2, rvals->next_expr);
        }

        numberOfTmps++;

        // append new statement to stmts
        string_statements_together(stmts, new_stmt);


        // termination condition: eg. rvals: 1 + 2 || no third value
        if(rvals->next_expr->next_expr == 0)
        {
            return new_stmt->stmt.stmt_expr->dest;
        }
        else // recursive call
        {
            value* tmp_value = malloc(sizeof(value)); 
            tmp_value->isTemp = true;
            tmp_value->next = NULL;
            tmp_value->next_expr = NULL;
            tmp_value->stmt_operator = malloc(sizeof(char[255]));
            tmp_value->valueType = VALUE_TYPE_SYMBOL;
            tmp_value->value.element = malloc(sizeof(symbol_table_element));
            tmp_value->value.element->id = malloc(sizeof(char[255]));
            
            strcpy(tmp_value->value.element->id, tmp_name);
            strcpy(tmp_value->stmt_operator, rvals->next_expr->stmt_operator);

            tmp_value->next_expr = rvals->next_expr->next_expr;
            free(rvals->next_expr);
            free(rvals);

            return slice_expressions(tmp_value, stmts);
        }
    }
    else
    {
        err("Error: internal error");
        return NULL;
    }
}

stmt_expr* init_stmt_expr()
{
    stmt_expr* new_stmt_expr = malloc(sizeof(stmt_expr));
    new_stmt_expr->dest = malloc(sizeof(char[255]));
    new_stmt_expr->op = malloc(sizeof(char[255]));
    new_stmt_expr->operand1 = malloc(sizeof(char[255]));
    new_stmt_expr->operand2 = malloc(sizeof(char[255]));

    return new_stmt_expr;
}

stmt_return* init_stmt_return()
{
    stmt_return* new_stmt = malloc(sizeof(stmt_return));
    new_stmt->return_expr_list = malloc(sizeof(stmt_list_element));
    new_stmt->return_id = malloc(sizeof(char[255]));

    return new_stmt;
}

stmt_cond* init_stmt_cond()
{
    stmt_cond* new_stmt = malloc(sizeof(stmt_cond));
    new_stmt->cond_id = malloc(sizeof(char[255]));
    new_stmt->cond_stmt_list = malloc(sizeof(stmt_list_element));
    new_stmt->true_list = malloc(sizeof(stmt_list_element));
    new_stmt->false_list = malloc(sizeof(stmt_list_element));

    return new_stmt;
}

stmt_loop* init_stmt_loop()
{
    stmt_loop* new_stmt = malloc(sizeof(stmt_loop));
    new_stmt->cond_id = malloc(sizeof(char[255]));
    new_stmt->cond_stmt_list = malloc(sizeof(stmt_list_element));
    new_stmt->loop_list = malloc(sizeof(stmt_list_element));
    new_stmt->do_while = false;

    return new_stmt;
}

void add_to_list(stmt_list_element* list, stmt_list_element* element)
{
    stmt_list_element* current_element = list;
    while(current_element->next != NULL)
    {
        current_element = current_element->next;
    }

    if(current_element->next == NULL)
    {
        current_element->next = element;
    }
}

stmt_list_element* get_last_statement(stmt_list_element* list)
{
    stmt_list_element* current_element = list;
    while(current_element->next != NULL)
    {
        current_element = current_element->next;
    }

    return current_element;
}

stmt_list_element* stmt_from_cond(value* cond_expr, stmt_list_element* true_list, stmt_list_element* false_list)
{
    stmt_list_element* cond_list = stmt_from_expr(cond_expr);
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    reverse_stmt_list(&cond_list);

    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_COND;
    new_element->stmt.stmt_cond = init_stmt_cond();
    new_element->stmt.stmt_cond->cond_stmt_list = cond_list;
    char convert_arr[255];

    if(!strcmp(cond_expr->stmt_operator, ""))
    {
        if(cond_expr->valueType == VALUE_TYPE_SYMBOL)
        {
            strcpy(new_element->stmt.stmt_cond->cond_id, cond_expr->value.element->id);
        }

        if(cond_expr->valueType == VALUE_TYPE_VALUE)
        {
            sprintf(convert_arr, "%d", cond_expr->value.rval);
            strcpy(new_element->stmt.stmt_cond->cond_id, convert_arr);
        }
        else if(cond_expr->valueType == VALUE_TYPE_ARR_ELEMENT)
        {
            if(cond_expr->index->valueType == VALUE_TYPE_VALUE)
            {
                sprintf(convert_arr, "%s[%d]", cond_expr->value.element->id, cond_expr->index->value.rval);
                strcpy(new_element->stmt.stmt_cond->cond_id, convert_arr);
            }
            else if (cond_expr->index->valueType == VALUE_TYPE_SYMBOL)
            {
                sprintf(convert_arr, "%s[%s]", cond_expr->value.element->id, cond_expr->index->value.element->id);
                strcpy(new_element->stmt.stmt_cond->cond_id, convert_arr);
            }
        }
        else if (cond_expr->valueType == VALUE_TYPE_FUNCTION_CALL)
        {
            copy_function_call(&new_element->stmt.stmt_cond->cond_id, cond_expr);
        }
    }
    else
    {
        strcpy(new_element->stmt.stmt_cond->cond_id,  get_last_statement(cond_list)->stmt.stmt_expr->dest);
    }
    reverse_stmt_list(&true_list);
    new_element->stmt.stmt_cond->true_list = true_list;
    if(false_list != NULL)
    {
        reverse_stmt_list(&false_list);
        new_element->stmt.stmt_cond->false_list = false_list;
    }

    return new_element;
}

stmt_list_element* stmt_from_loop(value* cond_expr, stmt_list_element* loop_list, bool doWhile)
{
    stmt_list_element* cond_list = stmt_from_expr(cond_expr);
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    reverse_stmt_list(&cond_list);
    reverse_stmt_list(&loop_list);

    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_LOOP;
    new_element->stmt.stmt_loop = init_stmt_loop();
    new_element->stmt.stmt_loop->cond_stmt_list = cond_list;
    char convert_arr[255];

    if(!strcmp(cond_expr->stmt_operator, ""))
    {
        if(cond_expr->valueType == VALUE_TYPE_SYMBOL)
        {
            strcpy(new_element->stmt.stmt_loop->cond_id, cond_expr->value.element->id);
        }

        if(cond_expr->valueType == VALUE_TYPE_VALUE)
        {
            sprintf(convert_arr, "%d", cond_expr->value.rval);
            strcpy(new_element->stmt.stmt_loop->cond_id, convert_arr);
        }
        else if(cond_expr->valueType == VALUE_TYPE_ARR_ELEMENT)
        {
            if(cond_expr->index->valueType == VALUE_TYPE_VALUE)
            {
                sprintf(convert_arr, "%s[%d]", cond_expr->value.element->id, cond_expr->index->value.rval);
                strcpy(new_element->stmt.stmt_loop->cond_id, convert_arr);
            }
            else if (cond_expr->index->valueType == VALUE_TYPE_SYMBOL)
            {
                sprintf(convert_arr, "%s[%s]", cond_expr->value.element->id, cond_expr->index->value.element->id);
                strcpy(new_element->stmt.stmt_loop->cond_id, convert_arr);
            }
        }
        else if(cond_expr->valueType == VALUE_TYPE_FUNCTION_CALL)
        {
            copy_function_call(&new_element->stmt.stmt_loop->cond_id, cond_expr);
        }
    }
    else
    {
        strcpy(new_element->stmt.stmt_loop->cond_id, get_last_statement(cond_list)->stmt.stmt_expr->dest);
    }
    new_element->stmt.stmt_loop->loop_list = loop_list;
    new_element->stmt.stmt_loop->do_while = doWhile;

    return new_element;
}

stmt_list_element* stmt_from_return(value* expr)
{
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    
    stmt_list_element* return_stmt_list = NULL;
    
    if (expr != NULL)
    {
        return_stmt_list = stmt_from_expr(expr);
        reverse_stmt_list(&return_stmt_list);
    }

    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_RETURN;
    new_element->stmt.stmt_return = init_stmt_return();

    if (expr != NULL)
    {
        new_element->stmt.stmt_return->return_expr_list = return_stmt_list;

        new_element->stmt.stmt_return->return_id = malloc(sizeof(char[255]));
        char convert_arr[255];
        if(!strcmp(expr->stmt_operator, ""))
        {
            if(expr->valueType == VALUE_TYPE_SYMBOL)
            {
                strcpy(new_element->stmt.stmt_return->return_id, expr->value.element->id);
            }
            else if (expr->valueType == VALUE_TYPE_VALUE)
            {
                sprintf(convert_arr, "%d" ,expr->value.rval);
                strcpy(new_element->stmt.stmt_return->return_id, convert_arr);
            }
            else if(expr->valueType == VALUE_TYPE_ARR_ELEMENT)
            {
                if(expr->index->valueType == VALUE_TYPE_VALUE)
                {
                    sprintf(convert_arr, "%s[%d]", expr->value.element->id, expr->index->value.rval);
                    strcpy(new_element->stmt.stmt_return->return_id, convert_arr);
                }
                else if (expr->index->valueType == VALUE_TYPE_SYMBOL)
                {
                    sprintf(convert_arr, "%s[%s]", expr->value.element->id, expr->index->value.element->id);
                    strcpy(new_element->stmt.stmt_return->return_id, convert_arr);
                }
            }
            else if(expr->valueType == VALUE_TYPE_FUNCTION_CALL)
            {
                copy_function_call(&new_element->stmt.stmt_return->return_id, expr);
            }
        }
        else
        {
            strcpy(new_element->stmt.stmt_return->return_id, get_last_statement(return_stmt_list)->stmt.stmt_expr->dest);
        }

    }

    return new_element;
}

stmt_list_element* stmt_from_var_decl(symbol_table_element* var)
{
    stmt_list_element* new_element = malloc(sizeof(stmt_list_element));
    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_TMP_DEC;

    return new_element;
}

void set_expr_details (char* op, value* currentExpr, value* nextExpr)
{
    value* curr = currentExpr;
    while (true)
    {
        if(curr->next_expr != NULL)
        {
            curr = curr->next_expr;
        }
        else
        {
            break;
        }
    }
    
    if(op != NULL)
    {
        strcpy(curr->stmt_operator, op);
    }

    curr->next_expr = nextExpr;
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

            if (currentDefiniedElement->isParam == false)
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
            else 
            {
                break;
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

    if (strcmp(stmt_expr->operand2, "") && strcmp(stmt_expr->op, "") && strcmp(stmt_expr->op, "="))
        fprintf (fp, "%s = %s %s %s;\n", stmt_expr->dest, stmt_expr->operand1, stmt_expr->op, stmt_expr->operand2);

    if (!strcmp(stmt_expr->operand2, "") && strcmp(stmt_expr->op, "") && strcmp(stmt_expr->op, "="))
        fprintf (fp, "%s = %s %s;\n", stmt_expr->dest, stmt_expr->op, stmt_expr->operand1);

    if (!strcmp(stmt_expr->operand2, "") && strcmp(stmt_expr->op, "") && !strcmp(stmt_expr->op, "="))
        fprintf (fp, "%s = %s;\n", stmt_expr->dest, stmt_expr->operand1);

}

void print_while_loop (FILE *fp, stmt_loop* stmt_loop, int function_scope)
{

    int top_label = label_counter++;
    int loop_body = label_counter++;
    int after_loop = label_counter++;

    fprintf(fp, "label%d:\n", top_label);
    print_statements(fp, function_scope, stmt_loop->cond_stmt_list);

    fprintf(fp, "if (%s)\n{\ngoto label%d;\n}\ngoto label%d;\n", stmt_loop->cond_id, loop_body, after_loop);
    
    fprintf(fp, "label%d:\n", loop_body);
    print_statements(fp, function_scope, stmt_loop->loop_list);
    fprintf(fp, "goto label%d;\n", top_label);
    
    fprintf(fp, "label%d:\n", after_loop);

}

void print_do_while_loop (FILE *fp, stmt_loop* stmt_loop, int function_scope)
{

    int loop_body = label_counter++;
    int cond_label = label_counter++;
    int after_loop = label_counter++;

    fprintf(fp, "label%d:\n", loop_body);
    print_statements(fp, function_scope, stmt_loop->loop_list);
    fprintf(fp, "goto label%d;\n", cond_label);
    
    fprintf(fp, "label%d:\n", cond_label);

    print_statements(fp, function_scope, stmt_loop->cond_stmt_list);

    fprintf(fp, "if (%s)\n{\ngoto label%d;\n}\ngoto label%d;\n", stmt_loop->cond_id, loop_body, after_loop);

    fprintf(fp, "label%d:\n", after_loop);

}

void print_cond (FILE *fp, stmt_cond *stmt_cond, int function_scope)
{

    print_statements(fp, function_scope, stmt_cond->cond_stmt_list);

    int true_label = label_counter++;
    int false_label = label_counter++;
    int after_label = label_counter++;

    fprintf(fp, "if (%s)\n{\ngoto label%d;\n}\ngoto label%d;\n", stmt_cond->cond_id, true_label, false_label);

    fprintf(fp, "label%d:\n", true_label);
    print_statements(fp, function_scope, stmt_cond->true_list);
    fprintf(fp, "goto label%d;\n", after_label);

    fprintf(fp, "label%d:\n", false_label);
    print_statements(fp, function_scope, stmt_cond->false_list);
    fprintf(fp, "goto label%d;\n", after_label);

    fprintf(fp, "label%d:\n", after_label);

}

void print_statements (FILE *fp, int function_scope, stmt_list_element *first_stmt_of_block)
{

    stmt_list_element *current_stmt = first_stmt_of_block;

    while (true)
    {

        if (current_stmt->scope == function_scope)
        {

            switch (current_stmt->type) {

                case STMT_TYPE_EMPTY:
                break;

                case STMT_TYPE_COND:
                print_cond (fp, current_stmt->stmt.stmt_cond, function_scope);
                break;

                case STMT_TYPE_LOOP:
                if (current_stmt->stmt.stmt_loop->do_while)
                    print_do_while_loop(fp, current_stmt->stmt.stmt_loop, function_scope);
                else 
                    print_while_loop(fp, current_stmt->stmt.stmt_loop, function_scope);
                break;

                case STMT_TYPE_EXPR:
                print_stmt_expr(fp, current_stmt->stmt.stmt_expr);
                break;

                case STMT_TYPE_RETURN:
                print_statements(fp, function_scope, current_stmt->stmt.stmt_return->return_expr_list);
                fprintf (fp, "return %s;\n", current_stmt->stmt.stmt_return->return_id);
                break;

                case STMT_TYPE_TMP_DEC:
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

void print_local_variables (FILE *fp, int function_scope)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        if (currentElement->scope == function_scope && !currentElement->isParam)
        {

            if (currentElement->type == SYMBOL_TYPE_VAR)
                fprintf(fp, "int %s;\n", currentElement->id);

            if (currentElement->type == SYMBOL_TYPE_ARRAY)
                fprintf(fp, "int %s[%d];\n", currentElement->id, currentElement->length);

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
            fprintf(fp, "\n{\n");

            // print local variables
            print_local_variables(fp, currentElement->function_scope);

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

    reverse_stmt_list(&first_stmt);

    FILE *fp = 0;
    fp = fopen(file_string, "w+");

    print_global_elements(fp);
    fprintf(fp, "\n");
    print_functions(fp);

    fclose(fp);

}

void init_first_stmt () 
{
    first_stmt.next = NULL;
    first_stmt.scope = 0;
    first_stmt.type = STMT_TYPE_EMPTY;
    first_stmt.stmt.stmt_cond = NULL;
}

void add_to_global_stmt_list(stmt_list_element* stmt_list)
{
    stmt_list_element* current_element = &first_stmt;
    while(true)
    {
        if(current_element->next != NULL)
        {
            current_element = current_element->next;
        }
        else
        {
            current_element->next = stmt_list;
            break;
        }
    }
}

void string_statements_together(stmt_list_element* stmt_list, stmt_list_element* stmt)
{
    stmt_list_element* current_element = stmt_list;

    while(true)
    {
        if(current_element->next != NULL)
        {
            current_element = current_element->next;
        }
        else
        {
            current_element->next = stmt;
            break;
        }
    }
}

stmt_list_element* create_empty_stmt ()
{

    stmt_list_element* new_element;
    new_element = malloc(sizeof(stmt_list_element));
    new_element->stmt.stmt_expr = init_stmt_expr();
    new_element->next = NULL;
    new_element->scope = numberOfScopes;
    new_element->type = STMT_TYPE_EMPTY;
    
    return new_element;

}

void reverse_stmt_list (stmt_list_element **head_ref)
{
    stmt_list_element *prev = NULL;
    stmt_list_element *current = *head_ref;
    stmt_list_element *next = NULL;

    while (current != NULL)
    {
        next = current->next;

        current->next = prev;

        prev = current;
        current = next;
    }
    *head_ref = prev;
}