---
id: PEND-02
titulo: Testes novos na semana de 20/10 (dia extra de atividade)
modulo_afetado: [tests]
criticidade: alta
status: aberto
responsavel: Salomão, Warley, Sabrina
data_abertura: 2026-10-14
prazo: 2026-10-23
---

# PEND-02: Testes da semana de 20/10

Dois `TEST_CASE` por pessoa, um commit cada. São lacunas reais (conferidas contra os testes existentes em 14/10).

| Quem | Arquivo | Comportamento |
|---|---|---|
| Salomão | `teste_batalha.cpp` | Executar rodada depois do fim da batalha é recusado |
| Salomão | `teste_batalha.cpp` | O registro ganha uma linha por ação da rodada |
| Warley | `teste_heroi.cpp` | Desequipar devolve o item ao inventário |
| Warley | `teste_relatorio.cpp` | Estatísticas de herói sem nenhuma batalha |
| Sabrina | `teste_persistencia.cpp` | Salvar duas vezes o mesmo herói sobrescreve o save |
| Sabrina | `teste_item.cpp` | Trocar o teste de getters por um teste de comportamento |

Antes de escrever, confira o comportamento real no `.cpp`; se ele não existir, o teste vira a especificação e a implementação entra no C08.
