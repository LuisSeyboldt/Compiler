#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// enum for different symbol types
typedef enum symbol_type_enum 
{
    SYMBOL_TYPE_VAR, SYMBOL_TYPE_ARRAY, SYMBOL_TYPE_FUNC
} symbol_type;

// enum for different function return types
typedef enum func_return_type_enum
{
    FUNC_RETURN_TYPE_NONE, FUNC_RETURN_TYPE_INT, FUNC_RETURN_TYPE_VOID
} func_return_type;

// id, typ (int, array, func), 
// func: return type, num param
// array: length
typedef struct symbol_table_struct
{

    // general variables for all symbols:
    char* id; // name of the element
    symbol_type type; // type of the symbol
    struct symbol_table_struct *next; // next element in the symbol table 

    // variables for functions:
    func_return_type return_type; // return type of the function 
    unsigned int param_count; // parameter count of the function
    int function_scope;
    bool definied;

    int scope; // scope of the symbol (0 is global)

    bool isParam; // is the symbol parameter?

    // variables for arrays:
    unsigned int length; // array length

} symbol_table_element;

// union for the parameter list data
typedef struct parameter_list
{
    int numberOfParameters;
    symbol_table_element* symbols;
} parameter_list;

// Global first elemnt of the symbol table
symbol_table_element first_element;
extern int numberOfScopes;

extern symbol_table_element* get_function_from_scope (int scope);
extern parameter_list* init_param_list(int paramCount, symbol_table_element* symbols);
extern symbol_table_element *get_last_table_element();
extern symbol_table_element* init_sbl (char* id, int length, symbol_type type);
void add_sbl(symbol_table_element* symbol, bool isLocal, bool isParam);
extern bool element_in_namespace_scope (symbol_table_element *element, int scope);
extern bool element_in_namespace(symbol_table_element *element);
extern symbol_table_element *get_element_in_namespace (char* id);
extern symbol_table_element *check_for_existing_function(symbol_table_element* new_function);
extern void delete_elements_of_scope (int scope);
extern void add_fun (char* id, func_return_type rtype, unsigned int param_count, bool definition);
extern void print_all_symbol_tables ();
extern void set_scope(symbol_table_element* symbols, int scope, bool isParam);
extern void print_symbol_table (int scope);
extern void cleanMem();
extern char* get_ID_of_scope(int scope);
extern bool scope_has_elements(int scope);
extern symbol_table_element *get_element_in_scope(char *id, int scope);
extern symbol_table_element *get_frist_parameter_of_func(int functionScope);

#endif