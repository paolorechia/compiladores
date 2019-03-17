#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"

symbol_table * malloc_table(int table_max_size) {
  symbol_table * table = (symbol_table * ) malloc(sizeof(symbol_table));
  table->max_size = table_max_size;
  table->idx = -1;
  table->symbols = (symbol * ) malloc(sizeof(symbol) * table_max_size);
  return table;
}

void free_table(symbol_table * table) {
  free(table->symbols);
  free(table);
}

int insert_table(symbol_table * table, symbol new_symbol){
  table->idx++;
  symbol * saved_symbol = &table->symbols[table->idx];
  strcpy(saved_symbol->identifier, new_symbol.identifier);
  saved_symbol->category = new_symbol.category;
  switch(new_symbol.category) {
    case VARIABLE:
      saved_symbol->values.variable.lexical_level = new_symbol.values.variable.lexical_level;
      saved_symbol->values.variable.offset= new_symbol.values.variable.offset;
      saved_symbol->values.variable.variable_type= new_symbol.values.variable.variable_type;
      break;
    case PARAMETER:
      saved_symbol->values.parameter.lexical_level = new_symbol.values.parameter.lexical_level;
      saved_symbol->values.parameter.offset= new_symbol.values.parameter.offset;
      saved_symbol->values.parameter.variable_type = new_symbol.values.parameter.variable_type;
      saved_symbol->values.parameter.parameter_type= new_symbol.values.parameter.parameter_type;
      break;
  }
  return 0;
}

int remove_table(symbol_table * table){
  if (table->idx > -1) {
    table->idx--;
    return 0;
  }
  return -1;
}

int search_table(symbol_table * table, char id[TAM_TOKEN]){
  int idx = -1;
  int i = table->idx;
  while (i >= 0 && idx == -1) {
    if (strcmp(table->symbols[i].identifier, id) == 0) {
      idx = i;
    }
    i--;
  }
  return idx;
}

void print_table(symbol_table * table) {
  int i;
  char str_header[128];
  strcpy(str_header, "------------------------------------------------------------------------------------------------------------");
  printf("%s\n", str_header);
  for (i = table->idx; i >= 0; i--) {
      switch(table->symbols[i].category) {
        case VARIABLE:
          print_variable_symbol(table->symbols[i]);
          break;
        case PARAMETER:
          print_parameter_symbol(table->symbols[i]);
          break;
      }
  }
  printf("%s\n", str_header);
}

void print_variable_symbol(symbol s) {
  VariableType var_type = s.values.variable.variable_type;
  char var_type_str[32];
  switch(s.values.variable.variable_type) {
    case INTEGER:
      strcpy(var_type_str, "INTEGER");
      break;
    case BOOLEAN:
      strcpy(var_type_str, "BOOLEAN");
      break;
    case UNDEFINED:
      strcpy(var_type_str, "UNDEFINED");
      break;
  }
  printf("| %s  | VARIABLE | lexical_level: %d | offset: %d | type: %s |\n",
            s.identifier, s.values.variable.lexical_level, s.values.variable.offset, 
            var_type_str);

}

void print_parameter_symbol(symbol s) {
  VariableType var_type = s.values.parameter.variable_type;
  char var_type_str[32];
  char param_type_str[32];
  switch(s.values.parameter.variable_type) {
    case INTEGER:
      strcpy(var_type_str, "INTEGER");
      break;
    case BOOLEAN:
      strcpy(var_type_str, "BOOLEAN");
      break;
    case UNDEFINED:
      strcpy(var_type_str, "UNDEFINED");
      break;
  }
  switch(s.values.parameter.parameter_type) {
    case BYVAL:
      strcpy(param_type_str, "BYVAL");
      break;
    case BYREFERENCE:
      strcpy(param_type_str, "BYREF");
      break;
  }
  printf("| %s | PARAMETER | lexical_level: %d | offset: %d | type: %s | param: %s\n",
            s.identifier, s.values.parameter.lexical_level, s.values.parameter.offset, 
            var_type_str, param_type_str);
}

