#!/bin/sh
cd ../ENTREGA/src
echo "k,n,combinacoes,uteis,tempo_ms"
k=1
while [ $k -le 14 ]; do
    : > /tmp/grande.pas
    i=0
    while [ $i -lt $k ]; do
        cat ../testes/validos/01_teste_professor.pas >> /tmp/grande.pas
        echo >> /tmp/grande.pas
        i=$((i + 1))
    done
    ./main /tmp/grande.pas | grep combinacoes | sed -E "s/([0-9]+) tokens, ([0-9]+) combinacoes \(s, r, k\), ([0-9]+) com as duas celulas preenchidas, ([0-9.]+) ms/$k,\1,\2,\3,\4/"
    k=$((k + 1))
done
