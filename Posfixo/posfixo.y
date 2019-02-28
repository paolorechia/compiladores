
%{
#include <stdio.h>
#include <string.h>
#include "posfixo.h"

%}

%token INT_IDENT BOOLEAN_IDENT MAIS MENOS OR ASTERISCO DIV ABRE_PARENTESES FECHA_PARENTESES AND

%%

expr       : expr MAIS termo {printf ("+"); } |
             expr MENOS termo {printf ("-"); } | 
             termo | expr_boolean
;

termo      : termo ASTERISCO fator  {printf ("*"); }| 
             termo DIV fator  {printf ("/"); }|
             fator
;

fator      : INT_IDENT {printf ("A"); }


expr_boolean: expr_boolean OR termo_boolean {printf("|"); } |
              termo_boolean

termo_boolean: termo_boolean AND boolean {printf("&"); } |
               boolean

boolean   :  BOOLEAN_IDENT { printf("B"); }

;

%%

main (int argc, char** argv) {
   yyparse();
   printf("\n");
}

yyerror(char *s) {
  fprintf(stderr, "error: %s\n", s);
}
