#include <stdio.h>
#include <stdlib.h>
#include "tabela_simbolos.h"

int main() {
  printf("SymbolTable test program\n");
  int table_max_size = 1024;
  Symbol * table;
  malloc_table(table, table_max_size);
  free_table(table);
}
