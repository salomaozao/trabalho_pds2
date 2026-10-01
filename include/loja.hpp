#ifndef RPG_LOJA_HPP
#define RPG_LOJA_HPP

#include <memory>
#include <string>

/**
 * @file loja.hpp
 * @brief Compra e venda de itens usando o ouro do heroi.
 */

namespace rpg {

class CatalogoItens;
class Heroi;
class Item;

class Loja {
public:
    explicit Loja(const CatalogoItens& catalogo);

    const CatalogoItens& catalogo() const;

    /**
     * @brief Cobra o preco e coloca uma copia do item no inventario.
     * @throws ItemNaoEncontrado   se o item nao existir no catalogo.
     * @throws InventarioCheio     se nao houver espaco.
     * @throws RecursoInsuficiente se faltar ouro.
     */
    std::shared_ptr<Item> comprar(Heroi& heroi, const std::string& nome);

    /**
     * @brief Retira o item do inventario e paga metade do preco.
     * @return Ouro recebido.
     * @throws ItemNaoEncontrado
     */
    int vender(Heroi& heroi, const std::string& nome);

    static int precoDeVenda(const Item& item);

private:
    const CatalogoItens& catalogo_;
};

}  // namespace rpg

#endif  // RPG_LOJA_HPP
