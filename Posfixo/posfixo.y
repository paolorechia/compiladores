
%{
#include <stdio.h>
#include <string.h>
#include "posfixo.h"

%}

%token IDENT MAIS MENOS OR ASTERISCO DIV ABRE_PARENTESES FECHA_PARENTESES

%%

expr       : expr MAIS termo {printf ("+"); } |
             expr MENOS termo {printf ("-"); } | 
             termo
;

termo      : termo ASTERISCO fator  {printf ("*"); }| 
             termo DIV fator  {printf ("/"); }|
             fator
;

fator      : IDENT {printf ("%s", last_token); }
;

%%

main (int argc, char** argv) {
   yyparse();
   printf("\n");
}

