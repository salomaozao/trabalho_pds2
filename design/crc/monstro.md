# Monstro

Inimigo controlado pelo jogo. Instâncias vêm do catálogo carregado de
`data/monstros.txt`.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Conhecer o nível e as recompensas de experiência e ouro | `Personagem` |
| Calcular o poder de ataque como o maior entre força e inteligência | `Dado` |
| Calcular o dano bruto com uma rolagem de d4 | `CatalogoMonstros` |
| Serializar-se no formato do arquivo de dados | |
| Ser construído a partir de uma linha do arquivo, validando os campos | |
