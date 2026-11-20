#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gramatica.h"

char simbolos[MAX_SIMB][30];
int terminal[MAX_SIMB];
int auxiliar[MAX_SIMB];
int nsimb = 0;

int esq[MAX_PROD];
int tam[MAX_PROD];
int dir[MAX_PROD][MAX_DIR];
int nprod = 0;

int esq2[MAX_PROD];
int tam2[MAX_PROD];
int dir2[MAX_PROD][MAX_DIR];
int nprod2 = 0;

int fecho[MAX_SIMB][MAX_SIMB];
int ncascata = 0;

int busca_simbolo(char nome[]) {
    for (int i = 0; i < nsimb; i++) {
        if (strcmp(simbolos[i], nome) == 0) return i;
    }

    if (nsimb == MAX_SIMB) {
        printf("Simbolos demais na gramatica\n");
        exit(2);
    }

    strcpy(simbolos[nsimb], nome);
    terminal[nsimb] = !(nome[0] >= 'A' && nome[0] <= 'Z');
    auxiliar[nsimb] = 0;
    nsimb++;
    return nsimb - 1;
}

void add_producao(int a, int d[], int n) {
    for (int p = 0; p < nprod; p++) {
        if (esq[p] != a || tam[p] != n) continue;
        int igual = 1;
        for (int j = 0; j < n; j++) {
            if (dir[p][j] != d[j]) igual = 0;
        }
        if (igual) return;
    }

    if (nprod == MAX_PROD) {
        printf("Producoes demais na gramatica\n");
        exit(2);
    }

    esq[nprod] = a;
    tam[nprod] = n;
    for (int j = 0; j < n; j++) {
        dir[nprod][j] = d[j];
    }
    nprod++;
}

void copia_producoes() {
    nprod2 = nprod;
    for (int p = 0; p < nprod; p++) {
        esq2[p] = esq[p];
        tam2[p] = tam[p];
        for (int j = 0; j < tam[p]; j++) {
            dir2[p][j] = dir[p][j];
        }
    }
    nprod = 0;
}

void le_gramatica(char arquivo[]) {
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) {
        printf("Nao foi possivel abrir %s\n", arquivo);
        exit(2);
    }

    char linha[500];
    int a = -1;

    while (fgets(linha, 500, f) != NULL) {
        char *palavra = strtok(linha, " \t\r\n");
        if (palavra == NULL) continue;

        if (strcmp(palavra, "|") != 0) {
            a = busca_simbolo(palavra);
            strtok(NULL, " \t\r\n");
        }

        int d[MAX_DIR];
        int n = 0;

        palavra = strtok(NULL, " \t\r\n");
        while (palavra != NULL) {
            if (strcmp(palavra, "|") == 0) {
                add_producao(a, d, n);
                n = 0;
            } else if (strcmp(palavra, "&") != 0) {
                d[n] = busca_simbolo(palavra);
                n++;
            }
            palavra = strtok(NULL, " \t\r\n");
        }
        add_producao(a, d, n);
    }

    fclose(f);
}

int conta_variaveis() {
    int cont = 0;
    for (int a = 0; a < nsimb; a++) {
        for (int p = 0; p < nprod; p++) {
            if (esq[p] == a) {
                cont++;
                break;
            }
        }
    }
    return cont;
}

void imprime_gramatica() {
    for (int a = 0; a < nsimb; a++) {
        int primeira = 1;
        for (int p = 0; p < nprod; p++) {
            if (esq[p] != a) continue;

            if (primeira) printf("  %s ->", simbolos[a]);
            else printf(" |");
            primeira = 0;

            for (int j = 0; j < tam[p]; j++) {
                printf(" %s", simbolos[dir[p][j]]);
            }
            if (tam[p] == 0) printf(" &");
        }
        if (!primeira) printf("\n");
    }
    printf("  (%d variaveis, %d producoes)\n\n", conta_variaveis(), nprod);
}

void imprime_conjunto(int marca[]) {
    int primeiro = 1;
    printf("{");
    for (int i = 0; i < nsimb; i++) {
        if (marca[i]) {
            if (!primeiro) printf(", ");
            printf("%s", simbolos[i]);
            primeiro = 0;
        }
    }
    printf("}");
}

void tira_vazias(int mostrar) {
    int anulavel[MAX_SIMB] = {0};
    int mudou = 1;

    while (mudou) {
        mudou = 0;
        for (int p = 0; p < nprod; p++) {
            if (anulavel[esq[p]]) continue;
            int todos = 1;
            for (int j = 0; j < tam[p]; j++) {
                if (!anulavel[dir[p][j]]) todos = 0;
            }
            if (todos) {
                anulavel[esq[p]] = 1;
                mudou = 1;
            }
        }
    }

    copia_producoes();

    for (int p = 0; p < nprod2; p++) {
        if (tam2[p] == 0) continue;

        int pos[MAX_DIR];
        int k = 0;
        for (int j = 0; j < tam2[p]; j++) {
            if (anulavel[dir2[p][j]]) {
                pos[k] = j;
                k++;
            }
        }

        for (int mascara = 0; mascara < (1 << k); mascara++) {
            int d[MAX_DIR];
            int n = 0;
            for (int j = 0; j < tam2[p]; j++) {
                int tira = 0;
                for (int b = 0; b < k; b++) {
                    if (pos[b] == j && (mascara & (1 << b))) tira = 1;
                }
                if (!tira) {
                    d[n] = dir2[p][j];
                    n++;
                }
            }
            if (n > 0) add_producao(esq2[p], d, n);
        }
    }

    if (anulavel[0]) add_producao(0, NULL, 0);

    if (mostrar) {
        printf("PASSO 1 - EXCLUSAO DAS PRODUCOES VAZIAS\n");
        printf("  Ve = ");
        imprime_conjunto(anulavel);
        printf("\n");
        imprime_gramatica();
    }
}

void tira_unitarias(int mostrar) {
    for (int a = 0; a < nsimb; a++) {
        for (int b = 0; b < nsimb; b++) {
            fecho[a][b] = 0;
        }
    }

    for (int p = 0; p < nprod; p++) {
        if (tam[p] == 1 && !terminal[dir[p][0]]) fecho[esq[p]][dir[p][0]] = 1;
    }

    for (int k = 0; k < nsimb; k++) {
        for (int a = 0; a < nsimb; a++) {
            for (int b = 0; b < nsimb; b++) {
                if (fecho[a][k] && fecho[k][b]) fecho[a][b] = 1;
            }
        }
    }

    for (int a = 0; a < nsimb; a++) {
        fecho[a][a] = 0;
    }

    copia_producoes();

    for (int p = 0; p < nprod2; p++) {
        if (tam2[p] == 1 && !terminal[dir2[p][0]]) continue;
        add_producao(esq2[p], dir2[p], tam2[p]);
    }

    for (int a = 0; a < nsimb; a++) {
        for (int b = 0; b < nsimb; b++) {
            if (!fecho[a][b]) continue;
            for (int p = 0; p < nprod2; p++) {
                if (esq2[p] == b && !(tam2[p] == 1 && !terminal[dir2[p][0]])) {
                    add_producao(a, dir2[p], tam2[p]);
                }
            }
        }
    }

    if (mostrar) {
        printf("PASSO 2 - EXCLUSAO DAS PRODUCOES A -> B\n");
        for (int a = 0; a < nsimb; a++) {
            int tem = 0;
            for (int b = 0; b < nsimb; b++) {
                if (fecho[a][b]) tem = 1;
            }
            if (tem) {
                printf("  FECHO-%s = ", simbolos[a]);
                imprime_conjunto(fecho[a]);
                printf("\n");
            }
        }
        imprime_gramatica();
    }
}

void tira_inuteis(int mostrar) {
    int gera[MAX_SIMB] = {0};
    int atinge[MAX_SIMB] = {0};
    int tinha[MAX_SIMB] = {0};

    for (int i = 0; i < nsimb; i++) {
        if (terminal[i]) gera[i] = 1;
    }
    for (int p = 0; p < nprod; p++) {
        tinha[esq[p]] = 1;
    }

    int mudou = 1;
    while (mudou) {
        mudou = 0;
        for (int p = 0; p < nprod; p++) {
            if (gera[esq[p]]) continue;
            int todos = 1;
            for (int j = 0; j < tam[p]; j++) {
                if (!gera[dir[p][j]]) todos = 0;
            }
            if (todos) {
                gera[esq[p]] = 1;
                mudou = 1;
            }
        }
    }

    atinge[0] = 1;
    mudou = 1;
    while (mudou) {
        mudou = 0;
        for (int p = 0; p < nprod; p++) {
            if (!atinge[esq[p]]) continue;
            for (int j = 0; j < tam[p]; j++) {
                if (!atinge[dir[p][j]]) {
                    atinge[dir[p][j]] = 1;
                    mudou = 1;
                }
            }
        }
    }

    copia_producoes();

    for (int p = 0; p < nprod2; p++) {
        int ok = gera[esq2[p]] && atinge[esq2[p]];
        for (int j = 0; j < tam2[p]; j++) {
            if (!gera[dir2[p][j]]) ok = 0;
        }
        if (ok) add_producao(esq2[p], dir2[p], tam2[p]);
    }

    if (mostrar) {
        int tirou[MAX_SIMB] = {0};
        for (int i = 0; i < nsimb; i++) {
            if (tinha[i] && !(gera[i] && atinge[i])) tirou[i] = 1;
        }
        printf("PASSO 3 - EXCLUSAO DOS SIMBOLOS INUTEIS\n");
        printf("  variaveis retiradas = ");
        imprime_conjunto(tirou);
        printf("\n");
        imprime_gramatica();
    }
}

int variavel_do_terminal(int t) {
    char nome[30];
    sprintf(nome, "T(%s)", simbolos[t]);

    int antes = nsimb;
    int v = busca_simbolo(nome);
    if (nsimb > antes) {
        auxiliar[v] = 1;
        int d[1] = {t};
        add_producao(v, d, 1);
    }
    return v;
}

int acha_ou_cria(int b, int c) {
    for (int p = 0; p < nprod; p++) {
        if (auxiliar[esq[p]] && tam[p] == 2 && dir[p][0] == b && dir[p][1] == c) return esq[p];
    }

    char nome[30];
    ncascata++;
    sprintf(nome, "C%d", ncascata);

    int v = busca_simbolo(nome);
    auxiliar[v] = 1;
    int d[2] = {b, c};
    add_producao(v, d, 2);
    return v;
}

void converte_fnc(int mostrar) {
    copia_producoes();

    for (int p = 0; p < nprod2; p++) {
        int d[MAX_DIR];
        int n = tam2[p];

        for (int j = 0; j < n; j++) {
            d[j] = dir2[p][j];
            if (n >= 2 && terminal[d[j]]) d[j] = variavel_do_terminal(d[j]);
        }

        while (n > 2) {
            d[n-2] = acha_ou_cria(d[n-2], d[n-1]);
            n--;
        }

        add_producao(esq2[p], d, n);
    }

    if (mostrar) {
        printf("PASSO 4 - FORMA NORMAL DE CHOMSKY\n");
        imprime_gramatica();
    }
}
