#ifndef PILHAS_AUXILIARES_H

#define PILHAS_AUXILIARES_H
#include "tabela_simbolos.h"
#include <string.h>

#define MAX_STACK_SIZE 1024
#define LABEL_MAX_SIZE 4
#define LABEL_DIGITS 3

/* VariableType Stack */
typedef struct tvar_type_stack {
  VariableType A[MAX_STACK_SIZE];
  int idx;
} tvar_type_stack;

void init_type_stack(tvar_type_stack * type_stack);
VariableType push_type_stack(tvar_type_stack * type_stack, VariableType var_type);
VariableType pop_type_stack(tvar_type_stack * type_stack);
VariableType peek_type_stack(tvar_type_stack * type_stack);
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

/* Label Stack */
typedef struct label_stack {
  char A[MAX_STACK_SIZE][LABEL_MAX_SIZE];
  int idx;
} tlabel_stack;

void init_label_stack(tlabel_stack * label_stack);
int push_label_stack(tlabel_stack * label_stack, char * label);
char * pop_label_stack(tlabel_stack * label_stack);
char * peek_label_stack(tlabel_stack * label_stack);
void print_label_stack(tlabel_stack * label_stack);

/* Label functions */
int increment_label_counter(int count);
void generate_label(int * counter, char * dest_str);

#endif /* PILHAS_AUXILIARES_H */
