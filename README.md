# Trabalho Final de LFA – Reconhecedor de miniPascal com CYK

Reconhecedor da linguagem miniPascal (UDESC, Linguagens Formais e Autômatos, prof. Leandro Israel Pinto).

- **Léxico:** flex (`ENTREGA/src/pascal.l`), que gera um autômato finito determinístico.
- **Sintaxe:** a GLC do miniPascal (`ENTREGA/src/gramatica.txt`) é simplificada, convertida para a
  Forma Normal de Chomsky e reconhecida pelo algoritmo **CYK** (Cocke-Younger-Kasami).

## Pastas

| Pasta | Conteúdo |
|---|---|
| `ENTREGA/` | **tudo o que vai para o professor**: relatório em PDF, código e testes |
| `ENTREGA/src/` | código em C, gramática, `.bat` para compilar e rodar |
| `ENTREGA/testes/` | 3 programas válidos, 2 inválidos e o script de teste |
| `relatorio/` | fonte do relatório (`relatorio.tex`), dados e script do gráfico de desempenho |
| `base_professor/` | arquivos originais enviados pelo professor |

## Uso rápido (Windows)

```
cd ENTREGA\src
COMPILAR.bat
main.exe -a ..\testes\validos\02_enunciado.pas
main.exe -g -p exemplo.pas
cd ..\testes
rodar_testes.bat
```

Para regerar o PDF: `tectonic relatorio/relatorio.tex` (ou compilar no Overleaf) e copiar para
`ENTREGA/Relatorio-LFA.pdf`.
