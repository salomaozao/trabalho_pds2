#ifndef RPG_ITEM_HPP
#define RPG_ITEM_HPP

#include <memory>
#include <string>
#include <vector>

/**
 * @file item.hpp
 * @brief Itens do jogo: armas, armaduras e pocoes.
 *
 * Formato de uma linha em data/itens.txt e nos saves:
 *   ARMA;Nome;Preco;BonusAtaque
 *   ARMADURA;Nome;Preco;BonusDefesa
 *   POCAO;Nome;Preco;VIDA|MANA;Quantidade
 */

namespace rpg {

class Personagem;

enum class TipoItem { ARMA, ARMADURA, POCAO };

/** @brief Nome legivel do tipo de item, usado em relatorios e mensagens. */
std::string nomeDoTipo(TipoItem tipo);

/** @brief Item generico com nome e preco de compra. */
class Item {
public:
    /** @throws EntradaInvalida se o nome for vazio ou o preco negativo. */
    Item(const std::string& nome, int preco);
    virtual ~Item();

    /** @brief Nome do item. */
    const std::string& nome() const;
    /** @brief Preco de compra do item. */
    int preco() const;

    /** @brief Tipo concreto do item (arma, armadura ou pocao). */
    virtual TipoItem tipo() const = 0;
    /** @brief Descricao textual do item para exibicao. */
    virtual std::string descricao() const = 0;
    /** @brief Cria uma copia independente deste item. */
    virtual std::shared_ptr<Item> clonar() const = 0;

    /** @brief Linha no formato dos arquivos de dados. */
    std::string serializar() const;

protected:
    /** @brief Campos apos o preco na serializacao, ja com ';' na frente. */
    virtual std::string camposExtras() const = 0;

private:
    std::string nome_;
    int preco_;
};

class Arma : public Item {
public:
    /** @throws EntradaInvalida se o bonus for negativo. */
    Arma(const std::string& nome, int preco, int bonusAtaque);

    /** @brief Bonus de ataque concedido pela arma. */
    int bonusAtaque() const;
    TipoItem tipo() const override;
    std::string descricao() const override;
    std::shared_ptr<Item> clonar() const override;

protected:
    std::string camposExtras() const override;

private:
    int bonusAtaque_;
};

class Armadura : public Item {
public:
    /** @throws EntradaInvalida se o bonus for negativo. */
    Armadura(const std::string& nome, int preco, int bonusDefesa);

    /** @brief Bonus de defesa concedido pela armadura. */
    int bonusDefesa() const;
    TipoItem tipo() const override;
    std::string descricao() const override;
    std::shared_ptr<Item> clonar() const override;

protected:
    std::string camposExtras() const override;

private:
    int bonusDefesa_;
};

enum class EfeitoPocao { VIDA, MANA };

/** @brief Nome legivel do efeito da pocao, usado em relatorios e mensagens. */
std::string nomeDoEfeito(EfeitoPocao efeito);

/** @brief Consumivel que recupera vida ou mana. */
class Pocao : public Item {
public:
    /** @throws EntradaInvalida se a quantidade nao for positiva. */
    Pocao(const std::string& nome, int preco, EfeitoPocao efeito,
          int quantidade);

    /** @brief Efeito que a pocao aplica ao ser usada (vida ou mana). */
    EfeitoPocao efeito() const;
    /** @brief Quantidade de vida ou mana recuperada ao usar a pocao. */
    int quantidade() const;

    /**
     * @brief Aplica o efeito no alvo.
     * @return Quantidade efetivamente recuperada.
     */
    int aplicar(Personagem& alvo) const;

    TipoItem tipo() const override;
    std::string descricao() const override;
    std::shared_ptr<Item> clonar() const override;

protected:
    std::string camposExtras() const override;

private:
    EfeitoPocao efeito_;
    int quantidade_;
};

/**
 * @brief Monta um item a partir dos campos de uma linha ja dividida.
 * @throws EntradaInvalida se o tipo for desconhecido ou faltarem campos.
 */
std::shared_ptr<Item> criarItem(const std::vector<std::string>& campos);

/** @brief Divide a linha em ';' e chama criarItem. */
std::shared_ptr<Item> criarItemDeLinha(const std::string& linha);

} // namespace rpg

#endif // RPG_ITEM_HPP
