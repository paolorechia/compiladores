// Testar se funciona corretamente o empilhamento de par�metros
// passados por valor ou por refer�ncia.

%define parse.error verbose

%{

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "tabela_simbolos.h"
#include "pilhas_auxiliares.h"


int declaring_subroutine = 0;
int malloced_local_vars = 0;
int removed_local_vars = 0;
int param_num = 0;
int lexical_level = 0;
int offset = 0;
int label_counter = 0;

char label[LABEL_MAX_SIZE];
char * label_pter;
char * label_pter2;
char last_procedure_identifier[TAM_TOKEN];
char last_identifier[TAM_TOKEN];
char last_variable_identifier[TAM_TOKEN];
char last_instruction[TAM_TOKEN];
char temp_str[TAM_TOKEN];

thead * declaring_param_list;
thead * last_param_list;
thead * caller_param_list;
tnode * list_node; 
ParameterType current_param_type = NULL_PARAM;

symbol new_symbol;
symbol * symb_pter;
symbol_table * table;
tvar_type_stack var_type_stack;
tsymbol_stack symbol_stack;
tlabel_stack label_stack;
tint_stack offset_stack;


VariableType t1;
VariableType t2;

extern char * yytext;

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO 
%token T_BEGIN T_END VAR IDENT DOIS_PONTOS_IGUAL
%token MENOR MAIOR IGUAL
%token PROCEDURE_TOKEN FUNCTION_TOKEN
%token IF WHILE DO THEN ELSE NUMERO
%token MAIS MENOS ASTERICO DIV BARRA
%token AND OR TRUE FALSE
%token READ WRITE
%token LABEL GOTO
%token FORWARD

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE 

%%

programa    :{ 
               geraCodigo (NULL, "INPP"); 
             }
             PROGRAM IDENT 
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
//             print_table(table);
               geraCodigo (NULL, "PARA"); 
             }
;

bloco       : 
              declara_rotulos parte_declara_vars {
                generate_label(&label_counter, (char *)label);
                sprintf(temp_str, "DSVS %s", label);
                geraCodigo(NULL, temp_str);
                push_label_stack(&label_stack, label);
              } 
              bloco_subrotina
              { 
                label_pter = pop_label_stack(&label_stack); 
//                print_label_stack(&label_stack);
                geraCodigo(label_pter, "NADA");
                free(label_pter);
              }
              comando_composto {
                /*
                printf("Leaving block... before cleaning up...\n");
                printf("Lexical level: %d\n", lexical_level);
                print_table(table);
                */
                remove_nested_subroutines(table, lexical_level);
                removed_local_vars = remove_local_vars(table);
                if (removed_local_vars > 0) {
                  sprintf(temp_str, "DMEM %d", removed_local_vars);
                  geraCodigo (NULL, temp_str);
                };
                remove_parameters(table);
                /*
                printf("Leaving block... after cleaning up...\n");
                print_table(table);
                */
              }
;

declara_rotulos: LABEL lista_rotulos PONTO_E_VIRGULA |
;

lista_rotulos: lista_rotulos VIRGULA declara_rotulo | declara_rotulo
; 

declara_rotulo: NUMERO { 
      generate_label(&label_counter, (char * )label);
      insert_label(table, token, lexical_level, label);
   }
;


bloco_subrotina: lista_subrotinas |
;

lista_subrotinas: lista_subrotinas declara_subrotina | declara_subrotina 
;

declara_subrotina: declara_cabecalho_procedure FORWARD PONTO_E_VIRGULA {
                      lexical_level++;
                      generate_label(&label_counter, (char * )label);
                      insert_procedure(table, last_procedure_identifier, lexical_level, label, declaring_param_list);
                      lexical_level--;
                   } |
                   declara_cabecalho_procedure { 
                      lexical_level++;
                      symb_pter = find_local_identifier(table, last_procedure_identifier, lexical_level);
                      // Se encontrou, procedure ja foi declarado em forward
                      if (symb_pter != NULL) {
                        label_to_string(symb_pter->lexical_level, label);
                        declaring_param_list = symb_pter->values.procedure.parameter_list;
                      // Senao, criar
                      } else {
                        generate_label(&label_counter, (char * )label);
                        insert_procedure(table, last_procedure_identifier, lexical_level, label, declaring_param_list);
                      }
                      // print_table(table);
                      // Entrada em bloco do procedimento
                      copy_parameters_to_table_from_id(table, last_procedure_identifier);
                      update_subroutine_parameters(table);
                      // print_table(table);
                      sprintf(temp_str, "ENPR %d", lexical_level);
                      geraCodigo(label, temp_str);
                   } declara_procedure_bloco |
                   declara_funcao
                   
;

declara_cabecalho_procedure: PROCEDURE_TOKEN IDENT {
                          strncpy(last_procedure_identifier, token, TAM_TOKEN);
                          declaring_param_list = l_init();
                          push_istack(&offset_stack, 0);
                          param_num = 0; 
                        } lp PONTO_E_VIRGULA;

declara_procedure_bloco: bloco {
                        // TODO insert subroutine into table here
                        sprintf(temp_str, "RTPR %d, %d", lexical_level, param_num);
                        geraCodigo(NULL, temp_str);
                        lexical_level--;
                        pop_istack(&offset_stack);
//                        print_table(table);
                      }
                     PONTO_E_VIRGULA
;


declara_funcao: FUNCTION_TOKEN IDENT {
                        // print_table(table);
                        lexical_level++;
                        generate_label(&label_counter, (char * )label);
                        sprintf(temp_str, "ENPR %d", lexical_level);
                        geraCodigo(label, temp_str);
                        insert_function(table, token, lexical_level, label);
                        declaring_param_list = peek_table(table)->values.function.parameter_list;
                        push_istack(&offset_stack, 0);
                        param_num = 0; 
                        // print_table(table);
                      }
                      lp
                      DOIS_PONTOS IDENT {
                        copy_parameters_to_table(table);
                        update_subroutine_parameters(table);
                        /* Acrescentar retorno de funcao na tabela simbolos */
                        if (update_function_return_type(table, token) == -1) return -1;
                      } PONTO_E_VIRGULA 
                      bloco {
//                        print_table(table);
                        // TODO: verificar se param_num funciona corretamente 
                        sprintf(temp_str, "RTPR %d, %d", lexical_level, param_num);
                        geraCodigo(NULL, temp_str);
                        lexical_level--;
                        pop_istack(&offset_stack);
                        // print_table(table);
                      }
                     PONTO_E_VIRGULA
;

lp: ABRE_PARENTESES  lista_parametros FECHA_PARENTESES | 
    ABRE_PARENTESES FECHA_PARENTESES |
;

lista_parametros:  lista_parametros pv_ou_v parametro { param_num++;} |
                  parametro { param_num++; }

pv_ou_v: VIRGULA | PONTO_E_VIRGULA;

parametro: IDENT {  strcpy(last_identifier, token); } DOIS_PONTOS tipo {
            VariableType var_type = parse_var_type(token);
            l_insert(declaring_param_list, last_identifier, var_type, BYVAL);
           } 
           | VAR IDENT  { strcpy(last_identifier, token); } DOIS_PONTOS tipo {
            VariableType var_type = parse_var_type(token);
            l_insert(declaring_param_list, last_identifier, var_type, BYREFERENCE);
           }

parte_declara_vars:  var 
;


var         : { } VAR declara_vars { 
//                    print_table(table); 
                    } 
            |
;

declara_vars: declara_vars declara_var 
            | declara_var 
;

declara_var : { } 
              lista_id_var DOIS_PONTOS 
              tipo 
              { /* AMEM */
                malloced_local_vars = update_var_type(table, token);
                sprintf(temp_str, "AMEM %d", malloced_local_vars);
                geraCodigo (NULL, temp_str); 
              }
              PONTO_E_VIRGULA
;

tipo        : IDENT 
;

lista_id_var: lista_id_var VIRGULA IDENT 
              { 
                /* insere �ltima vars na tabela de s�mbolos */ 
                int offset = peek_istack(&offset_stack);
                offset = insert_variable(table, token, lexical_level, offset);
                update_top_istack(&offset_stack, offset);

              }
            | IDENT {
                /* insere vars na tabela de s�mbolos */
                int offset = peek_istack(&offset_stack);
                offset = insert_variable(table, token, lexical_level, offset);
                update_top_istack(&offset_stack, offset);
             }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;

comando_sem_rotulo_ou_composto: comando_composto | comando_sem_rotulo

comando_composto: T_BEGIN lista_comandos T_END | T_BEGIN T_END

lista_comandos: comando | comando PONTO_E_VIRGULA | comando PONTO_E_VIRGULA lista_comandos 
;

comando: comando_sem_rotulo | 
  NUMERO {
    symb_pter = find_identifier(table, token);
    if (check_symbol_category(symb_pter, LABEL_SYMBOL_TYPE, NULL_CAT) == -1) return -1;
    // TODO: verificar se o n�mero de vari�veis locais est� correto
    sprintf(temp_str, "ENRT %d %d", lexical_level, malloced_local_vars);
    // Gera ENRT k, n (nivel lexico atual, numero variaveis locais)
    label_to_string(symb_pter->values.label.label, label);
    geraCodigo(label, temp_str);
    // TODO: verificar se � preciso limpar tabela de simbolos
  }
  DOIS_PONTOS comando_sem_rotulo 
;

comando_sem_rotulo: atribuicao_ou_chamada_procedimento |
                    comando_repetitivo |
                    comando_condicional |
                    leitura |
                    escrita |
                    chamada_goto 
;


chamada_goto: GOTO NUMERO {
    // TODO: testar se funciona corretamente
    // Encontrar rotulo na tabela de simbolos
    symb_pter = find_identifier(table, token);
    if (check_symbol_category(symb_pter, LABEL_SYMBOL_TYPE, NULL_CAT) == -1) return -1;
    label_to_string(symb_pter->values.label.label, label);
    // DSVR, p, j, k (rotulo alvo, nivel lexico destino, nivel lexico atual)
    sprintf(temp_str, "DSVR %s, %d, %d", label, symb_pter->lexical_level, lexical_level);
    geraCodigo(NULL, temp_str);
    // TODO: Verificar se � preciso alterar nivel lexico corrente aqui
    // lexical_level = symb_pter->lexical_level;
  } ;
;

leitura: READ ABRE_PARENTESES lista_leitura FECHA_PARENTESES 
;

lista_leitura: lista_leitura VIRGULA ident_variavel empilha_variavel { 
                    symb_pter = pop_symbol_stack(&symbol_stack);
                    geraCodigo(NULL, "LEIT");
                    if (assemble_read_write_instruction(temp_str, "ARMZ", symb_pter) == -1) return -1;
                    geraCodigo(NULL, temp_str);
                    } 
                    | ident_variavel empilha_variavel {
                      symb_pter = pop_symbol_stack(&symbol_stack);
                      geraCodigo(NULL, "LEIT");
                    if (assemble_read_write_instruction(temp_str, "ARMZ", symb_pter) == -1) return -1;
                      geraCodigo(NULL, temp_str);
                    } 

escrita: WRITE ABRE_PARENTESES lista_escrita FECHA_PARENTESES 
;

lista_escrita: lista_escrita VIRGULA elemento { geraCodigo(NULL, "IMPR"); }
              | elemento { geraCodigo(NULL, "IMPR"); }

comando_condicional: if_then {
                label_pter = pop_label_stack(&label_stack); 
                geraCodigo(label_pter2, "NADA");
                free(label_pter);
              }
              cond_else {
                label_pter = pop_label_stack(&label_stack);
                geraCodigo(label_pter, "NADA");
                free(label_pter);
              }
;

if_then: IF expr_condicional {
            generate_label(&label_counter, (char * )label);
            push_label_stack(&label_stack, label);
            generate_label(&label_counter, (char * )label);
            push_label_stack(&label_stack, label);
            sprintf(temp_str, "DSVF %s", label);
            geraCodigo(NULL, temp_str);
         } 
         THEN comando_sem_rotulo_ou_composto {
            label_pter = pop_label_stack(&label_stack);
            label_pter2 = pop_label_stack(&label_stack);
            sprintf(temp_str, "DSVS %s", label_pter2);
            geraCodigo(NULL, temp_str);
            push_label_stack(&label_stack, label_pter2);
            push_label_stack(&label_stack, label_pter);
            free(label_pter);
            free(label_pter2);
         }

;

cond_else: ELSE
           comando_sem_rotulo_ou_composto
           | %prec LOWER_THAN_ELSE

comando_repetitivo: WHILE {
                            generate_label(&label_counter, (char * )label);
                            geraCodigo(label, "NADA");
                            push_label_stack(&label_stack, label);
                            generate_label(&label_counter, (char * )label);
                            push_label_stack(&label_stack, label);
                          }
                    expr_condicional {
                            label_pter = pop_label_stack(&label_stack);
                            sprintf(temp_str, "DSVF %s", label_pter);
                            geraCodigo(NULL, temp_str);
                            push_label_stack(&label_stack, label);
                            free(label_pter);
                         }
                    DO
                    comando_sem_rotulo_ou_composto { }
                    { 
                      label_pter2 = pop_label_stack(&label_stack);
                      label_pter = pop_label_stack(&label_stack);
                      sprintf(temp_str, "DSVS %s", label_pter);
                      geraCodigo(NULL, temp_str);
                      geraCodigo(label_pter2, "NADA");
                      free(label_pter);
                    }
;


atribuicao_ou_chamada_procedimento: IDENT { strcpy(last_identifier, token); } acontinua
;

acontinua: atribuicao | chamada_sem_parametro | chamada_com_parametros 


chamada_sem_parametro: { 
  symb_pter = find_identifier(table, last_identifier); 
  if (symb_pter == NULL) {
    fprintf(stderr, "ERROR: procedure %s was not found! Double check that you've declared it!\n", symb_pter->identifier);
    return -1;
  } else {
      if (check_symbol_category(symb_pter, PROCEDURE, FUNCTION) == -1) return -1;
      char label[55];
      if (symb_pter->category == PROCEDURE) {
        last_param_list = symb_pter->values.procedure.parameter_list;
        if (l_size(last_param_list) > 0) {
          fprintf(stderr, "ERROR: procedure signature mismatch. Expecting %d parameters!\n", l_size(last_param_list));
          return -1;
        }
        label_to_string(symb_pter->values.procedure.label, label);
      } else {
        last_param_list = symb_pter->values.function.parameter_list;
        if (l_size(last_param_list) > 0) {
          fprintf(stderr, "ERROR: function signature mismatch. Expecting %d parameters!\n", l_size(last_param_list));
          return -1;
        }
        geraCodigo(NULL, "AMEM 1");
      }
      sprintf(temp_str, "CHPR %s, %d", label, lexical_level);
      geraCodigo(NULL, temp_str);
  }
}
;

chamada_com_parametros: ABRE_PARENTESES {
    symb_pter = find_identifier(table, last_identifier); 
    if (check_symbol_category(symb_pter, PROCEDURE, FUNCTION) == -1) return -1;
    if (symb_pter->category == PROCEDURE) {
      last_param_list = symb_pter->values.procedure.parameter_list;
    } else {
      last_param_list = symb_pter->values.function.parameter_list;
      geraCodigo(NULL, "AMEM 1");
    }
    caller_param_list = l_init();
    l_copy(last_param_list, caller_param_list);
  } 
  lista_parametros_chamada {
    if (l_size(caller_param_list) > 0) {
      fprintf(stderr, "ERROR: procedure signature mismatch. Expecting %d parameters!\n", l_size(last_param_list));
      l_free(caller_param_list);
      return -1;
    }
    l_free(caller_param_list);
  }
  FECHA_PARENTESES {
    symb_pter = find_identifier(table, last_identifier); 
    if (symb_pter == NULL) {
      fprintf(stderr, "ERROR: procedure %s was not found! Double check that you've declared it!\n", symb_pter->identifier);
      return -1;
    } else {
      if (check_symbol_category(symb_pter, PROCEDURE, FUNCTION) == -1) return -1;
        char label[55];
        if (symb_pter->category == PROCEDURE) {
          label_to_string(symb_pter->values.procedure.label, label);
        } else {
          label_to_string(symb_pter->values.function.label, label);
        }
        sprintf(temp_str, "CHPR %s, %d", label, lexical_level);
        geraCodigo(NULL, temp_str);

    }
  }
;

lista_parametros_chamada: lista_parametros_chamada VIRGULA parametro_chamada | parametro_chamada;


parametro_chamada: {
    list_node = pop_first(caller_param_list);
    if (list_node == NULL) {
      fprintf(stderr, "ERROR: procedure signature mismatch. Expecting %d parameters!\n", l_size(last_param_list));
      free(list_node);
      free(caller_param_list);
      return -1;
    }
    current_param_type = list_node->parameter_type;
  }
  expr { 
  push_type_stack(&var_type_stack, list_node->variable_type);
  if (type_check(&var_type_stack, nl) == -1) {
    char var_type_str[255];
    variable_type_to_string(list_node->variable_type, var_type_str);
    fprintf(stderr, "ERROR: procedure signature mismatch. Expecting type %s for parameter: %s\n", var_type_str, list_node->identifier);
    free(list_node);
    free(caller_param_list);
    return -1;
  }
  current_param_type = NULL_PARAM;
  free(list_node);
}
;


atribuicao: 
            DOIS_PONTOS_IGUAL {
                symb_pter = find_identifier(table, last_identifier); 
                if (symb_pter == NULL) {
                  fprintf(stderr, "ERROR: variable %s was not found! Double check that you've declared it!\n", symb_pter->identifier);
                  return -1;
                }
                if (symb_pter->category != VARIABLE && symb_pter->category != PARAMETER && symb_pter->category != FUNCTION) {
                  char category[255];
                  category_type_to_string(symb_pter->category, (char *) &category);
                  fprintf(stderr, "ERROR: Symbol %s is not a variable or a parameter! Declared as: %s\n", symb_pter->identifier, category);
                  return -1;
                }
                switch (symb_pter->category) {
                  case VARIABLE:
                    push_symbol_stack(&symbol_stack, *symb_pter);
                    push_type_stack(&var_type_stack, symb_pter->values.variable.variable_type);
                  break;
                  case PARAMETER:
                    push_symbol_stack(&symbol_stack, *symb_pter);
                    push_type_stack(&var_type_stack, symb_pter->values.parameter.variable_type);
                  break;
                  case FUNCTION:
                    push_symbol_stack(&symbol_stack, *symb_pter);
                    push_type_stack(&var_type_stack, symb_pter->values.function.variable_type);
                  break;
                }
              }
            expr { /* Gera sequencia de operacoes, confere tipos */ }
            {
              /* Desempilha endereco de memoria e gera ARMZ/ARMI */ 
                if (type_check(&var_type_stack, nl) == -1) return -1;
                symb_pter = pop_symbol_stack(&symbol_stack);
                if (symb_pter->category == VARIABLE || symb_pter->category == FUNCTION ||
                   (symb_pter->category == PARAMETER && symb_pter->values.parameter.parameter_type == BYVAL)) {
                  if (assemble_read_write_instruction(temp_str, "ARMZ", symb_pter) == -1) return -1;
                } else if (symb_pter->category == PARAMETER &&
                           symb_pter->values.parameter.parameter_type == BYREFERENCE) {
                  if (assemble_read_write_instruction(temp_str, "ARMI", symb_pter) == -1) return -1; 
                }
                geraCodigo(NULL, temp_str);
            }
              ;


expr_condicional: expr relacao expressao_simples {  geraCodigo(NULL, last_instruction); }
;

expr: expressao_simples | expr relacao expressao_simples {
                          /* Gerar instrucao de comparacao aqui! */
                          geraCodigo(NULL, last_instruction);
                          push_type_stack(&var_type_stack, BOOLEAN);
                           }
                        |
;


relacao: IGUAL { sprintf(last_instruction, "CMIG"); } |
         MENOR { sprintf(last_instruction, "CMME"); } |
         MAIOR { sprintf(last_instruction, "CMMA"); } |
         MENOR IGUAL { sprintf(last_instruction, "CMEG"); } |
         MAIOR IGUAL { sprintf(last_instruction, "CMAG"); } |
         MENOR MAIOR { sprintf(last_instruction, "CMDG"); } // diferente
;


expressao_simples: expressao_simples MAIS termo 
                    { geraCodigo(NULL, "SOMA");
                      if (type_check(&var_type_stack, nl) == -1) return -1;
                    } |
                  expressao_simples MENOS termo 
                    { geraCodigo(NULL, "SUBT"); 
                      if (type_check(&var_type_stack, nl) == -1) return -1;
                    } |
                  expressao_simples OR termo
                    {geraCodigo(NULL, "DISJ"); 
                      if (type_check(&var_type_stack, nl) == -1) return -1;
                      } |
                  termo
;

termo: termo DIV fator 
        { geraCodigo(NULL, "DIVI"); 
          if (type_check(&var_type_stack, nl) == -1) return -1;
        } |
       fator
;

fator: fator ASTERICO elemento 
        {  geraCodigo(NULL, "MULT"); 
          if (type_check(&var_type_stack, nl) == -1) return -1;
        } |
       fator AND elemento 
        { geraCodigo(NULL, "CONJ");
          if (type_check(&var_type_stack, nl) == -1) return -1;
        } |
       elemento | 
       ABRE_PARENTESES expr FECHA_PARENTESES
;

elemento: num | 
          boolean |
          ident_variavel { strcpy(last_identifier, last_variable_identifier); } chamada_com_parametros |
          ident_variavel empilha_variavel {
          /* Desempilha endereco da memoria da pilha */
          symb_pter = pop_symbol_stack(&symbol_stack);
          // print_table(table);
          if (symb_pter->category == FUNCTION) {
          /* Caso especial em que fun��o n�o tem par�metros */
            last_param_list = symb_pter->values.function.parameter_list;
            if (l_size(last_param_list) > 0) {
              fprintf(stderr, "ERROR: function signature mismatch. Expecting %d parameters!\n", l_size(last_param_list));
              return -1;
            }
            geraCodigo(NULL, "AMEM 1");
            sprintf(temp_str, "CHPR %s, %d", label, lexical_level);
            geraCodigo(NULL, temp_str);
            break;
          } 
          /* Primeiro if marca que est� empilhando par�metro passado por refer�ncia */
          if (current_param_type == BYREFERENCE) {
            if ((symb_pter->category == PARAMETER && symb_pter->values.parameter.parameter_type == BYVAL) ||
                 symb_pter->category == VARIABLE) {
              if (assemble_read_write_instruction(temp_str, "CREN", symb_pter) == -1) return -1;
            } else {
              if (assemble_read_write_instruction(temp_str, "CRVL", symb_pter) == -1) return -1;
            }
          } /* Sen�o, trata refer�ncias normais �s vari�veis/par�metros */
          else {
            if (symb_pter->category == PARAMETER && symb_pter->values.parameter.parameter_type == BYREFERENCE) {
              if (assemble_read_write_instruction(temp_str, "CRVI", symb_pter) == -1) return -1;
            }
            else {
              if (assemble_read_write_instruction(temp_str, "CRVL", symb_pter) == -1) return -1;
            }
          }
          geraCodigo(NULL, temp_str);
        }
;


ident_variavel : IDENT {
  { strcpy(last_variable_identifier, token); }
}
;

num: NUMERO { 
              sprintf(temp_str, "CRCT %s", token); 
              push_type_stack(&var_type_stack, INTEGER);
              geraCodigo(NULL, temp_str);
            }

;


empilha_variavel: { 
  /* `Procura variavel na tabela de simbolos e empilha endereco */ 
    symb_pter = find_identifier(table, last_variable_identifier); 
    if (symb_pter == NULL) {
        fprintf(stderr, "ERROR: variable %s was not found! Double check that you've declared it!\n", symb_pter->identifier);
        return -1;
    }
    switch(symb_pter->category) {
      case VARIABLE:
        push_symbol_stack(&symbol_stack, *symb_pter);
        push_type_stack(&var_type_stack, symb_pter->values.variable.variable_type);
        break;
      case PARAMETER:
        push_symbol_stack(&symbol_stack, *symb_pter);
        push_type_stack(&var_type_stack, symb_pter->values.parameter.variable_type);
        break;
      case FUNCTION:
        push_symbol_stack(&symbol_stack, *symb_pter);
        push_type_stack(&var_type_stack, symb_pter->values.function.variable_type);
        break;
      default:
        fprintf(stderr, "ERROR: Invalid Category!\n");
        return -1;
    }
}
;


boolean: TRUE { geraCodigo(NULL, "CRCT 1");
                push_type_stack(&var_type_stack, BOOLEAN); 
              } |
         FALSE { geraCodigo(NULL, "CRCT 0");
                 push_type_stack(&var_type_stack, BOOLEAN);
               }
               ;


%%

main (int argc, char** argv) {
   FILE* fp;
   extern FILE* yyin;

   if (argc<2 || argc>2) {
         printf("usage compilador <arq>a %d\n", argc);
         return(-1);
      }

   fp=fopen (argv[1], "r");
   if (fp == NULL) {
      printf("usage compilador <arq>b\n");
      return(-1);
   }


/* -------------------------------------------------------------------
 *  Inicia a Tabela de S�mbolos
 * ------------------------------------------------------------------- */

   table = malloc_table(MAX_TABLE_SIZE);
   symbol new_symbol;
   init_type_stack(&var_type_stack);
   init_istack(&offset_stack);
   init_symbol_stack(&symbol_stack);
 

   yyin=fp;
   if (yyparse() == -1) {
    fprintf(stderr, ">>>>>>\n");
    char buffer[1024];
    int cl = 0;
    nl--;
    fseek(fp, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), fp) && cl != nl) {
      cl++;
    }
    fprintf(stderr, "%s\n", buffer);
   }

   // print_type_stack(&var_type_stack);
   free_table(table);
   fclose(fp);
   return 0;
}

void yyerror(char const * s) {
  extern FILE * yyin;
  fprintf(stderr, "ERROR:  %s\n", s);
  FILE * fp = yyin;
  char buffer[1024];
  int cl = 0;
  nl--;
  fseek(fp, 0, SEEK_SET);
  while (fgets(buffer, sizeof(buffer), fp) && cl != nl - 1) {
    cl++;
  }
  fprintf(stderr, "At line: %d\n >>>>>>\n", nl);
  fprintf(stderr, "\t%d: %s", nl - 1, buffer);
  fgets(buffer, sizeof(buffer), fp);
  fprintf(stderr, "\t%d: %s", nl, buffer);
}

int type_check(tvar_type_stack * var_type_stack, int nl) {
    t1 = pop_type_stack(var_type_stack);
    t2 = pop_type_stack(var_type_stack);
    char var_type_str1[255];
    char var_type_str2[255];
    variable_type_to_string(t1, (char *) &var_type_str1);
    variable_type_to_string(t2, (char *) &var_type_str2);
    if (t1 != t2) {
      fprintf( stderr, "ERROR: Type mismatch at line %d: t1: %s, t2:%s\n", nl, var_type_str1, var_type_str2);
      return -1;
    }
    push_type_stack(var_type_stack, t1);
    return 0;
}
