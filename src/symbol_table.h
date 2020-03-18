enum symbol_type 
{
    INT, ARRAY, FUNC
};

enum func_return_type
{
    INT, VOID
};

struct symbol_table
{

    char* id;
    symbol_type type;

    func_return_type return_type;
    unsigned int param_count;

    unsigned int length;

    // id, typ (int, array, func), 
    // func: return type, num param
    // array: length

};