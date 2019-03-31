#include "pilhas_auxiliares.h"
#include "stdio.h"


void init_type_stack(tvar_type_stack * type_stack) {
  type_stack->idx = 0;
}

VariableType push_type_stack(tvar_type_stack * type_stack, VariableType var_type) {
  type_stack->idx++;
  if (type_stack->idx > MAX_STACK_SIZE) {
    printf("Type stack is full, too bad! Something probably went wrong...\n");
    return -1;
  }
  type_stack->A[type_stack->idx] = var_type; 
  return var_type;
}

VariableType pop_type_stack(tvar_type_stack * type_stack) { 
  if (type_stack->idx <= 0) {
    printf("Type stack is empty!\n");
    return -1;
  }
  VariableType var_type = type_stack->A[type_stack->idx];
  type_stack->idx++;
  return var_type;
}

void print_type_stack(tvar_type_stack * type_stack) {
  int idx = type_stack->idx;
  while (idx >= 0 ) {
    VariableType var_type = type_stack->A[idx];
    switch(var_type) {
      case INTEGER:
        printf(" Integer \n");
        break;
      case BOOLEAN:
        printf(" Boolean \n");
        break;
      case UNDEFINED:
        printf(" Undefined \n");
        break;
      default:
        break;
    }
    idx--;
  }
}
