#include "pilhas_auxiliares.h"
#include "stdio.h"


/* Var type stack */
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

/* Symbol stack */
void init_symbol_stack(tsymbol_stack * symbol_stack) {
  symbol_stack->idx = 0;
}

void reset_symbol_stack_queue_idx(tsymbol_stack * symbol_stack) {
  symbol_stack->queue_start_idx = 1;
}

int push_symbol_stack(tsymbol_stack * symbol_stack, symbol symbol) {
  symbol_stack->idx++;
  if (symbol_stack->idx > MAX_STACK_SIZE) {
    printf("Symbol stack is full, too bad! Something probably went wrong...\n");
    return -1;
  }
  symbol_stack->A[symbol_stack->idx] = symbol; 
  return 0;
}

int queue_symbol_stack(tsymbol_stack * symbol_stack, symbol symbol) {
  symbol_stack->idx++;
  if (symbol_stack->idx > MAX_STACK_SIZE) {
    printf("Symbol stack is full, too bad! Something probably went wrong...\n");
    return -1;
  }
  int idx = symbol_stack->idx;
  int prev_i = idx - 1;
  while (prev_i > 0) {
    symbol_stack->A[idx] = symbol_stack->A[prev_i];
    idx--;
    prev_i--; 
  } 
  symbol_stack->A[1] = symbol; 
  return 0;
}

symbol unqueue_symbol_stack(tsymbol_stack * symbol_stack) {
  symbol symbol_to_return;
  symbol_to_return.values.parameter.lexical_level = -1;
  if (symbol_stack->queue_start_idx > symbol_stack->idx) {
    return symbol_to_return;
  } 
  symbol * first_symbol = &(symbol_stack->A[symbol_stack->queue_start_idx]);
  symbol_stack->queue_start_idx++;
  strcpy(symbol_to_return.identifier, first_symbol->identifier);
  symbol_to_return.category = first_symbol->category;
  symbol_to_return.values.parameter.variable_type = first_symbol->values.parameter.variable_type;
  symbol_to_return.values.parameter.parameter_type = first_symbol->values.parameter.parameter_type;
  
  int prev_i = 1;
  int idx = prev_i + 1;
  while (idx <= symbol_stack->idx) {
    symbol_stack->A[prev_i] = symbol_stack->A[idx];
    idx++;
    prev_i++; 
  } 
  symbol_stack->idx--;
  return symbol_to_return;
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

/* Label stack */
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
  int idx = label_stack->idx;
  while (idx >= 0) {
    printf("%s\n", label_stack->A[idx]);
    idx--;
  }
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

/* Integer stack */
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

