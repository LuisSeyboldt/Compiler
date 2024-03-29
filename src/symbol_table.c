#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

//create parameter list
parameter_list* init_param_list(int paramCount, symbol_table_element* symbols)
{
    parameter_list* new_param_list = malloc(sizeof(parameter_list));
    new_param_list->numberOfParameters = paramCount;
    new_param_list->symbols = symbols;

    return new_param_list;
}

//create symbol tabel element
symbol_table_element* init_sbl (char* id, int length, symbol_type type)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = type;
    new_symbol->id = malloc(sizeof(id));
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length = length;
    new_symbol->param_count = 0;
    new_symbol->return_type = FUNC_RETURN_TYPE_NONE;
    new_symbol->scope = 0;
    new_symbol->function_scope = 0;
    new_symbol->isParam = false;

    return new_symbol;
}

//add symbol to symbol table
void add_sbl(symbol_table_element* symbol, bool isLocal, bool isParam)
{
    // sets the correct function scope if the symbol is local
    if(isLocal)
    {
        set_scope(symbol, numberOfScopes, isParam);
    }
    else // otherwise the global scope is set
    {
        set_scope(symbol, 0, isParam);
    }
    

    // if the element is already in the namsepace: do not add to symbol table 
    if (element_in_namespace(symbol))
    {
        fprintf(stderr, "Duplicate definition of variable!\n");
        free(symbol);
        exit(1);
    }

    // the element is appended to the symbol table
    symbol_table_element *last = get_last_table_element();
    last->next = symbol;

}

void set_scope(symbol_table_element* symbols, int scope, bool isParam)
{
    symbol_table_element* current_element = symbols;
    while(true)
    {
        current_element->scope = scope;
        current_element->isParam = isParam;
        if(current_element->next != NULL)
        {
            current_element = current_element->next;
        }
        else
        {
            return;
        }
    }

    return;
}

//add function to symbol table
void add_fun (char* id, func_return_type rtype, unsigned int param_count, bool definition)
{
    symbol_table_element *new_symbol;
    new_symbol = malloc(sizeof(symbol_table_element));
    new_symbol->type = SYMBOL_TYPE_FUNC;
    new_symbol->id = malloc(sizeof(id));
    strcpy(new_symbol->id, id);
    new_symbol->next = 0;
    new_symbol->length = 0;
    new_symbol->param_count = param_count;
    new_symbol->return_type = rtype;
    new_symbol->scope = 0;
    new_symbol->definied = definition;
    new_symbol->function_scope = numberOfScopes;
    new_symbol->isParam = false;
    new_symbol->numberOfTmpVars = 0;

    // if definition: check for existing entries
        // if definition exists: error duplicate definition
        // if declaration: delete entries and create fresh
        // create
    // if declaration: check for existing entries
        // if definition: error already defined
        // if declaratopm: error already declared
        // create

    // check for existing entries
    symbol_table_element* found_function = check_for_existing_function(new_symbol);
    if (found_function != 0)
    {
        if (definition)
        {
            if (found_function->definied)
            {
                fprintf(stderr, "Error: Duplicate definition of function!\n");
                exit(1); 
            }
            else 
            {
                // reduce the number of scopes to prevent duplicate entries in symbol table for definition and declaration
                new_symbol->function_scope = found_function->function_scope;
                //numberOfScopes--;
                jumpBack = true;
                oldScope = numberOfScopes;
                numberOfScopes = found_function->function_scope;
                delete_elements_of_scope(found_function->function_scope);
            }
        }
        else
        {
            if (found_function->definied)
            {
                fprintf(stderr, "Error: Function already definied!\n");
                exit(1); 
            }
            else 
            {
                fprintf(stderr, "Error: Duplicate declaration of function!\n");
                exit(1);
            }
        }
    }

    // append the symbol to the symbol table
    symbol_table_element *last = get_last_table_element();
    last->next = new_symbol;

}

// this function deletes all elements of a specified scope
void delete_elements_of_scope (int scope)
{

    // two iterators are needed: one for the current element and one for the previous element
    // when the current element is deleted the next element is appended to the previous one 
    // to ensure that the linked list stays intact
    symbol_table_element *currentElement = &first_element, *prev = 0;

    while (true)
    {
        if(currentElement != NULL)
        {
            bool delete = false;

            if (currentElement->type == SYMBOL_TYPE_FUNC)
            {
                if (currentElement->function_scope == scope)
                    delete = true;
            }
            else 
            {
                if (currentElement->scope == scope)
                    delete = true;
            }

            if (delete)
            {

                symbol_table_element *nextElement = currentElement->next;

                if (prev != 0)
                    prev->next = nextElement;
                else 
                    first_element = *nextElement;

                free (currentElement);
                currentElement = nextElement;

                // Nothing else in the list
                if(nextElement == NULL)
                    return;

                continue;

            }
        }

        if (currentElement->next == 0)
            return;

        // go to next element
        if (currentElement->next != 0)
        {
            prev = currentElement;
            currentElement = currentElement->next;
        }
    }
}

symbol_table_element* check_for_existing_function(symbol_table_element* new_function)
{
    symbol_table_element *currentElement = &first_element;

    while (true)
    {
        if (currentElement->type == SYMBOL_TYPE_FUNC)
        {
            // check if names are equal
            if (!strcmp(currentElement->id, new_function->id))
            {
                // check if return type and parameters are the same
                if (currentElement->return_type == new_function->return_type && currentElement->param_count == new_function->param_count)
                    return currentElement;
                else
                {
                    fprintf(stderr, "Error: Function overloading cannot be performed (in ANSI-C)!\n");
                    exit(1);       
                }
            }
        }

        // if no function has been found: return 0
        if (currentElement->next == 0)
            return 0;

        // go to next element
        if (currentElement->next != 0)
            currentElement = currentElement->next;
    }

    return 0;
}

//get last symbol table element
symbol_table_element *get_last_table_element()
{

    symbol_table_element *currentElement = &first_element;
    
    do 
    {
        //next element=last element?
        if (currentElement->next != 0)
            currentElement = currentElement->next;

        if (currentElement->next == 0)
            return currentElement;

    }
    while (currentElement->next != 0);

    return 0;

}

bool element_in_namespace_scope (symbol_table_element *element, int scope)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {
        
        if (currentElement->id != 0 && currentElement->scope == scope)
        {
            if (!strcmp(currentElement->id, element->id) && 
            currentElement->param_count == element->param_count && 
            currentElement->return_type == element->return_type)
            {

                if (((element->type == SYMBOL_TYPE_VAR || element->type == SYMBOL_TYPE_ARRAY) &&
                (currentElement->type == SYMBOL_TYPE_VAR || currentElement->type == SYMBOL_TYPE_ARRAY)) ||
                (element->type == SYMBOL_TYPE_FUNC && currentElement->type == SYMBOL_TYPE_FUNC))
                    return true;

            }
        }

        if (currentElement->next == 0)
            return false;

        if (currentElement->next != 0)
            currentElement = currentElement->next; 

    }
    
    return false;

}

bool element_in_namespace(symbol_table_element *element)
{

    return element_in_namespace_scope (element, element->scope);

}

// gets the symbol table element for an id
symbol_table_element *get_element_in_namespace (char* id)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        // only check an elememt id if it has a value
        if(currentElement->id != 0)
        {
            if (!strcmp(id, currentElement->id))
            {
                if (element_in_namespace (currentElement) || currentElement->scope == 0)
                    return currentElement;
            }
        }

        // iterate through the symbol table
        if (currentElement->next == 0)
            return NULL;

        if (currentElement->next != 0)
            currentElement = currentElement->next; 

    }
    
    return NULL;


}

// get the function element with a specified scope
symbol_table_element* get_function_from_scope (int scope)
{
    symbol_table_element *currentElement = &first_element;

    while (true)
    {
        // skip first element
        if(currentElement->id != NULL)
        {
            if(currentElement->type == SYMBOL_TYPE_FUNC)
            {
                if (currentElement->function_scope == scope)
                {
                    return currentElement;
                }
            }
        }

        if (currentElement->next == 0)
            return NULL;

        if (currentElement->next != 0)
            currentElement = currentElement->next; 

    }
    
    return NULL;
}

// print all symbol tables
void print_all_symbol_tables()
{
    for (int i = 0; i < numberOfScopes; i++)
        print_symbol_table (i);
}

// print a symbol table with a specified scope
void print_symbol_table (int scope)
{

    // only print the table if it has elements
    if (!scope_has_elements(scope))
        return;

    symbol_table_element *currentElement = &first_element;
    
    // create new file for the symbol table
    FILE *fp = 0;
    
    char fileString[128];
    char numberString[10];
    sprintf(numberString, "%d", scope);
    strcpy(fileString, "symbol_table_");
    strcat(fileString, numberString);
    strcat(fileString, ".txt\0");
    fp = fopen(fileString, "w+");

    // find ID of the function scope
    char* scopeID = 0;
    scopeID = get_ID_of_scope(scope);

    if (scopeID == 0)
    {
        fprintf(stderr, "Error printing the symbol table\n");
        return;
    }

    fprintf(fp, "Start symbol table (scope: %s):\n", scopeID);
    fprintf(stderr, "Start symbol table (scope: %s):\n", scopeID);

    while (true)
    {
        
        if (currentElement->id != 0 && currentElement->scope == scope)
        {
            switch (currentElement->type)
            {
                case SYMBOL_TYPE_VAR:
                if (currentElement->isParam)
                {
                    fprintf(fp, "Type: Function Parameter Variable, ID: %s\n", currentElement->id);
                    fprintf(stderr, "Type: Function Parameter Variable, ID: %s\n", currentElement->id);
                }
                else
                {
                    fprintf(fp, "Type: Variable, ID: %s\n", currentElement->id);
                    fprintf(stderr, "Type: Variable, ID: %s\n", currentElement->id);
                }
                break;

                case SYMBOL_TYPE_FUNC:
                fprintf(fp, "Type: Function, ID: %s, ", currentElement->id);
                fprintf(stderr, "Type: Function, ID: %s, ", currentElement->id);
            
                if (currentElement->return_type == FUNC_RETURN_TYPE_INT)
                {
                    fprintf(fp, "Return type: int, ");
                    fprintf(stderr, "Return type: int, ");
                }

                if (currentElement->return_type == FUNC_RETURN_TYPE_VOID)
                {
                    fprintf(fp, "Return type: void, ");
                    fprintf(stderr, "Return type: void, ");
                }

                if (currentElement->return_type == FUNC_RETURN_TYPE_NONE)
                {
                    fprintf(fp, "Return type: ERROR, ");+
                    fprintf(stderr, "Return type: ERROR, ");
                }

                fprintf(fp, "Parameter count: %d\n", currentElement->param_count);
                fprintf(stderr, "Parameter count: %d\n", currentElement->param_count);
                
                fprintf(fp, "Number of temporary variables: %d\n", currentElement->numberOfTmpVars);
                fprintf(stderr, "Number of temporary variables: %d\n", currentElement->numberOfTmpVars);
                break;

                case SYMBOL_TYPE_ARRAY:
                if (currentElement->isParam)
                {
                    fprintf(fp, "Type: Function Parameter Array, ID: %s, Array length: %d\n", currentElement->id, currentElement->length);
                    fprintf(stderr, "Type: Function Parameter Array, ID: %s, Array length: %d\n", currentElement->id, currentElement->length);
                }
                else
                {
                    fprintf(fp, "Type: Array, ID: %s, Array length: %d\n", currentElement->id, currentElement->length);
                    fprintf(stderr, "Type: Array, ID: %s, Array length: %d\n", currentElement->id, currentElement->length);
                }
                break;

                default:
                    fprintf (fp, "Error! Unrecognized type!\n");
                    fprintf (stderr, "Error! Unrecognized type!\n");
                break;

            };
        }

        if (currentElement->next == 0)
        {
            fclose(fp);

            if (scopeID != 0)
                free(scopeID);

            return;
        }

        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

    fclose(fp);

     if (scopeID != 0)
        free(scopeID);

    return;

}


// funktion to free allocated memory of the symbol table
void cleanMem()
{
    symbol_table_element* current_element = &first_element;
    if(current_element->next == NULL)
    {
        return;
    }

    while(true)
    {
        if(current_element->next != NULL)
        {
            symbol_table_element* tmp = current_element->next;

            // check for first element
            if (current_element->id != NULL)
            {
                free(current_element);
            }
            current_element = tmp;
        }
        else
        {
            if (current_element != NULL)
            {
                free(current_element);
            }
            return;
        }
    }

    return;
}

char* get_ID_of_scope(int scope)
{

    // var for return
    char* scopeID = 0;

    // return "global" string if the scope is global (= 0)
    if (scope == 0)
    {
        scopeID = malloc(sizeof("global\0"));
        strcpy(scopeID,"global\0");
        return scopeID;
    }

    // iterator
    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        // function is found when the element is of the type func and has the searched scope
        if (currentElement->type == SYMBOL_TYPE_FUNC && currentElement->function_scope == scope)
        {
            scopeID = malloc(sizeof(currentElement->id)); // allocate new memory
            strcpy(scopeID, currentElement->id); // copy the string
            return scopeID; 
        }

        // if the end of the list has been reached return with scopeID = 0
        if (currentElement->next == 0)
            return scopeID;

        // go to next element
        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

    return scopeID;

}

bool scope_has_elements(int scope)
{

    // iterator
    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        if (currentElement->type == SYMBOL_TYPE_FUNC)
        {
            if (currentElement->function_scope == scope)
                return true;
        }
        else 
        {
            if (currentElement->scope == scope)
                return true;
        }

        if (currentElement->next == 0)
            return false;

        // go to next element
        if (currentElement->next != 0)
            currentElement = currentElement->next;

    }

    return false;
}

// get elemnt with specified id in a specified scope
symbol_table_element *get_element_in_scope(char *id, int scope)
{

    symbol_table_element *currentElement = &first_element;

    while (true)
    {
        if(currentElement->id != 0)
        {
            if (!strcmp(id, currentElement->id))
            {
                if (element_in_namespace_scope (currentElement, scope))
                    return currentElement;
            }
        }

        if (currentElement->next == 0)
            return NULL;

        if (currentElement->next != 0)
            currentElement = currentElement->next; 

    }
    
    return NULL;

}

// finds and returns the first (in symbol table) parameter of a function
symbol_table_element *get_frist_parameter_of_func(int functionScope)
{

     // iterator
    symbol_table_element *currentElement = &first_element;

    while (true)
    {

        // if the scope of the element is correct 
        // and the element if a parameter return the element
        if (currentElement->scope == functionScope &&
        currentElement->isParam == true)
            return currentElement;

        // end of list and no element found -> error
        if (currentElement->next == 0)
            return NULL;

        // iterate
        if (currentElement->next != 0)
            currentElement = currentElement->next; 

    }
    
    return NULL;

}

void checkJumpBack()
{
    if (jumpBack)
    {
        numberOfScopes = oldScope;
        oldScope = 0;
        jumpBack = false;
    }
}