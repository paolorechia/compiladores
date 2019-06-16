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


/* Caller must free returned node */
/* FIFO pop */
tnode * pop_first(thead * head) {
  tnode * node = head->node->nxt;
  if (node != NULL) {
    head->node->nxt = node->nxt;
    head->size--;
  }
  return node;
}

void l_insert(thead * head, char identifier[TAM_TOKEN], VariableType var_type, ParameterType param_type) {
    tnode * node = head->node;
    while (node->nxt != NULL){
        node = node -> nxt;
    }
    node->nxt=malloc(sizeof(tnode));
    strcpy(node->nxt->identifier, identifier);
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
        l_insert(destination, node->identifier, node->variable_type, node->parameter_type);
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

  printf("ID: %s; VariableType: %s; ParameterType: %s\n", node->identifier, type_str, param_str);
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
  /* TODO */
  free(table->symbols);
  free(table);
}

int insert_table(symbol_table * table, symbol new_symbol){
  table->idx++;
  symbol * saved_symbol = &table->symbols[table->idx];
  strncpy(saved_symbol->identifier, new_symbol.identifier, TAM_TOKEN);
  saved_symbol->category = new_symbol.category;
  switch(new_symbol.category) {
    case VARIABLE:
      saved_symbol->lexical_level = new_symbol.lexical_level;
      saved_symbol->values.variable.offset= new_symbol.values.variable.offset;
      saved_symbol->values.variable.variable_type= new_symbol.values.variable.variable_type;
      break;
    case PARAMETER:
      saved_symbol->lexical_level = new_symbol.lexical_level;
      saved_symbol->values.parameter.offset= new_symbol.values.parameter.offset;
      saved_symbol->values.parameter.variable_type = new_symbol.values.parameter.variable_type;
      saved_symbol->values.parameter.parameter_type= new_symbol.values.parameter.parameter_type;
      break;
    case PROCEDURE:
      saved_symbol->lexical_level = new_symbol.lexical_level;
      saved_symbol->values.procedure.label = new_symbol.values.procedure.label;
      saved_symbol->values.procedure.parameter_list = l_init(); 
      l_copy(new_symbol.values.procedure.parameter_list, saved_symbol->values.procedure.parameter_list);
      break;
    case FUNCTION:
    /*TODO: check if this part is OK */
      saved_symbol->lexical_level = new_symbol.lexical_level;
      saved_symbol->values.function.label = new_symbol.values.function.label;
      saved_symbol->values.function.variable_type = UNDEFINED;
      saved_symbol->values.function.offset = -1;
      saved_symbol->values.function.parameter_list = l_init(); 
      l_copy(new_symbol.values.function.parameter_list, saved_symbol->values.function.parameter_list);
      break;
    case LABEL_SYMBOL_TYPE:
      saved_symbol->lexical_level = new_symbol.lexical_level;
      saved_symbol->values.label.label = new_symbol.values.label.label;
      break;
    default:
      printf("Invalid category\n");
  }
  return 0;
}


// fix list memory here (08062019: check if fix worked)
int remove_table(symbol_table * table, int number_to_remove){
  if (table->idx <= -1 || number_to_remove <= 0 || number_to_remove > table->idx + 1) {
    return -1; 
  }
  int i; 
  int idx = table->idx;
  for (i = idx; i >= (table->idx - number_to_remove); i--) {
    if (table->symbols[i].category == PROCEDURE) {
      l_free(table->symbols[i].values.procedure.parameter_list);
    }
  }
  table->idx -= number_to_remove;
  return 0;
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

int local_search_table(symbol_table * table, char id[TAM_TOKEN], int lexical_level){
  int idx = -1;
  int i = table->idx;
  while (i >= 0 && idx == -1) {
    if (strcmp(table->symbols[i].identifier, id) == 0 && 
       table->symbols[i].lexical_level == lexical_level) {
      idx = i;
    }
    i--;
  }
  return idx;
}

symbol * peek_table(symbol_table * table) {
  return &(table->symbols[table->idx]);
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
        case LABEL_SYMBOL_TYPE:
          print_label_symbol(table->symbols[i]);
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
            s.identifier, s.lexical_level, s.values.variable.offset, 
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
            s.identifier, s.lexical_level, s.values.parameter.offset, 
            var_type_str, param_type_str);
}

void label_to_string(int counter, char * dest_str) {
  char buffer[LABEL_MAX_SIZE];
  if ( counter < 10 ) {
    sprintf(buffer, "R0%d", counter);
  }  else {
    sprintf(buffer, "R%d", counter);
  }
  strncpy(dest_str, buffer, LABEL_MAX_SIZE);
}

int label_to_integer(char * input_str) {
  char buffer[LABEL_MAX_SIZE];
  char * pter = (char *) &buffer;
  while (*input_str != '\0') {
    if (*input_str != 'R') {
      *pter=*input_str;
      pter++;
    }
    input_str++;
  }
  *pter = '\0';
  int number = atoi(buffer);
  return number;
}

void variable_type_to_string(VariableType var_type, char * var_type_str) {
  switch(var_type) {
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
}

void category_type_to_string(CategoryType cat_type, char * output_str) {
  switch(cat_type) {
    case VARIABLE:
      strcpy(output_str, "VARIABLE");
      break;
    case FUNCTION:
      strcpy(output_str, "FUNCTION");
      break;
    case PROCEDURE:
      strcpy(output_str, "PROCEDURE");
      break;
    case PARAMETER:
      strcpy(output_str, "PARAMETER");
      break;
  }
}

void print_procedure_symbol(symbol s) {
  thead * parameter_list = s.values.procedure.parameter_list;
  char params_string[2048];
  strcpy(params_string, "No Parameters");
  if (parameter_list->size > 0) {
    char var_type_str[32];
    char param_type_str[32];
    char current_param[96];
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
  }
  char label_string[LABEL_MAX_SIZE];
  label_to_string(s.values.procedure.label, (char * ) &label_string);
  printf("| %s | PROCEDURE | lexical_level: %d | label: %s | params: %s\n",
            s.identifier, s.lexical_level, label_string,
            params_string);
}

void print_function_symbol(symbol s) {

  thead * parameter_list = s.values.function.parameter_list;
  char return_type_str[32];
  
  switch(s.values.function.variable_type) {
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
  char params_string[2048];
  strcpy(params_string, "No Parameters");
  if (parameter_list->size > 0) {
    char var_type_str[32];
    char param_type_str[32];
    char current_param[96];
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
  }
  char label_string[LABEL_MAX_SIZE];
  label_to_string(s.values.function.label, (char * ) &label_string);
  printf("| %s | FUNCTION | lexical_level: %d | label: %s | var_type: %s | return_offset: %d | params: %s\n",
            s.identifier, s.lexical_level, label_string,
            return_type_str, s.values.function.offset, params_string);
}

void print_label_symbol(symbol s) {
  char label_string[LABEL_MAX_SIZE];
  label_to_string(s.values.label.label, (char * ) &label_string);
  printf("| %s | LABEL_SYMBOL_TYPE| lexical_level: %d | label: %s\n",
            s.identifier, s.lexical_level, label_string);
}


int insert_variable(symbol_table * table, char * identifier, int lexical_level, int offset) {
  symbol new_symbol;
  new_symbol.category = VARIABLE;
  strcpy(new_symbol.identifier, identifier);
  new_symbol.lexical_level = lexical_level;
  new_symbol.values.variable.offset = offset;
  new_symbol.values.variable.variable_type = UNDEFINED;
  insert_table(table, new_symbol);
  offset++;
  return offset;
}

void insert_procedure(symbol_table * table, char * ident_token, int lexical_level, char * label, thead * parameter_list) {
  symbol new_symbol;
  new_symbol.category = PROCEDURE;
  strcpy(new_symbol.identifier, ident_token);
  new_symbol.lexical_level = lexical_level;
  new_symbol.values.procedure.label = label_to_integer(label);
  new_symbol.values.procedure.parameter_list = l_init();
  l_copy(parameter_list, new_symbol.values.procedure.parameter_list);
  l_free(parameter_list);
  insert_table(table, new_symbol);
}

VariableType insert_parameter(symbol_table * table, char * ident_token, int lexical_level, char *  var_type_str, ParameterType param_type) {
  VariableType var_type = parse_var_type(var_type_str);
  symbol new_symbol;
  strcpy(new_symbol.identifier, ident_token);
  new_symbol.category = PARAMETER;
  new_symbol.lexical_level= lexical_level;
  new_symbol.values.parameter.offset= 0;
  new_symbol.values.parameter.variable_type = var_type;
  new_symbol.values.parameter.parameter_type = param_type;
  insert_table(table, new_symbol);
  return var_type;
}

int update_subroutine_parameters(symbol_table * table) {
  int current_offset = -4;
  int idx = table->idx;
  symbol * current_symbol;
  current_symbol = &(table->symbols[idx]);
  int updated_vars = 0;
  while (idx >= 0 && current_symbol->category == PARAMETER) {
    current_symbol->values.parameter.offset = current_offset;
    updated_vars++; 
    current_offset--;
    idx--;
    current_symbol = &(table->symbols[idx]);
  }
  return updated_vars;
}

int copy_parameters_to_table(symbol_table * table) {
  int idx = table->idx;
  symbol * current_symbol = &(table->symbols[idx]);
  thead * params = l_init();
  if (current_symbol->category == PROCEDURE) {
    l_copy(current_symbol->values.procedure.parameter_list, params);
  } else if (current_symbol->category == FUNCTION) {
    l_copy(current_symbol->values.function.parameter_list, params);
  } else {
    fprintf(stderr, "ERROR: Not a procedure or a function!\n");
    return -1;
  }
  tnode * list_node;
  char var_type_str[TAM_TOKEN];
  int updated_vars = 0;
  while ( (list_node = pop_first(params)) != NULL)  {
    variable_type_to_string(list_node->variable_type, var_type_str);
    insert_parameter(
      table,
      list_node->identifier,
      current_symbol->lexical_level,
      var_type_str,
      list_node->parameter_type
    );
    updated_vars++;
  }
  l_free(params);
  return updated_vars;
}

int copy_parameters_to_table_from_id(symbol_table * table, char * identifier) {
  int idx = table->idx;
  symbol * current_symbol = find_identifier(table, identifier);
  printf("%s\n", current_symbol->identifier);
  thead * params = l_init();
  if (current_symbol->category == PROCEDURE) {
    l_copy(current_symbol->values.procedure.parameter_list, params);
  } else if (current_symbol->category == FUNCTION) {
    l_copy(current_symbol->values.function.parameter_list, params);
  } else {
    fprintf(stderr, "ERROR: Not a procedure or a function!\n");
    return -1;
  }
  tnode * list_node;
  char var_type_str[TAM_TOKEN];
  int updated_vars = 0;
  while ( (list_node = pop_first(params)) != NULL)  {
    variable_type_to_string(list_node->variable_type, var_type_str);
    insert_parameter(
      table,
      list_node->identifier,
      current_symbol->lexical_level,
      var_type_str,
      list_node->parameter_type
    );
    updated_vars++;
  }
  l_free(params);
  return updated_vars;
}

int remove_parameters(symbol_table * table) {
  int idx = table->idx;
  symbol * current_symbol;
  current_symbol = &(table->symbols[idx]);
  int removed_parameters = 0;
  while (idx >= 0 && current_symbol->category == PARAMETER) {
    removed_parameters++; 
    idx--;
    current_symbol = &(table->symbols[idx]);
  }
  table->idx = idx;
  return removed_parameters;
}

int remove_nested_subroutines(symbol_table * table, int current_lexical_level) {
  int idx = table->idx;
  symbol * current_symbol;
  current_symbol = &(table->symbols[idx]);
  int removed_subroutines = 0;
  while (idx >= 0 &&
            (current_symbol->category == PROCEDURE ||
             current_symbol->category == LABEL_SYMBOL_TYPE ||
             current_symbol->category == PARAMETER ||
             current_symbol->category == FUNCTION) &&
         current_symbol->lexical_level > current_lexical_level) {
    removed_subroutines++; 
    idx--;
    current_symbol = &(table->symbols[idx]);
  }
  table->idx = idx;
  return removed_subroutines;
}

void insert_function(symbol_table * table, char * ident_token, int lexical_level, char * label) {
  symbol new_symbol;
  new_symbol.category = FUNCTION;
  strcpy(new_symbol.identifier, ident_token);
  new_symbol.lexical_level = (int8_t) lexical_level;
  new_symbol.values.function.label =  (int8_t) label_to_integer(label);
  new_symbol.values.function.parameter_list = l_init();
  insert_table(table, new_symbol);
}

int update_function_return_type(symbol_table * table, char * return_type_token) {
  VariableType var_type = parse_var_type(return_type_token);
  char var_type_str[255];
  variable_type_to_string(var_type, var_type_str);
  if (var_type == -1) return -1;
  int idx = -1;
  int i = table->idx;
  while (i >= 0 && idx == -1) {
    if (table->symbols[i].category == FUNCTION) {
      idx = i;
    }
    i--;
  }
  if (idx == -1) { 
    fprintf(stderr, "ERROR: function symbol not found\n");
    return -1;
  }
  symbol * function_symb = &(table->symbols[idx]);
  /* update function symbol variable with return type, offset and all that */
  function_symb->values.function.offset = -(l_size(function_symb->values.function.parameter_list) + 4);
  function_symb->values.function.variable_type = var_type;
}

void insert_label(symbol_table * table, char * ident_token, int lexical_level, char * label) {
  symbol new_symbol;
  new_symbol.category = LABEL_SYMBOL_TYPE;
  strcpy(new_symbol.identifier, ident_token);
  new_symbol.lexical_level = (int32_t) lexical_level;
  new_symbol.values.label.label =  (int32_t) label_to_integer(label);
  insert_table(table, new_symbol);
}


int parse_var_type(char * token) {
  char parsed_token[TAM_TOKEN];
  char * pter = parsed_token;
  char * ch = token;
  while (*ch) {
    if (*ch >= 97 && *ch <= 122) {
      *ch = *ch - 32;
    }
    if (*ch < 65 || *ch > 90) {
      fprintf(stderr, "ERROR: Failed to parse variable type from token: %s at char %c!!!\n", token, *ch);
      printf("%d\n", *ch);
      return -1;
    }
    *pter = *ch;
    pter++;
    ch++;
  }
  *pter = '\0';
  char * integer = "INTEGER";
  char * boolean = "BOOLEAN";
  int test;
  if ((test = strcmp(parsed_token, integer)) == 0) {
    return INTEGER;
  }
  if ((test = strcmp(parsed_token, boolean)) == 0) {
    return BOOLEAN;
  }
  fprintf(stderr, "ERROR: Invalid variable type: '%s'. Must be either '%s' or '%s'\n", token, integer, boolean);
  return -1;
}

int update_var_type(symbol_table * table, char * token) {
  VariableType var_type = (VariableType) parse_var_type(token);
  if (var_type == - 1) {
    return -1;
  }
  int updated_variables = 0;
  int idx = table->idx;
  symbol * current_symbol; 
  current_symbol = &(table->symbols[idx]);
  while (idx >= 0 && current_symbol->category == VARIABLE && current_symbol->values.variable.variable_type == UNDEFINED) {
    current_symbol->values.variable.variable_type = var_type;
    idx--;
    updated_variables++;
    current_symbol = &(table->symbols[idx]);
  }
  return updated_variables;
}

int remove_local_vars(symbol_table * table) {
  int idx = table->idx;
  symbol * current_symbol;
  current_symbol = &(table->symbols[idx]);
  int removed_vars = 0;
  while (idx >= 0 && current_symbol->category == VARIABLE) {
    removed_vars++; 
    idx--;
    current_symbol = &(table->symbols[idx]);
  }
  table->idx = idx;
  return removed_vars;
}

// Finds first identifier regardless of lexical level
symbol * find_identifier(symbol_table * table, char * identifier) {
  int idx = search_table(table, identifier);
  if (idx == -1) {
    fprintf(stderr, "ERROR: Symbol %s could not be found! Double check if it has been declared!\n", identifier);
    return NULL;
  }
  return &(table->symbols[idx]);
}

// Attempts to find identifier at given lexical level
symbol * find_local_identifier(symbol_table * table, char * identifier, int lexical_level) {
  int idx = local_search_table(table, identifier, lexical_level);
  if (idx == -1) {
    return NULL;
  }
  return &(table->symbols[idx]);
}


int assemble_read_write_instruction(char * temp_str, const char * instruction, symbol * symb_pter) {
  switch(symb_pter->category) {
      case VARIABLE:
        sprintf(temp_str, "%s %d, %d", instruction,
                                       symb_pter->lexical_level,
                                       symb_pter->values.variable.offset);
      break;
      case PARAMETER:
        sprintf(temp_str, "%s %d, %d", instruction,
                                       symb_pter->lexical_level,
                                       symb_pter->values.parameter.offset);
      break;
      case FUNCTION:
        sprintf(temp_str, "%s %d, %d", instruction,
                                       symb_pter->lexical_level,
                                       symb_pter->values.function.offset);
      break;
      default:
        ;
        char cat_str[255];
        category_type_to_string(symb_pter->category, cat_str);
        fprintf(stderr, "ERROR: Invalid Category: %s!\n", cat_str);
        return -1;
    }
    return 0;
}

int check_symbol_category(symbol * symb_pter, CategoryType cat_type, CategoryType cat_type2) {
      if (symb_pter == NULL) {
        fprintf(stderr, "ERROR: unexpected NULL pointer!. Aborting...\n");
        return -1;
      }
      if (symb_pter->category != cat_type && symb_pter->category != cat_type2) {
        char category[255];
        char expected_category[255];
        category_type_to_string(symb_pter->category, (char *) &category);
        category_type_to_string(cat_type, (char *) &expected_category);
        fprintf(stderr, "ERROR: Symbol %s is not a %s! Declared as: %s\n", symb_pter->identifier, expected_category, category);
        return -1;
      }
      return 0;
}
