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
\&    { LE_TOKEN(AND); }
\|    { LE_TOKEN(OR); }
a  { LE_TOKEN(INT_IDENT); }
b  { LE_TOKEN(BOOLEAN_IDENT); }

%%

