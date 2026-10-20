---
id: PEND-04
titulo: C09 — uma validação nova por integrante, com exceção e teste
modulo_afetado: [src, tests]
criticidade: alta
status: aberto
responsavel: Salomão, Warley, Sabrina
data_abertura: 2026-10-14
prazo: 2026-11-03
linear: PES-56
---

# PEND-04: C09 (ter 03/11, 23:59)

Dois commits por pessoa: validação e teste.

| Quem | Validação |
|---|---|
| Salomão | Descansar com vida e mana cheias é recusado (`AcaoInvalida`) e não cobra ouro |
| Warley | `restaurarEstado` recusa experiência maior ou igual à necessária para o próximo nível |
| Sabrina | `itens.txt`/`monstros.txt` sem nenhum cadastro geram `ArquivoInvalido` ao carregar |
