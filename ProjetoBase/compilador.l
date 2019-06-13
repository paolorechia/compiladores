%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

#define IMPRIME(STR) {};
//#define IMPRIME(STR) printf("%s    # %s \n", STR, token);

#define LE_TOKEN(STR) {\
              strncpy(token, yytext, N);\
              return STR;\
              }

int nl = 1;  

%}

pulo_linha [\n]

descarte [\t ]+

ident [a-zAZ][a-zAZ1-9]*

numero [0-9]+

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;} 

{descarte}

{comentario}

program { simbolo  = simb_program;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("program ");
          return PROGRAM;
 }

var      { simbolo = simb_var;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("var  ");
          return VAR;
 }

begin { simbolo = simb_begin;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("begin ");
          return T_BEGIN;
 }


end { simbolo = simb_end;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("end  ");
          return T_END;
 }

:= { simbolo = simb_dois_pontos_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" dois_pontos_igual ");
          return DOIS_PONTOS_IGUAL;  
 }

\; { simbolo = simb_ponto_e_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" pt_virgula  ");
          return PONTO_E_VIRGULA;
 }


\: { simbolo = simb_dois_pontos;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" dois pontos ");
          return DOIS_PONTOS;
 }

= { simbolo = simb_igual;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" simb_igual ");
          return IGUAL;
 }

\> { simbolo = simb_maior;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" simb_maior");
          return MAIOR;
 }

\< { simbolo = simb_menor;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" simb_menor");
          return MENOR;
 }

\, { simbolo = simb_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" virgula  ");
          return VIRGULA;
 }


\. { simbolo = simb_ponto;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ponto  ");
          return PONTO; 
    }

\(  { simbolo = simb_abre_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" abre_parenteses  ");
          return ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" fecha_parenteses ");
          return FECHA_PARENTESES;
 }

\+  { simbolo = simb_mais;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" mais ");
          return MAIS;
 }
 
\-  { simbolo = simb_menos;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" menos ");
          return MENOS;
 }

\*  { simbolo = simb_asterico;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" asterico ");
          return ASTERICO;
 }

\/  { simbolo = simb_barra;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" barra ");
          return BARRA;
 }

and  { simbolo = simb_and;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" and ");
          return AND;
 }

or { simbolo = simb_or;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" or ");
          return OR;
 }

true { simbolo = simb_true;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" and ");
          return TRUE;
 }

false { simbolo = simb_false;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" false ");
          return FALSE;
 }

if  { simbolo = simb_if;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" if ");
          return IF;
 }

then  { simbolo = simb_then;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" then ");
          return THEN;
 }

else { simbolo = simb_else;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" else ");
          return ELSE;
 }
 
do  { simbolo = simb_do;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" do ");
          return DO;
 }

while  { simbolo = simb_while;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" while ");
          return WHILE;
 }
 
read { simbolo = simb_read;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" read ");
          return READ;
 }

write { simbolo = simb_write;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" write ");
          return WRITE;
 }

label { simbolo = simb_label;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" label ");
        return LABEL;
     }

goto { simbolo = simb_goto;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" goto ");
        return GOTO;
     }

forward { simbolo = simb_forward;
        strncpy (token, yytext, TAM_TOKEN);
        IMPRIME(" forward ");
        return FORWARD;
     }

procedure { simbolo = simb_procedure;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" procedure ");
          return PROCEDURE_TOKEN;
 }

function { simbolo = simb_function;
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" function ");
          return FUNCTION_TOKEN;
 }

{numero} { simbolo = simb_numero;
              strncpy(token, yytext, TAM_TOKEN);
              IMPRIME(" numero ");
              return NUMERO;
 }

{ident} { simbolo = simb_identificador; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return IDENT;
        }


%%

