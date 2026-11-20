#define MAX_SIMB 400
#define MAX_PROD 1000
#define MAX_DIR 12

extern char simbolos[MAX_SIMB][30];
extern int terminal[MAX_SIMB];
extern int auxiliar[MAX_SIMB];
extern int nsimb;

extern int esq[MAX_PROD];
extern int tam[MAX_PROD];
extern int dir[MAX_PROD][MAX_DIR];
extern int nprod;

int busca_simbolo(char nome[]);
void le_gramatica(char arquivo[]);
void imprime_gramatica();
void tira_vazias(int mostrar);
void tira_unitarias(int mostrar);
void tira_inuteis(int mostrar);
void converte_fnc(int mostrar);
