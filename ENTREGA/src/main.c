#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"
#include "gramatica.h"
#include "cyk.h"

extern FILE *yyin;
extern char *yytext;
extern int yylineno;
int yylex();

int main (int argc, char *argv[]) {
    int mostrar_gramatica = 0;
    int passo_a_passo = 0;
    int mostrar_arvore = 0;
    char *arquivo = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0) mostrar_gramatica = 1;
        else if (strcmp(argv[i], "-p") == 0) passo_a_passo = 1;
        else if (strcmp(argv[i], "-a") == 0) mostrar_arvore = 1;
        else arquivo = argv[i];
    }

    if (arquivo == NULL) {
        printf("Uso: main [-g] [-p] [-a] arquivo.pas\n");
        printf("  -g  mostra a conversao da gramatica para a FNC\n");
        printf("  -p  mostra o CYK passo a passo\n");
        printf("  -a  mostra a arvore de derivacao\n");
        return 2;
    }

    le_gramatica("gramatica.txt");
    if (mostrar_gramatica) {
        printf("GRAMATICA ORIGINAL\n");
        imprime_gramatica();
    }
    tira_vazias(mostrar_gramatica);
    tira_unitarias(mostrar_gramatica);
    tira_inuteis(mostrar_gramatica);
    converte_fnc(mostrar_gramatica);

    yyin = fopen(arquivo, "r");
    if (yyin == NULL) {
        printf("Nao foi possivel abrir %s\n", arquivo);
        return 2;
    }

    printf("ANALISE LEXICA\n");
    int token = yylex();
    while (token != 0) {
        if (token == T_ERRO) {
            printf("\nErro lexico na linha %d: '%s' nao pertence a linguagem\n", yylineno, yytext);
            printf("REJEITADA\n");
            return 1;
        }

        char nome[30];
        if (token == T_PALAVRA) {
            int i;
            for (i = 0; yytext[i] != '\0'; i++) {
                nome[i] = tolower(yytext[i]);
            }
            nome[i] = '\0';
        }
        if (token == T_SIMBOLO) strcpy(nome, yytext);
        if (token == T_ID) strcpy(nome, "id");
        if (token == T_INTEIRO) strcpy(nome, "num_int");
        if (token == T_REAL) strcpy(nome, "num_real");
        if (token == T_TEXTO) strcpy(nome, "texto");

        if (ntokens == MAX_TOKENS) {
            printf("Arquivo muito grande\n");
            return 2;
        }

        palavra[ntokens] = busca_simbolo(nome);
        strncpy(lexema[ntokens], yytext, 99);
        printf("  %3d  linha %3d  %-10s %s\n", ntokens + 1, yylineno, nome, yytext);
        ntokens++;

        token = yylex();
    }

    if (ntokens == 0) {
        printf("Arquivo vazio\nREJEITADA\n");
        return 1;
    }

    printf("\nALGORITMO CYK\n");
    int aceita = cyk(passo_a_passo);
    printf("%d tokens, %ld combinacoes (s, r, k), %ld com as duas celulas preenchidas, %.1f ms\n",
           ntokens, combinacoes, combinacoes_uteis, tempo);

    if (aceita) {
        printf("ACEITA: Programa pertence a V[1][%d]\n", ntokens);
    } else {
        printf("REJEITADA: Programa nao pertence a V[1][%d]\n", ntokens);
        return 1;
    }

    if (mostrar_arvore) {
        printf("\nARVORE DE DERIVACAO\n");
        imprime_arvore(0, 1, ntokens, 0);
    }

    return 0;
}
