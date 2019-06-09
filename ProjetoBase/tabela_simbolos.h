#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "compilador.h"

#define MAX_TABLE_SIZE 1024
#define LABEL_MAX_SIZE 4
#define LABEL_DIGITS 3

typedef enum CategoryType {VARIABLE, FUNCTION, PROCEDURE, PARAMETER} CategoryType;
typedef enum VariableType {INTEGER, BOOLEAN, UNDEFINED} VariableType;
typedef enum ParameterType {BYVAL, BYREFERENCE, NULL_PARAM} ParameterType;

/* Linked list struct */
typedef struct cel_struct{
    char identifier[TAM_TOKEN];
    VariableType variable_type;
    ParameterType parameter_type;
    struct cel_struct * nxt;
} tnode;

typedef struct head{
    tnode * node;
    int size;
} thead;

typedef struct fvariable {
  int lexical_level;
  int offset;
  VariableType type;
} fvariable;

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
    short lexical_level;
    short label;
    thead * parameter_list;
  } procedure;
  struct function {
    /* TODO: fix */
    short lexical_level;
    short label;
    fvariable * return_variable;
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
tnode * pop_first(thead * head); // QUEUE pop
void l_insert(thead *, char id[TAM_TOKEN], VariableType, ParameterType);
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
symbol * peek_table(symbol_table * table);
void print_table(symbol_table * table);
void print_variable_symbol(symbol s);
void print_parameter_symbol(symbol s);
void print_procedure_symbol(symbol s);
void print_function_symbol(symbol s);
void label_to_string(int c, char * ds);
void variable_type_to_string(VariableType var_type, char * output_str);
void category_type_to_string(CategoryType cat_type, char * output_str);
int label_to_integer(char * input_str);

/* Symbol token functions / table higher-level functions */
int insert_variable(symbol_table * symbol, char * identifier, int lexical_level, int offset);
void insert_procedure(symbol_table * table, char * ident_token, int lexical_level, char * label);
int update_subroutine_parameters(symbol_table * table);
VariableType insert_parameter(symbol_table * table, char * ident_token, int lexical_level, char *  var_type_str, ParameterType param_type);
void insert_function(symbol_table * table, char * ident_token, int lexical_level, char * label);
int update_function_return_type(symbol_table * table, char * token);
int parse_var_type(char * token);
int update_var_type(symbol_table * table, char * token);
int remove_local_vars(symbol_table * table);
symbol * find_identifier(symbol_table * table, char * identifier);
int assemble_read_write_instruction(char * temp_str, const char * instruction, symbol * symp_pter);
int check_symbol_category(symbol * symb_pter, CategoryType cat_type);

#endif /*TABELA_SIMBOLOS_H*/
