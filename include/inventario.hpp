#ifndef RPG_INVENTARIO_HPP
#define RPG_INVENTARIO_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "item.hpp"

/**
 * @file inventario.hpp
 * @brief Colecao de itens com capacidade limitada.
 */

namespace rpg {

class Inventario {
public:
    /** @throws EntradaInvalida se a capacidade for zero. */
    explicit Inventario(std::size_t capacidade = 8);

    std::size_t capacidade() const;
    std::size_t tamanho() const;
    bool vazio() const;
    bool cheio() const;

    /**
     * @throws InventarioCheio  se nao houver espaco.
     * @throws EntradaInvalida  se o item for nulo.
     */
    void adicionar(std::shared_ptr<Item> item);

    /**
     * @brief Retira e devolve o primeiro item com esse nome (sem caixa).
     * @throws ItemNaoEncontrado
     */
    std::shared_ptr<Item> remover(const std::string& nome);

    /** @throws ItemNaoEncontrado */
    std::shared_ptr<Item> buscar(const std::string& nome) const;

    bool contem(const std::string& nome) const;
    std::size_t contar(TipoItem tipo) const;
    const std::vector<std::shared_ptr<Item> >& itens() const;
    void limpar();

private:
    std::size_t indiceDe(const std::string& nome) const;

    std::size_t capacidade_;
    std::vector<std::shared_ptr<Item> > itens_;
};

}  // namespace rpg

#endif  // RPG_INVENTARIO_HPP
