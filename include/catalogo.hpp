#ifndef RPG_CATALOGO_HPP
#define RPG_CATALOGO_HPP

#include <istream>
#include <memory>
#include <string>
#include <vector>

#include "item.hpp"
#include "monstro.hpp"

/**
 * @file catalogo.hpp
 * @brief Catalogos de itens e monstros carregados dos arquivos de data/.
 *
 * Linhas vazias e linhas iniciadas por '#' sao ignoradas. Uma linha
 * malformada interrompe a carga com ArquivoInvalido indicando o numero
 * da linha.
 */

namespace rpg {

class Dado;

class CatalogoItens {
public:
    /**
     * @brief Carrega os itens a partir de um arquivo.
     * @throws ArquivoInvalido se o arquivo nao abrir ou tiver linha invalida.
     */
    void carregarArquivo(const std::string& caminho);

    /**
     * @brief Carrega os itens a partir de um fluxo de entrada ja aberto.
     * @throws ArquivoInvalido se alguma linha for invalida.
     */
    void carregar(std::istream& entrada, const std::string& origem);

    /** @brief Adiciona um item ao catalogo. */
    void adicionar(std::shared_ptr<Item> item);

    /**
     * @brief Devolve uma copia nova do item com esse nome.
     * @throws ItemNaoEncontrado
     */
    std::shared_ptr<Item> criar(const std::string& nome) const;

    /** @brief Indica se existe algum item com esse nome no catalogo. */
    bool contem(const std::string& nome) const;
    /** @brief Quantidade de itens cadastrados no catalogo. */
    std::size_t tamanho() const;
    /** @brief Lista de todos os itens do catalogo. */
    const std::vector<std::shared_ptr<Item>>& itens() const;
    /** @brief Lista os itens do catalogo filtrados por tipo. */
    std::vector<std::shared_ptr<Item>> porTipo(TipoItem tipo) const;

private:
    std::vector<std::shared_ptr<Item>> itens_;
};

class CatalogoMonstros {
public:
    /**
     * @brief Carrega os monstros a partir de um arquivo.
     * @throws ArquivoInvalido se o arquivo nao abrir ou tiver linha invalida.
     */
    void carregarArquivo(const std::string& caminho);

    /**
     * @brief Carrega os monstros a partir de um fluxo de entrada ja aberto.
     * @throws ArquivoInvalido se alguma linha for invalida.
     */
    void carregar(std::istream& entrada, const std::string& origem);

    /** @brief Adiciona um monstro ao catalogo. */
    void adicionar(const Monstro& monstro);

    /**
     * @brief Copia nova do monstro com esse nome, com vida e mana cheias.
     * @throws ItemNaoEncontrado
     */
    Monstro criar(const std::string& nome) const;

    /** @brief Indica se existe algum monstro com esse nome no catalogo. */
    bool contem(const std::string& nome) const;
    /** @brief Quantidade de monstros cadastrados no catalogo. */
    std::size_t tamanho() const;
    /** @brief Lista de todos os monstros do catalogo. */
    const std::vector<Monstro>& todos() const;
    /** @brief Lista os monstros com nivel ate o maximo informado. */
    std::vector<Monstro> ateNivel(int nivelMaximo) const;

    /**
     * @brief Sorteia um monstro de nivel <= nivelMaximo.
     *
     * Se nenhum couber no nivel, sorteia entre os mais fracos do catalogo.
     * @throws AcaoInvalida se o catalogo estiver vazio.
     */
    Monstro sortear(int nivelMaximo, const Dado& dado) const;

private:
    std::vector<Monstro> monstros_;
};

} // namespace rpg

#endif // RPG_CATALOGO_HPP
