
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


%define parse.error verbose


%{

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "tabela_simbolos.h"
#include "pilhas_auxiliares.h"

#define CHECK_TYPE 


int num_vars;
int lexical_level = 0;
int offset = 0;
int label_counter = 0;
char label[LABEL_MAX_SIZE];
char * label_pter;
char * label_pter2;

char last_instruction[TAM_TOKEN];

symbol new_symbol;
symbol * symb_pter;
symbol_table * table;
tvar_type_stack var_type_stack;
tsymbol_stack symbol_stack;
tlabel_stack label_stack;

VariableType t1;
VariableType t2;

extern char * yytext;
char temp_str[TAM_TOKEN];

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO 
%token T_BEGIN T_END VAR IDENT DOIS_PONTOS_IGUAL
%token MENOR MAIOR IGUAL
%token PROCEDURE_TOKEN FUNCTION_TOKEN
%token IF WHILE DO THEN ELSE NUMERO
%token MAIS MENOS ASTERICO BARRA
%token AND OR TRUE FALSE
%token READ WRITE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE 

%%

programa    :{ 
             geraCodigo (NULL, "INPP"); 
             }
             PROGRAM IDENT 
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
             print_table(table);
             num_vars = remove_local_vars(table);
             sprintf(temp_str, "DMEM %d", num_vars);
             geraCodigo (NULL, temp_str);
             geraCodigo (NULL, "PARA"); 
             }
;

bloco       : 
              parte_declara_vars declara_subrotina comando_composto
;

declara_subrotina: declara_procedimento |

declara_procedimento: PROCEDURE_TOKEN IDENT {
                        lexical_level++;

                        generate_label(&label_counter, (char * )label);
                        push_label_stack(&label_stack, label);
                        sprintf(temp_str, "DSVS %s", label);
                        geraCodigo(NULL, temp_str);

                        generate_label(&label_counter, (char * )label);
                        push_label_stack(&label_stack, label);
                        sprintf(temp_str, "ENPR %d", lexical_level);
                        label_pter = pop_label_stack(&label_stack);
                        geraCodigo(label_pter, temp_str);

                        insert_procedure(table, token, lexical_level, label);
                        print_table(table);
                      }
                      lp PONTO_E_VIRGULA 
                      bloco {
                        label_pter = pop_label_stack(&label_stack); 
                        geraCodigo(label_pter, "NADA");
                      }

lp: ABRE_PARENTESES lista_parametros FECHA_PARENTESES |
;

lista_parametros: lista_parametros VIRGULA parametro | parametro

parametro: IDENT | VAR IDENT;

/*
  l_insert(new_symbol.values.procedure.parameter_list, INTEGER, BYVAL);
  l_insert(new_symbol.values.procedure.parameter_list, BOOLEAN, BYVAL);
  l_insert(new_symbol.values.procedure.parameter_list, INTEGER, BYREFERENCE);
*/

//lista_parametros: VAR IDENT | IDENT |;

//declara_subrotina: declara_procedimento | declara_funcao;

//declara_funcao: FUNCTION_TOKEN IDENT lista_parametros bloco
//;

parte_declara_vars:  var 
;


var         : { } VAR declara_vars
            |
;

declara_vars: declara_vars declara_var 
            | declara_var 
;

declara_var : { } 
              lista_id_var DOIS_PONTOS 
              tipo 
              { /* AMEM */
                num_vars = update_var_type(table, token);
                sprintf(temp_str, "AMEN %d", num_vars);
                geraCodigo (NULL, temp_str); 
              }
              PONTO_E_VIRGULA
;

tipo        : IDENT 
;

lista_id_var: lista_id_var VIRGULA IDENT 
              { 
                /* insere última vars na tabela de símbolos */ 
                offset = insert_variable(table, token, lexical_level, offset);

              }
            | IDENT {
                /* insere vars na tabela de símbolos */
                offset = insert_variable(table, token, lexical_level, offset);
             }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;

comando_sem_rotulo_ou_composto: comando_composto_pv | comando_sem_rotulo 

comando_composto_pv: T_BEGIN comandos T_END PONTO_E_VIRGULA | T_BEGIN T_END PONTO_E_VIRGULA

comando_composto: T_BEGIN comandos T_END | T_BEGIN T_END

comandos: comandos comando | comando
;

comando: comando_sem_rotulo | NUMERO DOIS_PONTOS comando_sem_rotulo
;

comando_sem_rotulo: atribuicao_ou_chamada_procedimento |
                    comando_repetitivo |
                    comando_condicional |
                    leitura |
                    escrita
;

leitura: READ ABRE_PARENTESES lista_leitura FECHA_PARENTESES PONTO_E_VIRGULA
;

lista_leitura: lista_leitura VIRGULA variavel { 
                    symb_pter = pop_symbol_stack(&symbol_stack);
                    geraCodigo(NULL, "LEIT");
                    sprintf(temp_str, "ARMZ %d, %d", symb_pter->values.variable.lexical_level,
                                                     symb_pter->values.variable.offset);
                    geraCodigo(NULL, temp_str);
                    } 
                    | variavel {
                      symb_pter = pop_symbol_stack(&symbol_stack);
                      geraCodigo(NULL, "LEIT");
                      sprintf(temp_str, "ARMZ %d, %d", symb_pter->values.variable.lexical_level,
                                                       symb_pter->values.variable.offset);
                      geraCodigo(NULL, temp_str);
                    } 

escrita: WRITE ABRE_PARENTESES lista_escrita FECHA_PARENTESES PONTO_E_VIRGULA
;

lista_escrita: lista_escrita VIRGULA elemento { geraCodigo(NULL, "IMPR"); }
              | elemento { geraCodigo(NULL, "IMPR"); }

comando_condicional: if_then {
                label_pter = pop_label_stack(&label_stack); 
                label_pter2 = pop_label_stack(&label_stack); 
                geraCodigo(label_pter2, "NADA");
                push_label_stack(&label_stack, label_pter);
              }
              cond_else {
                label_pter = pop_label_stack(&label_stack);
                geraCodigo(label_pter, "NADA");
              }
;

if_then: IF expr {
            generate_label(&label_counter, (char * )label);
            push_label_stack(&label_stack, label);
            sprintf(temp_str, "DSVF %s", label);
            geraCodigo(NULL, temp_str);
            generate_label(&label_counter, (char * )label);
            push_label_stack(&label_stack, label);
         } 
         THEN comando_sem_rotulo_ou_composto {
            label_pter = peek_label_stack(&label_stack); 
            sprintf(temp_str, "DSVS %s", label_pter);
            geraCodigo(NULL, temp_str);
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
                    expr { 
                            label_pter = peek_label_stack(&label_stack); 
                            sprintf(temp_str, "DSVF %s", label_pter);
                            geraCodigo(NULL, temp_str);
                         }
                    DO
                    comando_composto { }
                    PONTO_E_VIRGULA { 
                      label_pter2 = pop_label_stack(&label_stack);
                      label_pter = pop_label_stack(&label_stack);
                      sprintf(temp_str, "DSVS %s", label_pter);
                      geraCodigo(NULL, temp_str);
                      geraCodigo(label_pter2, "NADA");
                    }
;

atribuicao_ou_chamada_procedimento: atribuicao
                                    // | chamada_sem_parametro
;

chamada_sem_parametro: IDENT PONTO_E_VIRGULA
;


atribuicao: variavel 
            DOIS_PONTOS_IGUAL 
            expr { /* Gera sequencia de operacoes, confere tipos */ }
            PONTO_E_VIRGULA {
              /* Desempilha endereco de memoria e gera ARMZ */ 
                if (type_check(&var_type_stack, nl) == -1) return -1;
                symb_pter = pop_symbol_stack(&symbol_stack);
                sprintf(temp_str, "ARMZ %d, %d", symb_pter->values.variable.lexical_level,
                                                 symb_pter->values.variable.offset);
                geraCodigo(NULL, temp_str);
              }
              ;


expr: expressao_simples | expr relacao expressao_simples {
                          /* Gerar instrucao de comparacao aqui! */
                          geraCodigo(NULL, last_instruction);
                           }
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

termo: termo BARRA fator 
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
          variavel {
          /* Desempilha endereco da memoria da pilha */
          symb_pter = pop_symbol_stack(&symbol_stack);
          sprintf(temp_str, "CRVL %d, %d", symb_pter->values.variable.lexical_level,
                                           symb_pter->values.variable.offset);
          geraCodigo(NULL, temp_str);
          } 
          ;

num: NUMERO { sprintf(temp_str, "CRCT %s", token); 
              push_type_stack(&var_type_stack, INTEGER);
              geraCodigo(NULL, temp_str);
            }
;


variavel: IDENT { 
  /*Procura variavel na tabela de simbolos e empilha endereco? */ 
    symb_pter = find_identifier(table, token); 
    if (symb_pter != NULL) {
      push_symbol_stack(&symbol_stack, *symb_pter);
      push_type_stack(&var_type_stack, symb_pter->values.variable.variable_type);
    } else {
      // bad things happened
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
 *  Inicia a Tabela de Símbolos
 * ------------------------------------------------------------------- */

   table = malloc_table(MAX_TABLE_SIZE);
   symbol new_symbol;
   init_type_stack(&var_type_stack);

   yyin=fp;
   if (yyparse() == -1) {
    printf(">>>>>>\n");
    char buffer[1024];
    int cl = 0;
    nl--;
    fseek(fp, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), fp) && cl != nl) {
      cl++;
    }
    printf("%s\n", buffer);
   }

   print_type_stack(&var_type_stack);
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
  printf("At line: %d\n >>>>>>\n", nl);
  printf("\t%d: %s", nl - 1, buffer);
  fgets(buffer, sizeof(buffer), fp);
  printf("\t%d: %s", nl, buffer);
}

int type_check(tvar_type_stack * var_type_stack, int nl) {
    t1 = pop_type_stack(var_type_stack);
    t2 = pop_type_stack(var_type_stack);
    if (t1 != t2) {
      printf("ERROR: Type mismatch at line %d: t1: %d, t2:%d\n", nl, t1, t2);
      return -1;
    }
    push_type_stack(var_type_stack, t1);
    return 0;
}
