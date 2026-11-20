%{
#include "tokens.h"
%}

%option noyywrap
%option caseless
%option yylineno
%option nounput
%option noinput

DIGITO   [0-9]
LETRA    [a-z_]

%%

program|var|procedure|function|begin|end|if|then|else|while|do|for|to|downto|repeat|until  return T_PALAVRA;
integer|real|boolean|char|string|true|false|and|or|not|div|mod                             return T_PALAVRA;

{DIGITO}+"."{DIGITO}+           return T_REAL;
{DIGITO}+                       return T_INTEIRO;
{LETRA}({LETRA}|{DIGITO})*      return T_ID;
'([^'\n]|'')*'                  return T_TEXTO;

":="|"<>"|"<="|">="             return T_SIMBOLO;
[:;,.()+*/=<>-]                 return T_SIMBOLO;

"{"[^}]*"}"                     ;
"(*"([^*]|"*"+[^*)])*"*"+")"    ;
"//".*                          ;
[ \t\r\n]+                      ;

.                               return T_ERRO;

%%
