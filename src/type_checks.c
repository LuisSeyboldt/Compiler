#include "type_checks.h"
#include "symbol_table.h"

value valueFromId(char* id) 
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id); 
    new_val.valueType = VALUE_TYPE_SYMBOL;

    return new_val;
}

value valueFromNum(int num)
{
    value new_val;
    new_val.value.rval = num;
    new_val.valueType = VALUE_TYPE_VALUE;

    return new_val;
}

value valueFromArray(char *id)
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id);
    new_val.valueType = VALUE_TYPE_ARR_ELEMENT;

    return new_val;
}

value valueFromFunction(char *id)
{
    value new_val;
    new_val.value.element = get_element_in_namespace(id);
    new_val.valueType = VALUE_TYPE_FUNCTION_CALL;

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

void checkReturnType(func_return_type rType, char* id)
{
    symbol_table_element* function = get_element_in_namespace(id);
    if(function->type != SYMBOL_TYPE_FUNC)
    {
        // error element is not a function
    }
    
    if(function->return_type != rType)
    {
        // error declaration type does not match definition type
    }

} 
