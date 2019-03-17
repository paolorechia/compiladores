#include "compilador.h"


typedef enum CategoryType {VARIABLE, FUNCTION, PROCEDURE, PARAMETER} CategoryType;
typedef enum VariableType {INTEGER, BOOLEAN} VariableType;
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
} SymbolUnion;


typedef struct {
  char identifier[TAM_TOKEN];
  CategoryType category; 
  SymbolUnion values; 
} Symbol;


int insert_table(Symbol * table, Symbol new_symbol);
int remove_table(Symbol * table, int symbol_index);
int search_table(Symbol * table, char id[TAM_TOKEN]);
int malloc_table(Symbol * table, int table_size);
void free_table(Symbol * table);
