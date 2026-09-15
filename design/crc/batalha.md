# Batalha

Coordena um combate por turnos entre um herói e um monstro.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Conhecer os dois combatentes, o estado (em andamento, vitória, derrota, fuga) e a rodada atual | `Heroi` |
| Recusar começar se um dos combatentes já estiver morto | `Monstro` |
| Decidir quem age primeiro pela destreza | `Dado` |
| Validar a ação antes de iniciar a rodada (mana para a habilidade, item é poção), para o monstro não ganhar um golpe grátis | `Inventario` |
| Executar a ação escolhida pelo herói: ataque, habilidade, poção ou fuga | `PersonagemMorto`, `AcaoInvalida`, `RecursoInsuficiente` |
| Executar o ataque do monstro | |
| Resolver a tentativa de fuga com dado e destreza | |
| Detectar o fim da batalha e, na vitória, conceder experiência e ouro e informar quantos níveis o herói subiu | |
| Atualizar as estatísticas do herói: dano causado e recebido, vitórias, derrotas e fugas | |
| Manter um registro textual de tudo que aconteceu | |
