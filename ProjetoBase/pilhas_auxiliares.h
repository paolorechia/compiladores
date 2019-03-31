#ifndef PILHAS_AUXILIARES_H

#define PILHAS_AUXILIARES_H
#include "tabela_simbolos.h"

#define MAX_STACK_SIZE 1024

typedef struct tvar_type_stack {
  VariableType A[MAX_STACK_SIZE];
  int idx;
} tvar_type_stack;

void init_type_stack(tvar_type_stack * type_stack);
VariableType push_type_stack(tvar_type_stack * type_stack, VariableType var_type);
VariableType pop_type_stack(tvar_type_stack * type_stack);
void print_type_stack(tvar_type_stack * type_stack);

#endif /* PILHAS_AUXILIARES_H */
