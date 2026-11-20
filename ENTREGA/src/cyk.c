#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gramatica.h"
#include "cyk.h"

int palavra[MAX_TOKENS];
char lexema[MAX_TOKENS][100];
int ntokens = 0;
long combinacoes = 0;
long combinacoes_uteis = 0;
double tempo = 0;

char *tabela;
char *ocupada;

char *celula(int r, int s) {
    long pos = (long)(s - 1) * ntokens + (r - 1);
    return tabela + pos * nsimb;
}

int tem_algo(int r, int s) {
    return ocupada[(s - 1) * ntokens + (r - 1)];
}

void marca(int r, int s, int a) {
    celula(r, s)[a] = 1;
    ocupada[(s - 1) * ntokens + (r - 1)] = 1;
}

void imprime_celula(int r, int s) {
    int primeiro = 1;
    printf("{");
    for (int a = 0; a < nsimb; a++) {
        if (celula(r, s)[a]) {
            if (!primeiro) printf(", ");
            printf("%s", simbolos[a]);
            primeiro = 0;
        }
    }
    printf("}");
}

int cyk(int mostrar) {
    int n = ntokens;
    tabela = calloc((long)n * n * nsimb, 1);
    ocupada = calloc((long)n * n, 1);
    if (tabela == NULL || ocupada == NULL) {
        printf("Sem memoria para a tabela\n");
        exit(2);
    }

    clock_t inicio = clock();

    for (int r = 1; r <= n; r++) {
        for (int p = 0; p < nprod; p++) {
            if (tam[p] == 1 && dir[p][0] == palavra[r-1]) marca(r, 1, esq[p]);
        }
        if (mostrar) {
            printf("V[%d][1] = ", r);
            imprime_celula(r, 1);
            printf("   (%s)\n", simbolos[palavra[r-1]]);
        }
    }

    for (int s = 2; s <= n; s++) {
        for (int r = 1; r <= n - s + 1; r++) {
            for (int k = 1; k <= s - 1; k++) {
                combinacoes++;

                if (mostrar) {
                    printf("s=%d r=%d k=%d: B em V[%d][%d] = ", s, r, k, r, k);
                    imprime_celula(r, k);
                    printf(", C em V[%d][%d] = ", r + k, s - k);
                    imprime_celula(r + k, s - k);
                    printf("\n");
                }

                if (!tem_algo(r, k) || !tem_algo(r + k, s - k)) continue;
                combinacoes_uteis++;

                for (int p = 0; p < nprod; p++) {
                    if (tam[p] != 2) continue;
                    if (celula(r, k)[dir[p][0]] && celula(r + k, s - k)[dir[p][1]]) {
                        marca(r, s, esq[p]);
                        if (mostrar) {
                            printf("    %s -> %s %s\n", simbolos[esq[p]], simbolos[dir[p][0]], simbolos[dir[p][1]]);
                        }
                    }
                }
            }
            if (mostrar) {
                printf("V[%d][%d] = ", r, s);
                imprime_celula(r, s);
                printf("\n\n");
            }
        }
    }

    tempo = 1000.0 * (clock() - inicio) / CLOCKS_PER_SEC;

    return celula(1, n)[0];
}

void recuo(int nivel) {
    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }
}

void imprime_arvore(int a, int r, int s, int nivel) {
    int nivel_filhos = nivel;

    if (!auxiliar[a]) {
        recuo(nivel);
        printf("%s\n", simbolos[a]);
        nivel_filhos = nivel + 1;
    }

    if (s == 1) {
        char *nome = simbolos[palavra[r-1]];
        recuo(nivel_filhos);
        if (strcmp(nome, "id") == 0 || strcmp(nome, "num_int") == 0 || strcmp(nome, "num_real") == 0 || strcmp(nome, "texto") == 0) {
            printf("%s  %s\n", nome, lexema[r-1]);
        } else {
            printf("%s\n", nome);
        }
        return;
    }

    for (int k = 1; k <= s - 1; k++) {
        for (int p = 0; p < nprod; p++) {
            if (esq[p] != a || tam[p] != 2) continue;
            if (celula(r, k)[dir[p][0]] && celula(r + k, s - k)[dir[p][1]]) {
                imprime_arvore(dir[p][0], r, k, nivel_filhos);
                imprime_arvore(dir[p][1], r + k, s - k, nivel_filhos);
                return;
            }
        }
    }
}
