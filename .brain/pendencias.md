# Sumário Executivo de Pendências e Backlog

Este arquivo é o inventário de pendências ativas, bloqueios e itens a monitorar no projeto **TP de PDS II: RPG por turnos (trabalho_pds2)**.

Para aprofundamento técnico, plano de ação e responsáveis de cada item, consulte os arquivos individuais no diretório [`pendencias/`](pendencias).

---

## Pendências Abertas / Em Andamento

| ID | Assunto / Pendência | Responsável | Criticidade | Status | Resumo e Ação Necessária | Detalhes |
| :--- | :--- | :--- | :---: | :---: | :--- | :---: |
| **[PEND-01](pendencias/01_participacao_minima.md)** | **Cada integrante atingir 8 commits, 100 linhas e 4 dias distintos** | Salomão, Warley, Sabrina | **Alta** | `Aberto` | Quem não atingir os três mínimos ao mesmo tempo tira zero no TP. Medir com bash scripts/stats.sh <email>. | [Abrir](pendencias/01_participacao_minima.md) |
| **[PEND-02](pendencias/02_testes_semana_20_10.md)** | **Testes novos na semana de 20/10 (dia extra de atividade)** | Salomão, Warley, Sabrina | **Alta** | `Aberto` | Dois TEST_CASE por pessoa, um commit cada, cobrindo caminhos que nenhum teste exercitava (conferido pela cobertura em 14/10). | [Abrir](pendencias/02_testes_semana_20_10.md) |
| **[PEND-03](pendencias/03_c08_funcionalidades.md)** | **C08 — uma funcionalidade nova por integrante** | Salomão, Warley, Sabrina | **Alta** | `Aberto` | Cada um faz 2 commits: código e teste. | [Abrir](pendencias/03_c08_funcionalidades.md) |
| **[PEND-04](pendencias/04_c09_validacoes.md)** | **C09 — uma validação nova por integrante, com exceção e teste** | Salomão, Warley, Sabrina | **Alta** | `Aberto` | Dois commits por pessoa: validação e teste. | [Abrir](pendencias/04_c09_validacoes.md) |
| **[PEND-05](pendencias/05_c10_apresentacao.md)** | **C10 — apresentação de 16 minutos e pergunta individual** | Salomão, Warley, Sabrina | **Alta** | `Aberto` | | Parte | Tempo | Quem | | [Abrir](pendencias/05_c10_apresentacao.md) |
| **[PEND-06](pendencias/06_perguntas_monitor.md)** | **Perguntar ao monitor sobre e-mail dos commits, multa e checkpoints atrasados** | Salomão | Media | `Aberto` | 1. Commits feitos com um e-mail diferente do Moodle contam para a participação? | [Abrir](pendencias/06_perguntas_monitor.md) |

---

## Pendências Concluídas / Resolvidas

| ID | Assunto | Conclusão | Data Resolução | Registro |
| :--- | :--- | :--- | :---: | :---: |


---

## Como Operar o Backlog
- Para **abrir uma nova pendência**: crie um arquivo `PEND-XX` em `pendencias/` e execute `python .brain/scripts/manage_brain.py sync`.
- Para **resolver uma pendência**: altere `status: resolvido` no frontmatter, adicione `data_resolucao: AAAA-MM-DD` e execute `python .brain/scripts/manage_brain.py sync`.
