#include "type_checks.h"
#include "symbol_table.h"

//pointer on values from various types

value* valueFromSbl(symbol_table_element* sbl)
{
    value* new_val = malloc(sizeof(value));
    new_val->isTemp = true; 
    new_val->next_expr = NULL;
    new_val->value.element = sbl;
    new_val->valueType = VALUE_TYPE_SYMBOL;
    new_val->stmt_operator = malloc(sizeof(char[255]));
    new_val->next = NULL;
    new_val->index = NULL;

    return new_val;
}

value* valueFromId(char* id) 
{
    value* new_val = malloc(sizeof(value));
    new_val->value.element = get_element_in_namespace(id); 
    new_val->valueType = VALUE_TYPE_SYMBOL;
    new_val->next = NULL;
    new_val->next_expr = NULL;
    new_val->isTemp = false; 
    new_val->stmt_operator = malloc(sizeof(char[255]));
    new_val->index = NULL;

    return new_val;
}

value* valueFromNum(int num)
{
    value* new_val = malloc(sizeof(value));
    new_val->value.rval = num;
    new_val->valueType = VALUE_TYPE_VALUE;
    new_val->next = NULL;
    new_val->next_expr = NULL;
    new_val->isTemp = false; 
    new_val->stmt_operator = malloc(sizeof(char[255]));
    new_val->index = NULL;

    return new_val;
}

value* valueFromArray(char *id, value* index)
{
    value* new_val = malloc(sizeof(value));
    new_val->value.element = get_element_in_namespace(id);
    new_val->valueType = VALUE_TYPE_ARR_ELEMENT;
    new_val->next = NULL;
    new_val->next_expr = NULL;
    new_val->isTemp = false; 
    new_val->stmt_operator = malloc(sizeof(char[255]));
    new_val->index = index;

    return new_val;
}

value* valueFromFunction(char *id)
{
    value* new_val = malloc(sizeof(value));
    new_val->value.element = get_element_in_namespace(id);
    new_val->valueType = VALUE_TYPE_FUNCTION_CALL;
    new_val->next = NULL;
    new_val->next_expr = NULL;
    new_val->isTemp = false; 
    new_val->stmt_operator = malloc(sizeof(char[255]));
    new_val->index = NULL;

    return new_val;
}

void checkExpr(value* expr1, value* expr2)
{
    if(expr1 == NULL || expr2 == NULL || expr1->value.element == NULL || expr2->value.element == NULL)
    {
        return; 
    }
    switch (expr1->valueType)
    {
    case VALUE_TYPE_SYMBOL:
        if(expr1->value.element->type == SYMBOL_TYPE_VAR)
        {
            caseLval(expr2);
        }
        else 
        {
            yyerror("ERROR: mismatched types");
        }
        break;
    case VALUE_TYPE_ARR_ELEMENT:
        caseLval(expr2);
        break; 
    
    default:
        yyerror("ERROR: unkown error");
        break;
    }
}

void checkSingleExpr(value* expr)
{
    if(expr->valueType == VALUE_TYPE_SYMBOL)
    {
        if(expr->value.element->type == SYMBOL_TYPE_ARRAY)
        {
            yyerror("ERROR: Array not allowed here");
        }
    }

    if(expr->valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr->value.element->return_type == FUNC_RETURN_TYPE_VOID)
        {
            yyerror("ERROR: Return type void not allowed here");
        }
    }

    return;
}

void checkRVal(value* expr1, value* expr2)
{
    // can not compare a array with something
    if(expr1->valueType == VALUE_TYPE_SYMBOL)
    {
        if(expr1->value.element == NULL)
        {
            yyerror("ERROR: Unknown variable");
        }

        if(expr1->value.element->type == SYMBOL_TYPE_ARRAY)
        {
            yyerror("ERROR: Array can not be assigned");
        }
    }
    if(expr2->valueType == VALUE_TYPE_SYMBOL)
    {
        if(expr2->value.element == NULL)
        {
            yyerror("ERROR: Unknown variable");
        }
        if(expr2->value.element->type == SYMBOL_TYPE_ARRAY)
        {
            yyerror("ERROR: Can not assign array");
        }
    }

    // cannot compare a void function call with something
    if(expr1->valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr1->value.element->return_type == FUNC_RETURN_TYPE_VOID)
        {
            yyerror("ERROR: Return type void not allowed here");
        }
    }
    if(expr2->valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr2->value.element->return_type == FUNC_RETURN_TYPE_VOID)
        {
            yyerror("ERROR: Return type void not allowed here");
        }
    }

    // everything else is fine
    return;
}

void caseLval(value* expr) 
{
    if(expr->valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr->value.element->return_type != FUNC_RETURN_TYPE_INT)
        {
            yyerror("ERROR: Return type void not allowed here");
        }
        else
        {
            return;
        }
    }
    if (expr->valueType == VALUE_TYPE_VALUE)
    {
        return;
    }
    if (expr->valueType == VALUE_TYPE_SYMBOL)
    {
        if (expr->value.element->type == SYMBOL_TYPE_VAR)
        {
            return;
        }
        else
        {
            yyerror("ERROR: Types do not match");
        }
    }
    if (expr->valueType == VALUE_TYPE_ARR_ELEMENT)
    {
        return;
    }
}

//check for return type void
void checkIfNotVoid(func_return_type type)
{
    if(type == FUNC_RETURN_TYPE_VOID)
    {
        yyerror("ERROR: Type void not allowed here");
    }

    return;
}

//check return type of function
void checkReturnType(func_return_type rType, char* id)
{
    symbol_table_element* function = get_element_in_namespace(id);
    if(function->type != SYMBOL_TYPE_FUNC)
    {
        yyerror("ERROR: No function with matching name");
    }
    
    if(function->return_type != rType)
    {
        yyerror("ERROR: declaration type does not match definition type");
    }

} 

value* valueFromFunctionWithParameterList(char *id, value *firstParamListElement)
{

    value* new_value = valueFromFunction(id);

    int numberOfParamInCall = 1;
    
    // get number of parameters in function call
    value *currentElement = firstParamListElement;
    while (true)
    {
        if (currentElement->next == 0)
        {
            break;
        }

        if (currentElement->next != 0)
        {
            currentElement = currentElement->next; 
            numberOfParamInCall++;
        }

    }

    // check if number matches function definition
    if (numberOfParamInCall != new_value->value.element->param_count)
    {
        yyerror ("Error: Number of parameters in function call does not match number of parameters of the called function!\n Aborting!\n");
    }

    // get first parameter
    symbol_table_element *firstDefinedParameter = get_frist_parameter_of_func(new_value->value.element->function_scope);

    // check parameters
    value *currentCallElement = firstParamListElement;
    symbol_table_element *currentDefiniedElement = firstDefinedParameter;
    while (true)
    {


        if (!compareParameters(currentDefiniedElement, currentCallElement) || 
        currentDefiniedElement->scope != new_value->value.element->function_scope)
        {
            yyerror ("Error: Parameter type of called function does not match parameter!\n Aborting!\n");
        }

        if (currentDefiniedElement->next == NULL)
        {
            if (currentCallElement->next == NULL)
                break;
        }
        else 
        {
            if (currentCallElement->next == NULL &&
            (currentDefiniedElement->next->scope != new_value->value.element->function_scope || 
            !currentDefiniedElement->next->isParam))
                break;
        }

        if (currentCallElement->next != 0 && currentDefiniedElement->next != 0)
        {
            currentCallElement = currentCallElement->next; 
            currentDefiniedElement = currentDefiniedElement->next;
        }

    }

    //cleanFunctionParameterMemory (firstParamListElement);
    new_value->next = firstParamListElement;
    return new_value;

}

//compare type of values and parameters
bool compareParameters (symbol_table_element *definiedParameter, value* callParameter)
{

    if (callParameter->valueType == VALUE_TYPE_VALUE)
    {
        if (definiedParameter->type == SYMBOL_TYPE_VAR && definiedParameter->isParam)
            return true;
        else 
            yyerror("Error: RVAL cannot be assigned to non int parameter in function call!\n Aborting!\n");
    }

    if (callParameter->valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if (definiedParameter->type == SYMBOL_TYPE_VAR &&
            callParameter->value.element->return_type == FUNC_RETURN_TYPE_INT &&
            definiedParameter->isParam)
            return true;
        else 
            yyerror("Error: Function return value is incompatible with parameter!\n Aborting!\n");
    }

    if (callParameter->valueType == VALUE_TYPE_ARR_ELEMENT)
    {
        if (definiedParameter->type == SYMBOL_TYPE_VAR &&
            definiedParameter->isParam)
            return true;
        else 
            yyerror ("Error: Array element type does not match parameter type!\n Aborting!\n");
    }

    if (definiedParameter->type == callParameter->value.element->type &&
        definiedParameter->isParam )
        return true;

    return false;

}

//copy function parameter
value* allocFunctionParameter (value* someValue)
{
    value *valuePtr = malloc (sizeof(someValue));
    memcpy (valuePtr, &someValue, sizeof(someValue));
    return valuePtr;
}

void cleanFunctionParameterMemory (value *firstFunctionCallParameter)
{
    value* current_element = firstFunctionCallParameter;
    if(current_element->next == NULL)
    {
        return;
    }

    while(true)
    {
        if(current_element->next != NULL)
        {
            value* tmp = current_element->next;

            free(current_element);

            current_element = tmp;
        }
        else
        {
            //clean the last existing element
            if (current_element != NULL)
            {
                free(current_element);
            }
            return;
        }
    }

    return;
}

void checkFuncReturn(value* returnExpr)
{
    // get current function (numberOfScopes - 1) gets the currently active scope
    symbol_table_element* func = get_function_from_scope(numberOfScopes);
    if(func == NULL)
    {
        yyerror("ERROR: Function not found");
    }

    // return value is array element or rValue
    if(returnExpr->valueType == VALUE_TYPE_ARR_ELEMENT || returnExpr->valueType == VALUE_TYPE_VALUE)
    {
        if(func->return_type != FUNC_RETURN_TYPE_INT)
        {
            yyerror("ERROR: function return type does not match return value");
        }
    }

    // return value is array or variable
    if(returnExpr->valueType == VALUE_TYPE_SYMBOL)
    {
        //array
        if(returnExpr->value.element->type == SYMBOL_TYPE_ARRAY)
        {
            yyerror("ERROR: cannot return a array");
        }

        // var
        if(returnExpr->value.element->type == SYMBOL_TYPE_VAR)
        {
            if(func->return_type != FUNC_RETURN_TYPE_INT)
            {
                yyerror("ERROR: function return type does not match return value");
            }
        }
    }

    return;
}

void checkVoidReturn()
{
    // get current function (numberOfScopes - 1) gets the currently active scope
    symbol_table_element* func = get_function_from_scope(numberOfScopes);

    if(func == NULL)
    {
        yyerror("ERROR: function not found");
    }

    if(func->return_type != FUNC_RETURN_TYPE_VOID)
    {
        yyerror("ERROR: cannot return void from non-void function");
    }

    return;
}


void checkParams(char* id, value* params)
{
    // get current function (numberOfScopes - 1) gets the currently active scope
    symbol_table_element* func = get_element_in_namespace(id);
    if(func == NULL)
    {
        yyerror("ERROR: function not found");
    }

    value* currentCallParam = params;
    symbol_table_element* currentFuncParam = func->next;

    //check availability of parameters
    if(currentFuncParam == NULL || !currentFuncParam->isParam)
    {
        yyerror("ERROR: function has no parameters");
    }

    for(int i = 0; i < func->param_count; i++)
    {
        // check if parameter types match if call param is a symbol
        if(currentCallParam->valueType == VALUE_TYPE_SYMBOL || currentCallParam->valueType == VALUE_TYPE_ARR_ELEMENT)
        {
            if(currentCallParam->value.element->type != currentFuncParam->type)
            {
                yyerror("ERROR: function call parameters types do not match function declaration");
            }

            if(currentFuncParam->type == SYMBOL_TYPE_ARRAY)
            {
                if(currentCallParam->value.element->length != currentFuncParam->length)
                {
                    yyerror("ERROR: array parameter size does not match function declaration");
                }
            }

            // check if there is next function parameter
            if(currentFuncParam->next != NULL)
            {
                // function has no more parameters but there are more call arguments
                if(!currentFuncParam->next->isParam && currentCallParam->next != NULL)
                {
                    yyerror("ERROR: mismatched number of parameters");
                }

                // function has another parameter, but there are no more call parameters
                if(currentFuncParam->next->isParam && currentCallParam->next == NULL)
                {
                    yyerror("ERROR: mismatched number of parameters");
                }

                // set current params to next params
                if(currentFuncParam->next->isParam && currentCallParam->next != NULL)
                {
                    currentFuncParam = currentFuncParam->next;
                    currentCallParam = currentCallParam->next;
                }
            }
            else
            {
                if(currentCallParam->next != NULL)
                {
                    yyerror("ERROR: mismatched number of parameters");
                }
            }
        }

        if(currentCallParam->valueType == VALUE_TYPE_VALUE)
        {
            if(currentFuncParam->type != SYMBOL_TYPE_VAR)
            {
                yyerror("ERROR: function call parameters types do not match function declaration");
            }
        }
    }
}

//check for null parameters
void checkZeroParams(char* id)
{
    symbol_table_element* func = get_element_in_namespace(id);
    if(func == NULL)
    {
        yyerror("ERROR: function not found");
    }

    if(func->param_count != 0)
    {
        yyerror("ERROR: function declaration defines no parameters");
    }
}