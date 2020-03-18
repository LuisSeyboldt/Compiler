#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// enum for different symbol types
typedef enum symbol_type_enum 
{
    NUM, ARRAY, FUNC
} symbol_type;

// enum for different function return types
typedef enum func_return_type_enum
{
    INT, VOID
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

    // variables for arrays:
    unsigned int length; // array length

} symbol_table_element;

extern symbol_table_element *get_last_table_element(symbol_table_element *first_element);

#endif