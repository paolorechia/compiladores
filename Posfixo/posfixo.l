%{
#include "posfixo.tab.h"
#include "posfixo.h"
#include <string.h>

#define LE_TOKEN(STR) {\
              strncpy(last_token, yytext, N);\
              return STR;\
              }


char last_token[N];
%}



%%

[\n\t ]  

\+    { LE_TOKEN(MAIS) }
\-    { LE_TOKEN(MENOS); }
\*    { LE_TOKEN(ASTERISCO);  }
\/    { LE_TOKEN(DIV);  }
[ab]  { LE_TOKEN(IDENT); }

%%

