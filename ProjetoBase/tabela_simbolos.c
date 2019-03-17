#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"


/* Linked List functions */
thead * l_init(){
    thead * head = malloc(sizeof(thead));
    head->node = malloc(sizeof(tnode));
    head->node->nxt = NULL;
    head->size = 0;
    return head;
}
void l_insert(thead * head, VariableType var_type, ParameterType param_type) {
    tnode * node = head->node;
    while (node->nxt != NULL){
        node = node -> nxt;
    }
    node->nxt=malloc(sizeof(tnode));
    node->nxt->variable_type = var_type;
    node->nxt->parameter_type = param_type;
    node->nxt->nxt=NULL;
    head->size += 1;
    return;
}

// destination must be initialized and empty before calling l_copy
int l_copy(thead * origin, thead * destination) {
    if (origin->node->nxt == NULL || destination->size > 0){
        return -1;
    }
    tnode * node = origin->node;
    while (node->nxt != NULL){
        node = node->nxt;
        l_insert(destination, node->variable_type, node->parameter_type);
    }
    destination->size = origin->size;
    return 0;
}

void l_print(thead * head){
    if (head->node->nxt == NULL){
        printf("Empty l\n");
        return;
    }
    tnode * node = head->node->nxt;
    while (node){
        print_node(node);
        node = node->nxt;
    }
}


void print_node(tnode * node) {
  char type_str[32];
  char param_str[32];
  switch(node->variable_type) {
    case INTEGER:
      strcpy(type_str, "INTEGER");
      break;
    case BOOLEAN:
      strcpy(type_str, "BOOLEAN");
      break;
    case UNDEFINED:
      strcpy(type_str, "UNDEFINED");
      break;
  }
  switch(node->parameter_type) {
    case BYVAL:
      strcpy(param_str, "BYVAL");
      break;
    case BYREFERENCE:
      strcpy(param_str, "BYREFERENCE");
      break;
  }

  printf("VariableType: %s; ParameterType: %s\n", type_str, param_str);
}


int l_size(thead * head){
    return head->size;
}

/* Recursive node free function */
int rec_clear(tnode * node){
    if (node->nxt != NULL){
        rec_clear(node->nxt);
    }
//    printf("Freeing node of name: %s\n", node->key);
    node->nxt = NULL;
    free(node);
}
int l_clear(thead * head){
    tnode * node = head->node;
    if (head->node->nxt == NULL){
        return 0;
    }
    rec_clear(head->node->nxt);
    head->node->nxt = NULL;
    head->size = 0;
}
void l_free(thead *head){
    l_clear(head);
    free(head->node);
    free(head);
}

/* Symbol Table functions */
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
    case PROCEDURE:
      saved_symbol->values.procedure.lexical_level = new_symbol.values.procedure.lexical_level;
      saved_symbol->values.procedure.label = new_symbol.values.procedure.label;
      saved_symbol->values.procedure.parameter_list = l_init(); 
      l_copy(new_symbol.values.procedure.parameter_list, saved_symbol->values.procedure.parameter_list);
    case FUNCTION:
      saved_symbol->values.function.lexical_level = new_symbol.values.function.lexical_level;
      saved_symbol->values.function.label = new_symbol.values.function.label;
      saved_symbol->values.function.return_type = new_symbol.values.function.return_type;
      saved_symbol->values.function.parameter_list = l_init(); 
      l_copy(new_symbol.values.function.parameter_list, saved_symbol->values.function.parameter_list);
  }
  return 0;
}


// fix list memory list here
int remove_table(symbol_table * table, int number_to_remove){
  if (number_to_remove <= 0 || number_to_remove > table->idx + 1) {
    return -2; 
  }
  int i; 
  int idx = table->idx;
  for (i = idx; i >= (table->idx - number_to_remove); i--) {
    if (table->symbols[i].category == PROCEDURE) {
      l_free(table->symbols[i].values.procedure.parameter_list);
    }
  }
  if (table->idx > -1) {
    table->idx -= number_to_remove;
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
        case PROCEDURE:
          print_procedure_symbol(table->symbols[i]);
          break;
        case FUNCTION:
          print_function_symbol(table->symbols[i]);
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

void print_procedure_symbol(symbol s) {
  thead * parameter_list = s.values.procedure.parameter_list;
  if (parameter_list->size > 0) {
    char var_type_str[32];
    char param_type_str[32];
    char current_param[96];
    char params_string[2048];
    strcpy(params_string, "( ");
    tnode * node = parameter_list->node;
    while (node->nxt != NULL){
      node = node->nxt;
      switch(node->variable_type) {
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
      switch(node->parameter_type) {
        case BYVAL:
          strcpy(param_type_str, "BYVAL");
          break;
        case BYREFERENCE:
          strcpy(param_type_str, "BYREF");
          break;
      }
      sprintf(current_param, "VarType: %s, ParamType: %s; ", var_type_str, param_type_str);
      strcat(params_string, current_param);
    }
    strcat(params_string, ")");
    printf("| %s | PROCEDURE | lexical_level: %d | label: %d | params: %s\n",
              s.identifier, s.values.procedure.lexical_level, s.values.procedure.label, 
              params_string);
    }
}
void print_function_symbol(symbol s) {
  thead * parameter_list = s.values.function.parameter_list;
  char return_type_str[32];
  switch(s.values.function.return_type) {
    case INTEGER:
      strcpy(return_type_str, "INTEGER");
      break;
    case BOOLEAN:
      strcpy(return_type_str, "BOOLEAN");
      break;
    case UNDEFINED:
      strcpy(return_type_str, "UNDEFINED");
      break;
  }
  if (parameter_list->size > 0) {
    char var_type_str[32];
    char param_type_str[32];
    char current_param[96];
    char params_string[2048];
    strcpy(params_string, "( ");
    tnode * node = parameter_list->node;
    while (node->nxt != NULL){
      node = node->nxt;
      switch(node->variable_type) {
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
      switch(node->parameter_type) {
        case BYVAL:
          strcpy(param_type_str, "BYVAL");
          break;
        case BYREFERENCE:
          strcpy(param_type_str, "BYREF");
          break;
      }
      sprintf(current_param, "VarType: %s, ParamType: %s; ", var_type_str, param_type_str);
      strcat(params_string, current_param);
    }
    strcat(params_string, ")");
    printf("| %s | FUNCTION | lexical_level: %d | label: %d | return_type: %s | params: %s\n",
              s.identifier, s.values.function.lexical_level, s.values.function.label, 
              return_type_str, params_string);
    }
}

