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

    int scope; // scope of the function (0 is global)

    // variables for arrays:
    unsigned int length; // array length

} symbol_table_element;

// Global first elemnt of the symbol table
symbol_table_element first_element;
int numberOfScopes = 1;

extern symbol_table_element *get_last_table_element();
extern symbol_table_element* init_sbl (char* id, int length, symbol_type type);
void add_sbl(symbol_table_element* symbol);
extern bool element_in_namespace(symbol_table_element *element);
extern void add_fun (char* id, func_return_type rtype, unsigned int param_count);
extern void print_all_symbol_tables ();
extern void print_symbol_table (int scope);
extern void cleanMem();

#endif