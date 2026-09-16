# User Stories

Papéis considerados: **jogador** (usa o jogo pelo terminal), **designer de conteúdo**
(edita os arquivos em `data/` sem mexer em código) e **desenvolvedor** (mantém e testa
o sistema).

---

## US01 — Criar um herói

**Como** jogador, **quero** criar um herói escolhendo nome e classe **para** começar
uma partida com um personagem que combine com meu estilo.

Critérios de aceitação:
- O jogador pode escolher entre as classes Guerreiro, Mago e Arqueiro.
- O jogo recusa a criação se o nome do herói estiver vazio.
- O herói criado começa no nível 1, com vida, mana e atributos iniciais definidos
  pela classe escolhida.
- Após a criação, o herói fica disponível para entrar em combate ou acessar a loja.

## US02 — Lutar contra monstros

**Como** jogador, **quero** enfrentar monstros em combate por turnos **para** ganhar
experiência e ouro.

Critérios de aceitação:
- A cada rodada posso escolher entre atacar, usar a habilidade da classe, usar uma
  poção ou fugir.
- Quem tem mais destreza age primeiro na rodada.
- O dano recebido desconta a defesa do alvo, mas todo golpe causa ao menos 1 de dano.
- Ao vencer, recebo a experiência e o ouro do monstro; ao perder, o herói revive com
  metade da vida e metade do ouro.
- O monstro sorteado tem nível menor ou igual ao do herói.

## US03 — Usar a habilidade da classe

**Como** jogador, **quero** que cada classe tenha uma habilidade especial com regra
de dano própria **para** que a escolha da classe faça diferença no combate.

Critérios de aceitação:
- Guerreiro, Mago e Arqueiro têm habilidades com nomes e fórmulas de dano distintas.
- A habilidade consome mana; sem mana suficiente, o jogo recusa a ação sem gastar a
  rodada.
- O poder de ataque de cada classe deriva de um atributo primário diferente
  (força, inteligência ou destreza).

## US04 — Gerenciar inventário e equipamentos

**Como** jogador, **quero** guardar itens, equipar armas e armaduras e beber poções
**para** fortalecer meu herói.

Critérios de aceitação:
- O inventário tem capacidade limitada e avisa quando está cheio.
- Equipar uma arma aumenta o ataque; equipar uma armadura aumenta a defesa.
- Trocar de equipamento devolve o anterior ao inventário.
- Poções não podem ser equipadas, só usadas; ao usar, o item é consumido.

## US05 — Comprar e vender na loja

**Como** jogador, **quero** gastar o ouro ganho nas batalhas **para** comprar
equipamentos melhores e vender o que não uso.

Critérios de aceitação:
- A loja lista todos os itens do catálogo com descrição e preço.
- Não posso comprar sem ouro suficiente nem com o inventário cheio.
- Vender um item paga metade do preço de compra.

## US06 — Salvar e continuar depois

**Como** jogador, **quero** salvar o progresso do herói e carregá-lo em outra sessão
**para** não perder o que conquistei.

Critérios de aceitação:
- O save guarda nível, experiência, vida, mana, atributos, ouro, equipamentos,
  inventário e estatísticas de combate.
- Ao iniciar o jogo, posso escolher entre os heróis salvos pelo nome.
- Um arquivo de save corrompido gera uma mensagem de erro clara, sem derrubar o jogo.

## US07 — Acompanhar o desempenho

**Como** jogador, **quero** ver um relatório com minhas estatísticas **para** saber
como estou me saindo.

Critérios de aceitação:
- A ficha mostra barras de vida e mana, atributos, ataque e defesa totais e
  equipamentos.
- O relatório mostra vitórias, derrotas, fugas, dano causado e recebido.
- A taxa de vitória é calculada quando há ao menos uma batalha.

## US08 — Ajustar o balanceamento sem recompilar

**Como** designer de conteúdo, **quero** cadastrar e ajustar itens e monstros em
arquivos de texto **para** balancear o jogo sem tocar no código.

Critérios de aceitação:
- Itens ficam em `data/itens.txt` e monstros em `data/monstros.txt`, um por linha,
  campos separados por `;`.
- Linhas vazias e linhas começando com `#` são ignoradas.
- Uma linha inválida interrompe a carga informando o arquivo e o número da linha.

## US09 — Testar as regras de forma determinística

**Como** desenvolvedor, **quero** que as regras de combate não dependam de números
aleatórios reais **para** escrever testes com valores exatos.

Critérios de aceitação:
- Toda rolagem passa por uma interface `Dado`, injetada nas classes que precisam dela.
- Existe uma implementação de dado com resultado fixo para os testes.
- O jogo inteiro pode ser dirigido por um fluxo de entrada em memória, sem terminal.
- 
