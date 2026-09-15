# Heroi (abstrata) — Guerreiro, Mago, Arqueiro

Personagem controlado pelo jogador. As subclasses definem o atributo primário,
a habilidade especial e a progressão por nível.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Acumular experiência e subir de nível quando atingir o necessário | `Personagem` |
| Conhecer o ouro e recusar gastos acima do saldo | `Inventario` |
| Manter o inventário de itens | `Arma`, `Armadura`, `Pocao` |
| Equipar e desequipar arma e armadura, devolvendo o anterior ao inventário | `Dado` |
| Compor o poder de ataque com o atributo primário mais o bônus da arma | `Estatisticas` |
| Compor a defesa total com o atributo de defesa mais o bônus da armadura | |
| Usar poções do inventário, consumindo-as | |
| Usar a habilidade da classe, cobrando a mana correspondente | |
| Registrar estatísticas de combate (vitórias, derrotas, fugas, dano) | |
| Reviver após derrota e restaurar estado a partir de um save | |
