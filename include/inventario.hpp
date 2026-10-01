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
    /**
     * @brief Cria um inventario vazio com a capacidade informada.
     * @throws EntradaInvalida se a capacidade for zero.
     */
    explicit Inventario(std::size_t capacidade = 8);

    /** @brief Capacidade maxima de itens do inventario. */
    std::size_t capacidade() const;
    /** @brief Quantidade atual de itens guardados. */
    std::size_t tamanho() const;
    /** @brief Indica se o inventario nao tem nenhum item. */
    bool vazio() const;
    /** @brief Indica se o inventario atingiu a capacidade maxima. */
    bool cheio() const;

    /**
     * @brief Adiciona um item ao inventario.
     * @throws InventarioCheio se nao houver espaco.
     * @throws EntradaInvalida se o item for nulo.
     */
    void adicionar(std::shared_ptr<Item> item);

    /**
     * @brief Retira e devolve o primeiro item com esse nome (sem caixa).
     * @throws ItemNaoEncontrado
     */
    std::shared_ptr<Item> remover(const std::string& nome);

    /**
     * @brief Busca o primeiro item com esse nome, ignorando maiusculas e minusculas.
     * @throws ItemNaoEncontrado
     */
    std::shared_ptr<Item> buscar(const std::string& nome) const;

    /** @brief Indica se existe algum item com esse nome. */
    bool contem(const std::string& nome) const;
    /** @brief Conta quantos itens do tipo informado existem no inventario. */
    std::size_t contar(TipoItem tipo) const;
    /** @brief Lista de todos os itens guardados, para relatorios e saves. */
    const std::vector<std::shared_ptr<Item>>& itens() const;
    /** @brief Remove todos os itens do inventario. */
    void limpar();

private:
    std::size_t indiceDe(const std::string& nome) const;

    std::size_t capacidade_;
    std::vector<std::shared_ptr<Item>> itens_;
};

} // namespace rpg

#endif // RPG_INVENTARIO_HPP
