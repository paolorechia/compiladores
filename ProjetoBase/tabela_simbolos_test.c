#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tabela_simbolos.h"

int main() {
  printf("symbolTable test program\n");
  int table_max_size = 1024;
  symbol_table * table = malloc_table(table_max_size);
  symbol new_symbol;
  strcpy(new_symbol.identifier, "Teste");

  new_symbol.category = VARIABLE;
  new_symbol.values.variable.lexical_level = 0;
  new_symbol.values.variable.offset= 0;
  new_symbol.values.variable.variable_type = UNDEFINED;
  insert_table(table, new_symbol);

  new_symbol.values.variable.offset = 1;
  strcpy(new_symbol.identifier, "var2");
  insert_table(table, new_symbol);

  new_symbol.category = PARAMETER;
  strcpy(new_symbol.identifier, "parametro_test");
  new_symbol.values.parameter.lexical_level= 0;
  new_symbol.values.parameter.offset= 2;
  new_symbol.values.parameter.variable_type = UNDEFINED;
  new_symbol.values.parameter.parameter_type = BYVAL;
  insert_table(table, new_symbol);

  new_symbol.category = PROCEDURE;
  strcpy(new_symbol.identifier, "meu_procedure");
  new_symbol.values.procedure.lexical_level = 1;
  new_symbol.values.procedure.label = 1;
  new_symbol.values.procedure.parameter_list = l_init();
  l_insert(new_symbol.values.procedure.parameter_list, INTEGER, BYVAL);
  l_insert(new_symbol.values.procedure.parameter_list, BOOLEAN, BYVAL);
  l_insert(new_symbol.values.procedure.parameter_list, INTEGER, BYREFERENCE);

  insert_table(table, new_symbol);
  l_free(new_symbol.values.procedure.parameter_list);

  new_symbol.category = FUNCTION;
  strcpy(new_symbol.identifier, "minha_funcao");
  new_symbol.values.function.lexical_level = 1;
  new_symbol.values.function.label = 2;
  new_symbol.values.function.return_type = INTEGER;
  new_symbol.values.function.parameter_list = l_init();
  l_insert(new_symbol.values.procedure.parameter_list, INTEGER, BYREFERENCE);
  l_insert(new_symbol.values.procedure.parameter_list, INTEGER, BYVAL);
  insert_table(table, new_symbol);

  print_table(table);
  
  printf("Testing search_table...\n");
  assert(search_table(table, "Teste") == 0);
  assert(search_table(table, "var2") == 1);
  assert(search_table(table, "parametro_test") == 2);
  assert(search_table(table, "undefined_var") == -1);

  printf("Testing conversion...\n");
  int label_test = new_symbol.values.function.label;
  char dest_str[LABEL_MAX_SIZE];
  label_to_string(label_test, (char *) &dest_str);
  int label_converted = label_to_integer(dest_str);
  printf("Label was... %d, then %s and finally back to: %d\n", label_test, dest_str, label_converted);

  remove_table(table, 2);
  print_table(table);
  free_table(table);
}
