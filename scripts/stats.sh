#!/bin/bash
# =============================================================================
#  Metricas de participacao individual (Figura 3 do enunciado).
#
#  Uso:
#    ./scripts/stats.sh <email_do_aluno>     mostra as metricas de um aluno
#    ./scripts/stats.sh                      mostra as metricas de todos os
#                                            autores que ja commitaram
#
#  O criterio da disciplina e binario: o aluno precisa atingir SIMULTANEAMENTE
#  o minimo de commits, de linhas efetivas e de dias ativos. Falhar em um
#  deles zera a nota individual, independentemente da nota do grupo.
# =============================================================================

MIN_COMMITS=8
MIN_LINHAS=100
MIN_DIAS=4

metricas_do_autor() {
    AUTOR="$1"

    COMMITS=$(git log --no-merges --author="$AUTOR" --oneline | wc -l)

    LINHAS=$(git log --no-merges --author="$AUTOR" \
        --pretty=tformat: --numstat \
        | awk '{ins+=$1; del+=$2} END {print ins+del+0}')

    DIAS=$(git log --no-merges --author="$AUTOR" \
        --pretty=format:"%ad" --date=short \
        | sort -u | wc -l)

    # Remove espacos que o wc -l adiciona em algumas plataformas.
    COMMITS=$(echo "$COMMITS" | tr -d ' ')
    DIAS=$(echo "$DIAS" | tr -d ' ')

    if [ "$COMMITS" -ge "$MIN_COMMITS" ] && \
       [ "$LINHAS" -ge "$MIN_LINHAS" ] && \
       [ "$DIAS" -ge "$MIN_DIAS" ]; then
        SITUACAO="OK"
    else
        SITUACAO="ABAIXO DO MINIMO"
    fi

    printf "%-38s commits: %3s/%s | linhas: %6s/%s | dias: %3s/%s | %s\n" \
        "$AUTOR" "$COMMITS" "$MIN_COMMITS" "$LINHAS" "$MIN_LINHAS" \
        "$DIAS" "$MIN_DIAS" "$SITUACAO"
}

if [ ! -d .git ]; then
    echo "Rode este script na raiz do repositorio."
    exit 1
fi

echo "Minimos exigidos: $MIN_COMMITS commits, $MIN_LINHAS linhas, $MIN_DIAS dias ativos"
echo "-------------------------------------------------------------------------------------------------"

if [ -n "$1" ]; then
    metricas_do_autor "$1"
else
    # Sem argumento, percorre todos os autores presentes no historico.
    git log --no-merges --pretty=format:"%ae" | sort -u | while read -r EMAIL; do
        [ -n "$EMAIL" ] && metricas_do_autor "$EMAIL"
    done
fi
