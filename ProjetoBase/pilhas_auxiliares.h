#ifndef PILHAS_AUXILIARES_H

#define PILHAS_AUXILIARES_H
#include "tabela_simbolos.h"

#define MAX_STACK_SIZE 1024

/* VariableType Stack */
typedef struct tvar_type_stack {
  VariableType A[MAX_STACK_SIZE];
  int idx;
} tvar_type_stack;

void init_type_stack(tvar_type_stack * type_stack);
VariableType push_type_stack(tvar_type_stack * type_stack, VariableType var_type);
VariableType pop_type_stack(tvar_type_stack * type_stack);
void print_type_stack(tvar_type_stack * type_stack);

/* Symbol Stack */
typedef struct symbol_stack {
  symbol A[MAX_STACK_SIZE];
  int idx;
} tsymbol_stack;

void init_symbol_stack(tsymbol_stack * symbol_stack);
int push_symbol_stack(tsymbol_stack * symbol_stack, symbol symbol);
symbol * pop_symbol_stack(tsymbol_stack * symbol_stack);
void print_symbol_stack(tsymbol_stack * symbol_stack);

#endif /* PILHAS_AUXILIARES_H */
