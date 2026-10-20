---
id: PEND-02
titulo: Testes novos na semana de 20/10 (dia extra de atividade)
modulo_afetado: [tests]
criticidade: alta
status: aberto
responsavel: Salomão, Warley, Sabrina
data_abertura: 2026-10-14
prazo: 2026-10-23
linear: PES-54
---

# PEND-02: Testes da semana de 20/10

Dois `TEST_CASE` por pessoa, um commit cada, cobrindo caminhos que nenhum teste exercitava (conferido pela cobertura em 14/10).

| Quem | Arquivo | Comportamento |
|---|---|---|
| Salomão | `teste_jogo.cpp` | Desequipar armadura pelo menu devolve ao inventário |
| Salomão | `teste_jogo.cpp` | Descansar sem ouro avisa e não restaura |
| Warley | `teste_heroi.cpp` | Trocar de armadura devolve a anterior ao inventário |
| Warley | `teste_heroi.cpp` | `restaurarEstado` ajusta máximos menores e recusa mana acima do máximo |
| Sabrina | `teste_persistencia.cpp` | Remover um save mantém os outros no índice |
| Sabrina | `teste_persistencia.cpp` | Save com mais itens que a capacidade é recusado |

## Status dos Integrantes
- [x] Salomão: concluído em 20/10 (commits b6804d2 e 60692a8)
- [x] Sabrina: concluído em 20/10 (commits 5a3ea30 e 2207ed6)
- [ ] Warley: pendente
