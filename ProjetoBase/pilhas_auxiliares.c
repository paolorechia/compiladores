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
  type_stack->idx--;
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

void init_symbol_stack(tsymbol_stack * symbol_stack) {
  symbol_stack->idx = 0;
}

int push_symbol_stack(tsymbol_stack * symbol_stack, symbol symbol) {
  symbol_stack->idx++;
  if (symbol_stack->idx > MAX_STACK_SIZE) {
    printf("Type stack is full, too bad! Something probably went wrong...\n");
    return -1;
  }
  symbol_stack->A[symbol_stack->idx] = symbol; 
  return 0;
}

symbol * pop_symbol_stack(tsymbol_stack * symbol_stack) {
  if (symbol_stack->idx <= 0) {
    printf("Type stack is empty!\n");
    return NULL;
  }
  symbol symbol = symbol_stack->A[symbol_stack->idx];
  symbol_stack->idx--;
  return &(symbol_stack->A[symbol_stack->idx + 1]);
}

void print_symbol_stack(tsymbol_stack * symbol_stack) {
  printf("NOT IMPLEMENTED!\n");
}

void init_label_stack(tlabel_stack * label_stack) {
  label_stack->idx = 0;
}
int push_label_stack(tlabel_stack * label_stack, char * label) {
  label_stack->idx++;
  if (label_stack->idx > MAX_STACK_SIZE) {
    printf("Type stack is full, too bad! Something probably went wrong...\n");
    return -1;
  }
  strncpy(label_stack->A[label_stack->idx], label, LABEL_MAX_SIZE);
  return 0;
}

char * pop_label_stack(tlabel_stack * label_stack) {
  if (label_stack->idx <= 0) {
    printf("Label stack is empty!\n");
    return (char *) NULL;
  }
  label_stack->idx--;
  return (char * )&(label_stack->A[label_stack->idx + 1]);
}

char * peek_label_stack(tlabel_stack * label_stack) {
  return (char * )&(label_stack->A[label_stack->idx]);
}

void print_label_stack(tlabel_stack * label_stack) {
  printf("NOT IMPLEMENTED\n");
}

int increment_label_counter(int count) {
  return ++count;
}

// Must pass a pointer to a buffer with minimum size (defined elsewhere)
void generate_label(int  * counter, char * dest_str) {
  char buffer[LABEL_MAX_SIZE];
  /*
  if (*counter < 10) {
    sprintf(buffer, "R00%d", *counter);
  } else 
  */
  if ( *counter < 100) {
    sprintf(buffer, "R0%d", *counter);
  }
  else {
    sprintf(buffer, "R%d", *counter);
  }
  strncpy(dest_str, buffer, LABEL_MAX_SIZE);
  *counter = increment_label_counter(*counter);
}

void init_istack(tint_stack * istack) {
  istack->idx = 0;
  istack->A[istack->idx] = 0;
}
int push_istack(tint_stack * istack, int new_val) {
  if (istack->idx > MAX_STACK_SIZE) {
    return -1;
  }
  istack->idx++;
  istack->A[istack->idx] = new_val;
  return istack->idx;
}
int peek_istack(tint_stack * istack) {
  return istack->A[istack->idx];
}
int pop_istack(tint_stack * istack) {
  if (istack->idx <= 0) {
    printf("Int stack is empty!\n");
    return -1;
  }
  istack->idx--;
  return istack->A[istack->idx + 1];
}
int update_top_istack(tint_stack * istack, int new_val) {
  istack->A[istack->idx]= new_val;
  return istack->idx;
}

void print_istack(tint_stack * istack) {
  printf("NOT IMPLEMENTED\n");
}
