#ifndef MACHINECODE_H
#define MACHINECODE_H

#include <stdio.h>

#include "symbol_table.h"

extern void print_machine_code (char* file_string);
void print_global_elements_as_mc (FILE *fp);
void print_function_header_mc (FILE *fp, symbol_table_element *currentElement);
void print_function_header_mc (FILE *fp, symbol_table_element *currentElement);

#endif