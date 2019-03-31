
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


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

symbol new_symbol;
symbol * symb_pter;
symbol_table * table;
tvar_type_stack var_type_stack;
tsymbol_stack symbol_stack;
VariableType t1;
VariableType t2;

extern char * yytext;
char temp_str[TAM_TOKEN];

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO 
%token T_BEGIN T_END VAR IDENT DOIS_PONTOS_IGUAL
%token MENOR MAIOR IGUAL
%token IF NUMERO
%token MAIS MENOS ASTERICO BARRA
%token AND OR TRUE FALSE

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
              parte_declara_vars
              { 
              }

              comando_composto
              ;




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

comando_composto: T_BEGIN comandos T_END | T_BEGIN T_END
;

comandos: comandos comando | comando
;

comando: comando_sem_rotulo | NUMERO DOIS_PONTOS comando_sem_rotulo
;

comando_sem_rotulo: atribuicao
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

expr: expressao_simples | expr relacao expressao_simples
;


relacao: IGUAL { geraCodigo(NULL, "CMIG"); } |
         MENOR { geraCodigo(NULL, "CMME"); } |
         MAIOR { geraCodigo(NULL, "CMMA"); } |
         MENOR IGUAL { geraCodigo(NULL, "CMEG"); } |
         MAIOR IGUAL { geraCodigo(NULL, "CMAG"); } |
         MENOR MAIOR { geraCodigo(NULL, "CMDG"); } // diferente
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
       elemento 
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
