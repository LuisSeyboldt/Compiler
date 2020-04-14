#include "type_checks.h"
#include "symbol_table.h"

value valueFromId(char* id) 
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id); 
    new_val.valueType = VALUE_TYPE_SYMBOL;
    new_val.next = NULL;

    return new_val;
}

value valueFromNum(int num)
{
    value new_val;
    new_val.value.rval = num;
    new_val.valueType = VALUE_TYPE_VALUE;
    new_val.next = NULL;

    return new_val;
}

value valueFromArray(char *id)
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id);
    new_val.valueType = VALUE_TYPE_ARR_ELEMENT;
    new_val.next = NULL;

    return new_val;
}

value valueFromFunction(char *id)
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id);
    new_val.valueType = VALUE_TYPE_FUNCTION_CALL;
    new_val.next = NULL;

    return new_val;
}

void checkExpr(value expr1, value expr2)
{
    switch (expr1.valueType)
    {
    case VALUE_TYPE_SYMBOL:
        if(expr1.value.element->type == SYMBOL_TYPE_VAR)
        {
            caseLval(expr2);
        }
        else 
        {
            //error
        }
        break;
    case VALUE_TYPE_ARR_ELEMENT:
        caseLval(expr2);
        break; 
    
    default:
        //error
        break;
    }
}

void checkSingleExpr(value expr)
{
    if(expr.valueType == VALUE_TYPE_SYMBOL)
    {
        if(expr.value.element->type == SYMBOL_TYPE_ARRAY)
        {
            //error
        }
    }

    if(expr.valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr.value.element->return_type == FUNC_RETURN_TYPE_VOID)
        {
            //error
        }
    }

    return;
}

void checkRVal(value expr1, value expr2)
{
    // can not compare a array with something
    if(expr1.valueType == VALUE_TYPE_SYMBOL)
    {
        if(expr1.value.element->type == SYMBOL_TYPE_ARRAY)
        {
            //error
        }
    }
    if(expr2.valueType == VALUE_TYPE_SYMBOL)
    {
        if(expr2.value.element->type == SYMBOL_TYPE_ARRAY)
        {
            //error
        }
    }

    // cannot compare a void function call with something
    if(expr1.valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr1.value.element->return_type == FUNC_RETURN_TYPE_VOID)
        {
            //error
        }
    }
    if(expr2.valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr2.value.element->return_type == FUNC_RETURN_TYPE_VOID)
        {
            //error
        }
    }

    // everything else is fine
    return;
}

void caseLval(value expr) 
{
    if(expr.valueType == VALUE_TYPE_FUNCTION_CALL)
    {
        if(expr.value.element->return_type != FUNC_RETURN_TYPE_INT)
        {
            //error
        }
        else
        {
            return;
        }
    }
    if (expr.valueType == VALUE_TYPE_VALUE)
    {
        return;
    }
    if (expr.valueType == VALUE_TYPE_SYMBOL)
    {
        if (expr.value.element->type == SYMBOL_TYPE_VAR)
        {
            return;
        }
        else
        {
            //error
        }
    }
    if (expr.valueType == VALUE_TYPE_ARR_ELEMENT)
    {
        return;
    }
}

void checkIfNotVoid(func_return_type type)
{
    if(type == FUNC_RETURN_TYPE_VOID)
    {
        //error
    }

    return;
}

extern value valueFromFunctionWithParameterList(char *id, value *first_param_list_element)
{

    value new_value = valueFromFunction(id);

    int numberOfParamInCall = 0;
    
    // get number of parameters in function call
    value *currentElement = first_param_list_element;
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

    if (numberOfParamInCall <= 0)
    {
        // error
        exit(1);
    }

    // check if number matches function definition
    if (numberOfParamInCall != new_value.value.element->param_count)
    {
        // error
        exit(1);
    }

    // get first parameter
    symbol_table_element *firstDefinedParameter = get_element_in_scope(first_param_list_element->value.element->id, new_value.value.element->function_scope);

    // check parameters
    value *currentCallElement = first_param_list_element;
    symbol_table_element *currentDefiniedElement = firstDefinedParameter;
    while (true)
    {

        if (!compareParameters(currentDefiniedElement, currentCallElement) && 
        currentDefiniedElement->scope == new_value.value.element->function_scope)
        {
            // error
            exit(1);
        }

        if (currentCallElement->next == 0 && 
        (currentDefiniedElement->next->scope != new_value.value.element->function_scope || 
        !currentDefiniedElement->next->isParam || 
        currentDefiniedElement->next == NULL))
        {
            break;
        }

        if (currentCallElement->next != 0 && currentDefiniedElement->next != 0)
        {
            currentCallElement = currentCallElement->next; 
            currentDefiniedElement = currentDefiniedElement->next;
        }

    }

    return new_value;

}

bool compareParameters (symbol_table_element *definiedParameter, value *callParameter)
{

    if (!strcmp (definiedParameter->id, callParameter->value.element->id) &&
    definiedParameter->type == callParameter->value.element->type &&
    definiedParameter->isParam )
        return true;

    return false;

}