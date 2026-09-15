# Jogo

Menus do terminal e ciclo principal. Lê de um `istream` e escreve em um
`ostream` recebidos no construtor, o que permite testar com fluxos em memória.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Exibir o menu inicial: criar herói, carregar herói ou sair | `Heroi` |
| Exibir o menu principal: ficha, inventário, loja, batalha, descanso, salvar | `Batalha` |
| Conduzir a criação do herói (nome, classe, kit inicial) | `Loja` |
| Conduzir uma batalha rodada a rodada, mostrando o painel e o registro | `CatalogoItens`, `CatalogoMonstros` |
| Reviver o herói após uma derrota | `RepositorioSaves` |
| Ler opções numéricas com validação e repetição em caso de erro | `relatorio` |
| Encerrar de forma limpa quando a entrada acaba | `Dado` |
| Capturar `ErroDeJogo` em todos os menus e exibir a mensagem sem derrubar o programa | |
