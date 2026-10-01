#ifndef RPG_RELATORIO_HPP
#define RPG_RELATORIO_HPP

#include <string>
#include <vector>

/**
 * @file relatorio.hpp
 * @brief Monta os textos exibidos no terminal. Nao imprime nada.
 */

namespace rpg {

class Batalha;
class CatalogoItens;
class Heroi;
class Inventario;
class Monstro;
class Personagem;

namespace relatorio {

std::string barraDeVida(const Personagem& personagem);
std::string barraDeMana(const Personagem& personagem);

/** @brief Nome, classe, nivel, recursos, atributos e equipamentos. */
std::string fichaDoHeroi(const Heroi& heroi);

std::string estatisticas(const Heroi& heroi);
std::string tabelaInventario(const Inventario& inventario);
std::string tabelaCatalogo(const CatalogoItens& catalogo);
std::string listaMonstros(const std::vector<Monstro>& monstros);

/** @brief Vida e mana dos dois lados, lado a lado. */
std::string painelDeCombate(const Batalha& batalha);

std::string resumoBatalha(const Batalha& batalha);

}  // namespace relatorio
}  // namespace rpg

#endif  // RPG_RELATORIO_HPP
