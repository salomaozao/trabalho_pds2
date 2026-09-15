# Item (abstrata) — Arma, Armadura, Pocao

Tudo que pode estar no inventário ou no catálogo da loja.

| Responsabilidades | Colaboradores |
|-------------------|---------------|
| Conhecer nome e preço | `Personagem` (alvo da poção) |
| Informar o tipo (arma, armadura ou poção) | `Inventario` |
| Descrever o efeito em texto para menus e relatórios | `CatalogoItens` |
| Clonar-se para que o catálogo entregue cópias independentes | |
| Serializar-se no formato dos arquivos de dados e dos saves | |
| Arma: conhecer o bônus de ataque | |
| Armadura: conhecer o bônus de defesa | |
| Poção: conhecer efeito (vida ou mana) e quantidade, e aplicá-lo em um personagem | |
| Ser construído a partir de uma linha de texto, recusando tipos ou campos inválidos | |
