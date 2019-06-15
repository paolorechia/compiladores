#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#include "compilador.h"
#include <stdint.h>

#define MAX_TABLE_SIZE 1024
#define LABEL_MAX_SIZE 4
#define LABEL_DIGITS 3

typedef enum CategoryType {VARIABLE, FUNCTION, PROCEDURE, PARAMETER, LABEL_SYMBOL_TYPE, NULL_CAT} CategoryType;
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

/* Proper Symbol Table struct */
typedef union level {
  struct variable {
    int32_t offset;
    VariableType variable_type;
  } variable;
  struct parameter {
    int32_t offset;
    VariableType variable_type;
    ParameterType parameter_type;
  } parameter;
  struct procedure {
    int32_t label;
    thead * parameter_list;
  } procedure;
  struct function {
    int32_t label;
    int32_t offset;
    VariableType variable_type;
    thead * parameter_list;
  } function;
  struct label {
    int32_t label;
  } label;
} symbol_union;


typedef struct {
  char identifier[TAM_TOKEN];
  int32_t lexical_level;
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
int local_search_table(symbol_table * table, char id[TAM_TOKEN], int lexical_level);
symbol * peek_table(symbol_table * table);
void print_table(symbol_table * table);
void print_variable_symbol(symbol s);
void print_parameter_symbol(symbol s);
void print_procedure_symbol(symbol s);
void print_function_symbol(symbol s);
void print_label_symbol(symbol s);
void label_to_string(int c, char * ds);
void variable_type_to_string(VariableType var_type, char * output_str);
void category_type_to_string(CategoryType cat_type, char * output_str);
int label_to_integer(char * input_str);

/* Symbol token functions / table higher-level functions */
int insert_variable(symbol_table * symbol, char * identifier, int lexical_level, int offset);
void insert_procedure(symbol_table * table, char * ident_token, int lexical_level, char * label);
int update_subroutine_parameters(symbol_table * table);
int copy_parameters_to_table(symbol_table * table);
int remove_parameters(symbol_table * table);
int remove_nested_procedures(symbol_table * table, int current_lexical_level);
VariableType insert_parameter(symbol_table * table, char * ident_token, int lexical_level, char *  var_type_str, ParameterType param_type);
void insert_function(symbol_table * table, char * ident_token, int lexical_level, char * label);
int update_function_return_type(symbol_table * table, char * token);
void insert_label(symbol_table * table, char * ident_token, int lexical_level, char * label);
int parse_var_type(char * token);
int update_var_type(symbol_table * table, char * token);
int remove_local_vars(symbol_table * table);
symbol * find_identifier(symbol_table * table, char * identifier);
symbol * find_local_identifier(symbol_table * table, char * identifier, int lexical_level);
int assemble_read_write_instruction(char * temp_str, const char * instruction, symbol * symp_pter);
int check_symbol_category(symbol * symb_pter, CategoryType cat_type, CategoryType cat_type2);

#endif /*TABELA_SIMBOLOS_H*/
