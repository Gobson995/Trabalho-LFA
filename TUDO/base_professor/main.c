

#include <stdio.h>
#include <stdlib.h>



#include "tokens.h"
#include "lex.yy.c"



main( int argc, char **argv )
{
    int token;
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
        yyin = fopen( argv[0], "r" );
    else
        yyin = stdin;

    token = yylex();
    while(token != 0)
    {
        if(token == T_INTEGER){
            printf("::integer -> %d\n", atoi(yytext));
        }
        if(token==T_ID){
            printf(":: %s\n", yytext);
        }
        token = yylex();
    }
}



