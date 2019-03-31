
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "tabela_simbolos.h"

int num_vars;
int lexical_level = 0;
int offset = 0;

symbol new_symbol;
symbol_table * table;

extern char * yytext;
char temp_str[TAM_TOKEN];

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token IF NUMERO
%token MAIS MENOS ASTERICO BARRA

%%

programa    :{ 
             geraCodigo (NULL, "INPP"); 
             }
             PROGRAM IDENT 
             ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
             bloco PONTO {
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
                printf("AMEN %d\n", update_var_type(table, token));   
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


atribuicao: IDENT   
            ATRIBUICAO 
            expr 
            PONTO_E_VIRGULA;


expr: expr MAIS termo { geraCodigo(NULL, "SOMA"); } |
      expr MENOS termo { geraCodigo(NULL, "SUBT"); } |
      termo
;

termo: termo BARRA fator { geraCodigo(NULL, "DIVI"); } |
       fator
;

fator: fator ASTERICO num {  geraCodigo(NULL, "MULT"); } |
       num
;

num: NUMERO { sprintf(temp_str, "CRCT %s", token); geraCodigo(NULL, temp_str); }
;

comando_composto: T_BEGIN comandos T_END 

comandos: atribuicao comandos |
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

   table  = malloc_table(MAX_TABLE_SIZE);
   symbol new_symbol;

   yyin=fp;
   yyparse();

  
   print_table(table);
   free_table(table);
   return 0;
}

