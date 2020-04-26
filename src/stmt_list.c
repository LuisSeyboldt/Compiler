#include "stmt_list.h"

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
