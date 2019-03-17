#include <stdio.h>
#include <stdlib.h>
#include "tabela_simbolos.h"

int insert_table(Symbol * table, Symbol new_symbol){
  return 0;
}

int remove_table(Symbol * table, int new_symbol){
  return 0;
}

int search_table(Symbol * table, char id[TAM_TOKEN]){
  return 0;
}

int malloc_table(Symbol * table, int table_max_size) {
  table = (Symbol * ) malloc(sizeof(Symbol) * table_max_size);
  return 0;
}

void free_table(Symbol * table) {
  free(table);
}
