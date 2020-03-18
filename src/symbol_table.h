#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// enum for different symbol types
enum symbol_type 
{
    INT, ARRAY, FUNC
};

// enum for different function return types
enum func_return_type
{
    INT, VOID
};

// id, typ (int, array, func), 
// func: return type, num param
// array: length
typedef struct symbol_table_struct
{

    // general variables for all symbols:
    char* id; // name of the element
    symbol_type type; // type of the symbol
    symbol_table_struct *next; // next element in the symbol table 

    // variables for functions:
    func_return_type return_type; // return type of the function 
    unsigned int param_count; // parameter count of the function

    // variables for arrays:
    unsigned int length; // array length

} symbol_table_type;

extern symbol_table_type symbol_table;

#endif