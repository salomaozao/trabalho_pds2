---
id: PEND-04
titulo: C09 — uma validação nova por integrante, com exceção e teste
modulo_afetado: [src, tests]
criticidade: alta
status: aberto
responsavel: Salomão, Warley, Sabrina
data_abertura: 2026-10-14
prazo: 2026-11-03
---

# PEND-04: C09 (ter 03/11, 23:59)

Lançar a exceção certa de `excecoes.hpp`, tratar no menu sem o jogo fechar e provar com `CHECK_THROWS_AS`. Dois commits por pessoa: validação e teste.

| Quem | Onde procurar entrada sem validação |
|---|---|
| Salomão | Opção inválida nos menus de `jogo.cpp`; ação de combate com parâmetro vazio |
| Warley | Atributos negativos ao criar herói ou monstro; nível fora da faixa |
| Sabrina | Linha malformada em `itens.txt` ou no save; preço negativo; inventário cheio |
