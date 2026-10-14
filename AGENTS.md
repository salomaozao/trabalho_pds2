# Instruções para assistentes de código

Vale para qualquer assistente (Claude Code, Codex, Cursor, Gemini, Copilot) usado por um dos integrantes. Leia inteiro antes de mexer no repositório.

## 1. Antes de qualquer tarefa

1. Leia a última entrada de [.brain/dialogo_ias.md](.brain/dialogo_ias.md) (seção "Próxima IA / Handoff") e as pendências abertas em [.brain/pendencias.md](.brain/pendencias.md).
2. Descubra **quem é o operador** (`git config user.name`) e trabalhe só nos módulos dele (tabela da seção 2).
3. `git pull` antes de editar.

## 2. Situação do projeto (14/10/2026)

- Grupo: **Gabriel Salomão, Gabriel Warley e Sabrina Vitória**. André e Sidney saíram.
- Checkpoints C01 a C07 entregues. Faltam **C08 (27/10)**, **C09 (03/11)** e **C10 (apresentação em 10/11 ou 12/11)**.
- Estado do código: 105 testes passando, cobertura de linhas de 97,6%, sem warnings.
- **O maior risco é individual.** Quem não atingir **8 commits, 100 linhas e 4 dias distintos** (medidos por e-mail com `scripts/stats.sh`) tira **zero**, mesmo com o grupo indo bem. Hoje ninguém atingiu; a falta é de commits em dias diferentes.

| Integrante | E-mail (Moodle) | Módulos | Arquivos |
|---|---|---|---|
| Gabriel Salomão | gabrielbertolucci@ufmg.br | Combate e interface | `batalha`, `dado`, `jogo`, `main` |
| Gabriel Warley | gwal2024@ufmg.br | Entidades e relatórios | `personagem`, `heroi`, `monstro`, `relatorio` |
| Sabrina Vitória | sabrinavs@ufmg.br | Itens, loja e persistência | `item`, `inventario`, `loja`, `catalogo`, `persistencia` |

As tarefas de cada um, por data, estão em [.brain/pendencias/](.brain/pendencias).

## 3. Regras de commit (afetam a nota)

- Confira `git config user.email` antes do primeiro commit: tem que ser o e-mail da tabela. Commit com outro e-mail não conta.
- **Um commit por mudança real**, feito pelo próprio integrante. Não junte o trabalho de várias pessoas num commit só e não suba código de outro integrante no seu nome.
- Nunca `git push --force`, nunca squash merge, nunca reescrever histórico.
- Não faça commits triviais (espaço, vírgula) para inflar contagem: o enunciado diz que isso é detectado.
- Mensagem curta em português dizendo o que mudou. Sem `Co-Authored-By` e sem menção a assistente de código no código, nos comentários ou nas mensagens de commit.

## 4. Estilo de código

- Minimalista. Resolver o problema e parar. Sem abstrações "para o futuro".
- Comentário só quando o código não se explica.
- Doxygen: `@brief` de uma linha; `@param`/`@return`/`@throws` só quando acrescentam algo.
- Validação onde ela protege uma regra do jogo ou um arquivo. Erros viram exceções de `excecoes.hpp` e são tratados em `jogo.cpp` sem derrubar o programa.
- Sem `assert`, sem log de debug, sem `TODO`, sem `FIXME`.
- Herança/polimorfismo simples; sem templates próprios, sem `auto` em excesso.
- Nomes em português, **sem acento nos `.hpp`/`.cpp`** (ASCII puro). Markdown pode ter acento.
- Testes em doctest: um `TEST_CASE` por comportamento, poucos `SUBCASE`, sem testar getter trivial. O `DadoFixo` deixa o combate determinístico.
- C++11.

## 5. Estrutura e build

```
include/   .hpp (um por módulo)        src/    .cpp + main.cpp
tests/     doctest, um por módulo      data/   itens.txt, monstros.txt, saves/
design/    user stories, CRC, arquitetura.md
```

`make`, `make run`, `make test`, `make coverage`, `make docs` (no Windows, `mingw32-make`). Ao terminar: compilar e rodar `make test` uma vez. Cobertura mínima 60%.

## 6. Ao terminar um bloco de trabalho

Acrescente uma entrada ao final de `.brain/dialogo_ias.md` (modelo no próprio arquivo), atualize o `status` da pendência em `.brain/pendencias/` e rode `python .brain/scripts/manage_brain.py sync`. Commite isso junto com o trabalho.
