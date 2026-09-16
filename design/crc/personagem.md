# Personagem (abstrata)

Base de tudo que entra em combate. Superclasse de `Heroi` e `Monstro`.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Conhecer nome, nível, vida/vida máxima e mana/mana máxima | `Atributos` |
| Conhecer os atributos (força, inteligência, destreza, defesa) | `Dado` |
| Saber se está vivo | `Combate` |
| Calcular o dano bruto de um ataque comum a partir do poder de ataque e de uma rolagem | |
| Receber dano descontando a defesa total, garantindo ao menos 1 de dano | |
| Curar e recuperar mana sem ultrapassar os máximos | |
| Gastar mana recusando quando não há o suficiente | |
| Delegar às subclasses o poder de ataque e a composição da defesa total | `Heroi`, `Monstro` |
| Produzir um resumo de uma linha para os menus | |
