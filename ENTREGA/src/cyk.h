#define MAX_TOKENS 2000

extern int palavra[MAX_TOKENS];
extern char lexema[MAX_TOKENS][100];
extern int ntokens;
extern long combinacoes;
extern long combinacoes_uteis;
extern double tempo;

int cyk(int mostrar);
void imprime_arvore(int a, int r, int s, int nivel);
