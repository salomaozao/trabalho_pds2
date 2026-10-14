# RPG por Turnos

Projeto final da disciplina DCC204 (Programação e Desenvolvimento de Software II),
UFMG, 2º semestre de 2026. É um motor simplificado de RPG por turnos escrito em
C++11, com interface de terminal e dados salvos em arquivos de texto.

Repositório: https://github.com/salomaozao/trabalho_pds2

## Integrantes

| Nome | Matrícula | E-mail | Módulos |
|------|-----------|--------|---------|
| Gabriel Salomão Bertolucci Roncoletta Nascimento | 2023100504 | gabrielbertolucci@ufmg.br | Combate e interface |
| Gabriel Warlei Avelino de Lima | 2024008695 | gwal2024@ufmg.br | Entidades e relatórios |
| Sabrina Vitória Silva | 2024097833 | sabrinavs@ufmg.br | Itens, loja e persistência |

## O problema

O sistema simula as partidas de um RPG de mesa: o jogador cria um herói, escolhe
uma classe, equipa itens, enfrenta monstros em combates por turnos e evolui de
nível. O progresso fica salvo em arquivo de texto e pode ser retomado depois.

Escolhemos esse tema por dois motivos.

O primeiro é que a herança aparece por necessidade do domínio. Guerreiro, mago e
arqueiro compartilham vida, mana e nível, mas cada um calcula o poder de ataque a
partir de um atributo diferente e tem uma habilidade especial própria. Não é um
cadastro com uma hierarquia colada por cima só para cumprir requisito.

O segundo é que as regras do jogo são funções puras e, por isso, dão para testar
de verdade. O cálculo de dano, a progressão de nível e a resolução de uma rodada
não dependem de tela nem de arquivo, só do estado dos personagens e de uma
rolagem de dado. Como o dado é uma abstração injetável (a classe `Dado`, com as
implementações `DadoAleatorio` e `DadoFixo`), o combate fica determinístico nos
testes e conseguimos verificar valores exatos.

## Funcionalidades

- Três classes de herói (Guerreiro, Mago, Arqueiro), cada uma com atributo
  primário, habilidade especial e progressão de nível próprios.
- Combate por turnos com iniciativa por destreza, defesa, habilidades, poções e
  fuga. Monstros sorteados de acordo com o nível do herói.
- Inventário com capacidade limitada; armas e armaduras equipáveis; poções de vida
  e mana.
- Loja para comprar e vender itens com o ouro ganho nas batalhas.
- Descanso na taverna para recuperar vida e mana.
- Saves em texto com nível, atributos, ouro, equipamentos, inventário e
  estatísticas de combate.
- Ficha do herói com barras de vida/mana e relatório de vitórias, derrotas,
  fugas e dano.
- Itens e monstros definidos em `data/itens.txt` e `data/monstros.txt`, editáveis
  sem recompilar.

## Como compilar e executar

Requisitos: g++ com suporte a C++11, GNU Make e, para o relatório de cobertura,
Python 3 com o gcovr. A documentação usa o Doxygen.

```bash
make            # compila o jogo em build/rpg
make run        # compila e executa o jogo
make test       # compila e roda os testes de unidade (doctest)
make coverage   # recompila instrumentado e gera o relatório do gcovr
make docs       # gera a documentação Doxygen em docs/html/index.html
make clean      # apaga binários, objetos e arquivos de cobertura
make deps       # baixa o doctest.h e instala o gcovr
```

No Windows, use `mingw32-make` no lugar de `make`. O jogo deve ser executado a
partir da raiz do repositório, porque ele procura os arquivos em `data/`.

## Como jogar

```
1. Novo heroi          escolhe nome e classe; começa com 50 de ouro e uma poção
2. Carregar heroi      lista os saves em data/saves/

Menu principal
1. Ver ficha           atributos, equipamentos e estatísticas
2. Inventario          equipar, desequipar e usar poções
3. Loja                comprar e vender
4. Procurar batalha    sorteia um monstro até o nível do herói + 1
5. Descansar           restaura vida e mana por 5 × nível de ouro
6. Salvar
7. Voltar
```

Em combate: atacar, usar a habilidade da classe (gasta mana), usar uma poção ou
tentar fugir. Ao vencer, o herói ganha experiência e ouro; ao perder, acorda na
taverna com metade da vida e metade do ouro.

## Estrutura do repositório

```
trabalho_pds2/
├── Makefile                 build, testes, cobertura e docs
├── Doxyfile                 configuração da documentação
├── build/                   binários e objetos (não versionado)
├── data/
│   ├── itens.txt            catálogo da loja
│   ├── monstros.txt         bestiário
│   └── saves/               heróis salvos (não versionado)
├── design/
│   ├── user_stories.md      requisitos
│   ├── arquitetura.md       módulos, hierarquia, regras e formatos
│   └── crc/                 um cartão CRC por classe central
├── docs/                    enunciado e documentação gerada
├── include/                 cabeçalhos .hpp com documentação Doxygen
├── scripts/stats.sh         métricas de participação individual no Git
├── src/                     implementações .cpp
└── tests/                   testes de unidade em doctest, um arquivo por módulo
```

Visão geral dos módulos, hierarquia de classes, regras de combate e formatos de
arquivo estão em [design/arquitetura.md](design/arquitetura.md).

## Testes e cobertura

Os testes usam o doctest e ficam em `tests/`, um arquivo por módulo. A classe
`Jogo` é testada de ponta a ponta alimentando os menus com um `istringstream` e
lendo a saída de um `ostringstream`. O `DadoFixo` deixa as batalhas
determinísticas, então os testes conferem valores exatos de dano, vida e ouro.

`make coverage` roda a suíte instrumentada e gera `build/cobertura.html`. A
cobertura de linhas de `src/` e `include/` fica acima de 95%
(97,6% em 14/10/2026).

## Decisões técnicas

**Código-fonte sem acentuação.** Os arquivos `.hpp` e `.cpp`, incluindo
comentários e strings, usam apenas ASCII. O grupo desenvolve em Windows e o
trabalho pode ser corrigido em Linux. Caracteres acentuados dentro de literais de
string saem corrompidos no terminal do Windows dependendo da página de código
ativa. Os arquivos Markdown, que não passam pelo terminal, usam acentuação
normal.

**Dado injetável.** Nenhuma classe chama o gerador aleatório diretamente. Quem
precisa de sorte recebe um `const Dado&`. O jogo passa um `DadoAleatorio`; os
testes passam um `DadoFixo`.

**Interface sobre fluxos.** `Jogo` recebe `std::istream&` e `std::ostream&` no
construtor em vez de usar `std::cin`/`std::cout` diretamente. Isso é o que permite
testar os menus.

**Equipamentos fora do inventário.** Arma e armadura equipadas ficam em slots
próprios do herói, não ocupam vaga no inventário. Trocar de equipamento devolve o
anterior ao inventário, que sempre tem vaga porque acabou de liberar uma.

**Índice de saves.** C++11 não tem uma forma portável de listar um diretório, então
`data/saves/indice.txt` guarda os nomes dos heróis salvos.

**Link estático do runtime do GCC no Windows.** O `Makefile` acrescenta
`-static-libgcc -static-libstdc++` quando detecta Windows. Sem isso, uma
`libstdc++-6.dll` antiga que esteja no `PATH` (a que vem junto com o Git Bash,
por exemplo) pode ser carregada no lugar da do compilador e derrubar o
executável com `STATUS_ENTRYPOINT_NOT_FOUND` antes de a `main` rodar.

**`doctest.h` versionado no repositório.** O cabeçalho do framework de testes
(versão 2.4.11) está commitado em `tests/`. É um arquivo único, e versioná-lo
garante que qualquer integrante rode `make test` sem depender de rede.

**Quebras de linha normalizadas em LF.** O `.gitattributes` força LF dentro do
repositório. Sem isso, integrantes em sistemas diferentes gerariam diffs de
arquivo inteiro a cada commit, o que atrapalharia a contagem de linhas usada na
avaliação de participação.
