# Diálogo entre assistentes — TP de PDS II (RPG por turnos)

Log cronológico, só acrescentar no fim. Modelo de entrada:

```
---

## AAAA-MM-DD — tipo(escopo): resumo

**Autor:** <assistente> · operador: <integrante>

**Contexto:** por que foi feito.

**Feito:** arquivos alterados e comandos.

**Pendente / atenção:** o que ficou.

**Próxima IA / Handoff:** o que a próxima sessão deve fazer.
```

---

## 2026-10-14 — chore(brain): memória compartilhada e situação do grupo

**Autor:** Claude Opus 5.5 (Claude Code) · operador: Gabriel Salomão

**Contexto:** André e Sidney saíram; o grupo tem três integrantes e três checkpoints pela frente. O código base entrou na `main` em 01/10 num único commit grande, e o README daquela data estava incompleto (corrigido em 14/10). Ninguém atinge ainda a participação mínima.

**Feito:** `AGENTS.md` (regras e situação, lido por qualquer assistente), `CLAUDE.md` apontando para ele, e esta pasta `.brain/` com as pendências PEND-01 a PEND-06 e a decisão DEC-01.

**Pendente / atenção:**
- PEND-01 é a mais importante: cada integrante precisa de commits em dias diferentes até 03/11.
- Não subir trabalho de outro integrante no próprio nome. Não usar force push nem squash.
- Os commits antigos do Salomão usam e-mail pessoal; os novos usam `gabrielbertolucci@ufmg.br`.

**Próxima IA / Handoff:** identifique o operador pelo `git config user.name`, abra a pendência dele (PEND-02 nesta semana) e trabalhe só nos módulos dele. Ao final, registre uma entrada aqui e rode o `sync`.

---

## 2026-10-20 — test(persistencia): remover save mantendo o indice e save acima da capacidade

**Autor:** Claude · operador: Sabrina Vitória

**Contexto:** PEND-01 e PEND-02: dia extra de atividade na semana de 20/10, com testes novos de persistência.

**Feito:** `tests/teste_persistencia.cpp`, dois TEST_CASE novos, um commit cada: "remover um save mantem os outros no indice" e "ler rejeita save com mais itens que a capacidade do inventario". `make test`: 107 casos, 967 asserções, SUCCESS.

**Pendente / atenção:** o PEND-02 da Sabrina cita outros dois testes (salvar duas vezes; trocar o teste de getters por um de comportamento), mas o grupo redistribuiu e ela fez estes dois.

**Próxima IA / Handoff:** C08 da Sabrina (27/10): desconto na loja por nível do herói em `loja`, depois o teste em `teste_loja.cpp`. C09 (03/11): recusar arquivos de itens e monstros sem cadastro e testar.
