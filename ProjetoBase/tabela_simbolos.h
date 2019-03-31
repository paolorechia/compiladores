#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "compilador.h"

#define MAX_TABLE_SIZE 1024

typedef enum CategoryType {VARIABLE, FUNCTION, PROCEDURE, PARAMETER} CategoryType;
typedef enum VariableType {INTEGER, BOOLEAN, UNDEFINED} VariableType;
typedef enum ParameterType {BYVAL, BYREFERENCE} ParameterType;

/* Linked list struct */
typedef struct cel_struct{
    VariableType variable_type;
    ParameterType parameter_type;
    struct cel_struct * nxt;
} tnode;

typedef struct head{
    tnode * node;
    int size;
} thead;

/* Proper Symbol Table struct */
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
    int label;
    thead * parameter_list;
  } procedure;
  struct function {
    short lexical_level;
    short label;
    VariableType return_type;
    thead * parameter_list;
  } function;
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


/* Linked list functions */
thead * l_init();
void l_insert(thead *, VariableType, ParameterType);
int l_copy(thead * origin, thead * destination);
void l_print(thead * head);
void print_node(tnode * node);
int l_size(thead * head);
int rec_clear(tnode * node);
int l_clear(thead * head);
void l_free(thead * head);

/* Symbol table basic functions */
symbol_table * malloc_table(int table_size);
void free_table(symbol_table * table);
int insert_table(symbol_table * table, symbol new_symbol);
int remove_table(symbol_table * table, int number_to_remove);
int search_table(symbol_table * table, char id[TAM_TOKEN]);
void print_table(symbol_table * table);
void print_variable_symbol(symbol s);
void print_parameter_symbol(symbol s);
void print_procedure_symbol(symbol s);
void print_function_symbol(symbol s);
void variable_type_to_string(VariableType var_type, char * output_str);

/* Symbol token functions / table higher-level functions */
int insert_variable(symbol_table * symbol, char * identifier, int lexical_level, int offset);
int parse_var_type(char * token);
int update_var_type(symbol_table * table, char * token);
int remove_local_vars(symbol_table * table);
symbol * find_identifier(symbol_table * table, char * identifier);

#endif /*TABELA_SIMBOLOS_H*/
