#include "compilador.h"


typedef enum CategoryType {VARIABLE, FUNCTION, PROCEDURE, PARAMETER} CategoryType;
typedef enum VariableType {INTEGER, BOOLEAN, UNDEFINED} VariableType;
typedef enum ParameterType {BYVAL, BYREFERENCE} ParameterType;

typedef union level {
  struct variable {
    int lexical_level;
    int offset;
    VariableType variable_type;
  } variable;
  struct parameter {
    short lexical_level;
    short offset;
    VariableType variable_type;
    ParameterType parameter_type;
  } parameter;
  struct procedure {
    int lexical_level;
    short label;
    short list_size;
    void * parameter_list_pointer;
  } procedure;
} symbol_union;


typedef struct {
  char identifier[TAM_TOKEN];
  CategoryType category; 
  symbol_union values; 
} symbol;

typedef struct {
  int idx;
  int max_size;
  symbol * symbols;
} symbol_table;


symbol_table * malloc_table(int table_size);
void free_table(symbol_table * table);
int insert_table(symbol_table * table, symbol new_symbol);
int remove_table(symbol_table * table, int symbol_index);
int search_table(symbol_table * table, char id[TAM_TOKEN]);
void print_table(symbol_table * table);
void print_variable_symbol(symbol s);
void print_parameter_symbol(symbol s);
