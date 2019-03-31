
// Testar se funciona corretamente o empilhamento de par�metros
// passados por valor ou por refer�ncia.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "tabela_simbolos.h"
#include "pilhas_auxiliares.h"

int num_vars;
int lexical_level = 0;
int offset = 0;

symbol new_symbol;
symbol_table * table;
tvar_type_stack var_type_stack;

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
             geraCodigo (NULL, "PARA"); 
             /* DMEM aqui? */
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
                /* insere �ltima vars na tabela de s�mbolos */ 
                offset = insert_variable(table, token, lexical_level, offset);

              }
            | IDENT {
                /* insere vars na tabela de s�mbolos */
                offset = insert_variable(table, token, lexical_level, offset);
             }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;


atribuicao: variavel 
            DOIS_PONTOS_IGUAL 
            expr { /* Gera sequencia de operacoes, confere tipos */ }
            PONTO_E_VIRGULA { /* Desempilha endereco de memoria e gera ARMZ */ }


expr: expr MAIS termo { geraCodigo(NULL, "SOMA"); } |
      expr MENOS termo { geraCodigo(NULL, "SUBT"); } |
      expr OR termo {geraCodigo(NULL, "DISJ"); } |
      termo
;

termo: termo BARRA fator { geraCodigo(NULL, "DIVI"); } |
       fator
;

fator: fator ASTERICO num {  geraCodigo(NULL, "MULT"); } |
       fator AND num {  geraCodigo(NULL, "CONJ"); } |
       num { push_type_stack(&var_type_stack, INTEGER); } | 
       boolean { push_type_stack(&var_type_stack, BOOLEAN); } 
;

num: NUMERO { sprintf(temp_str, "CRCT %s", token); geraCodigo(NULL, temp_str); }
;

comando_composto: T_BEGIN comandos T_END 

comandos: comandos comando | comando
;

comando: comando_sem_rotulo | NUMERO DOIS_PONTOS comando_sem_rotulo

comando_sem_rotulo: atribuicao

variavel: IDENT { /*Procura variavel na tabela de simbolos e empilha endereco? */ }

boolean: TRUE | FALSE


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

   yyin=fp;
   yyparse();

  
   print_type_stack(&var_type_stack);
   print_table(table);
   free_table(table);
   return 0;
}

