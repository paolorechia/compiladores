#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabela_simbolos.h"

int main() {
  printf("symbolTable test program\n");
  int table_max_size = 1024;
  symbol_table * table = malloc_table(table_max_size);
  symbol new_symbol;
  strcpy(new_symbol.identifier, "Teste");
  new_symbol.category = VARIABLE;
  new_symbol.values.variable.lexical_level = 1;
  new_symbol.values.variable.offset= 2;
  new_symbol.values.variable.variable_type = UNDEFINED;
  insert_table(table, new_symbol);
  print_table(table);
  free_table(table);
}
