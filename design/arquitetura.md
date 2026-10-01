# Arquitetura

## Módulos

| Módulo | Arquivos | Papel |
|--------|----------|-------|
| Exceções | `excecoes.hpp/.cpp` | Hierarquia única derivada de `ErroDeJogo` |
| Utilitários | `utilitarios.hpp/.cpp` | Funções puras de texto e formatação |
| Dado | `dado.hpp/.cpp` | Rolagem injetável: `DadoAleatorio` no jogo, `DadoFixo` nos testes |
| Entidades | `personagem`, `heroi`, `monstro` | Herança: `Personagem` → `Heroi` → classes; `Personagem` → `Monstro` |
| Itens | `item`, `inventario` | Herança: `Item` → `Arma`, `Armadura`, `Pocao` |
| Catálogos | `catalogo` | Carga de `data/itens.txt` e `data/monstros.txt` |
| Combate | `batalha` | Rodadas, iniciativa, recompensas, registro |
| Economia | `loja` | Compra e venda |
| Persistência | `persistencia` | Saves em texto `chave;valor` + índice |
| Relatórios | `relatorio` | Monta strings; não imprime |
| Interface | `jogo`, `main` | Menus sobre `istream`/`ostream` |

## Hierarquia de classes

```
Personagem (abstrata)
├── Heroi (abstrata)
│   ├── Guerreiro   força       Golpe Devastador   2×ataque + d6
│   ├── Mago        inteligência Bola de Fogo      ataque + 2d10
│   └── Arqueiro    destreza    Flecha Dupla       2 × (ataque + d6)
└── Monstro         max(força, inteligência) + d4

Item (abstrata)
├── Arma        +ataque
├── Armadura    +defesa
└── Pocao       recupera vida ou mana

Dado (abstrata)
├── DadoAleatorio
└── DadoFixo
```

Pontos de polimorfismo usados de fato pelo código:

- `Personagem::poderDeAtaque()` e `defesaTotal()` — o herói soma equipamentos, o
  monstro não.
- `Personagem::calcularDano()` — monstro usa d4, o padrão usa d6.
- `Heroi::atributoPrimario()`, `calcularDanoHabilidade()`, `aoSubirNivel()` — uma
  implementação por classe.
- `Item::tipo()`, `descricao()`, `clonar()`, `camposExtras()` — uma por tipo de item.
- `Dado::rolar()` — troca aleatório por determinístico nos testes.

## Fluxo de uma partida

```
main ── carrega catálogos ──► Jogo::executar
                                 │
                        menuInicial ── novoHeroi / carregarHeroi (RepositorioSaves)
                                 │
                        menuPrincipal
                          ├─ fichaDoHeroi (relatorio)
                          ├─ menuInventario ── Heroi::equipar / usarPocao
                          ├─ menuLoja ── Loja::comprar / vender
                          ├─ batalhar ── CatalogoMonstros::sortear ── Batalha::executarRodada
                          ├─ descansar
                          └─ salvar ── RepositorioSaves::salvar
```

## Regras de combate

- Iniciativa: o herói age primeiro se sua destreza for maior ou igual à do monstro.
- Dano efetivo = dano bruto − defesa total do alvo, mínimo 1.
- Fuga: `d6 + destreza/4 ≥ 5`. Se falhar, o monstro ataca normalmente.
- Vitória: herói recebe XP e ouro do monstro; XP para o próximo nível = `nível × 50`.
  Ao subir de nível, vida e mana são restauradas.
- Derrota: o herói revive com metade da vida e metade do ouro.
- Habilidade ou poção inválida é recusada **antes** da rodada começar, para o
  monstro não atacar de graça.

## Formatos de arquivo

`data/itens.txt`

```
ARMA;Nome;Preco;BonusAtaque
ARMADURA;Nome;Preco;BonusDefesa
POCAO;Nome;Preco;VIDA|MANA;Quantidade
```

`data/monstros.txt`

```
Nome;Nivel;Vida;Mana;Forca;Inteligencia;Destreza;Defesa;XP;Ouro
```

`data/saves/<nome>.txt` — uma linha `chave;valor` por campo; `arma;`, `armadura;` e
`item;` carregam um item serializado no formato acima. `data/saves/indice.txt` lista
os nomes salvos, já que C++11 não lista diretórios de forma portável.

Em todos os arquivos, linhas vazias e linhas começando com `#` são ignoradas.

## Tratamento de erros

Toda falha prevista lança uma subclasse de `ErroDeJogo`:

| Exceção | Quando |
|---------|--------|
| `EntradaInvalida` | valor fora do domínio (nome vazio, número negativo, classe desconhecida) |
| `ArquivoInvalido` | arquivo não abre ou linha malformada; carrega caminho e número da linha |
| `ItemNaoEncontrado` | busca por nome falhou em inventário ou catálogo |
| `InventarioCheio` | adicionar sem vaga |
| `RecursoInsuficiente` | falta mana ou ouro |
| `PersonagemMorto` | morto tentando agir ou ser alvo |
| `AcaoInvalida` | ação sem sentido no estado atual (rodada após o fim, equipar poção) |

`Jogo` captura `ErroDeJogo` em cada menu, imprime `[!] mensagem` e continua. `main`
captura o que escapar e encerra com código de saída diferente de zero.

## Testabilidade

- `Dado` injetado por referência em `Heroi`, `Monstro`, `Batalha`, `CatalogoMonstros`
  e `Jogo`.
- `Jogo` recebe `std::istream&` e `std::ostream&`; os testes usam `istringstream` e
  `ostringstream`.
- Catálogos e saves têm versões que leem/escrevem em `std::istream`/`std::ostream`,
  além das que abrem arquivos.
