#ifndef RPG_HEROI_HPP
#define RPG_HEROI_HPP

#include <memory>
#include <string>
#include <vector>

#include "inventario.hpp"
#include "item.hpp"
#include "personagem.hpp"

/**
 * @file heroi.hpp
 * @brief Heroi controlado pelo jogador e suas tres classes.
 */

namespace rpg {

/** @brief Contadores acumulados ao longo das batalhas. */
struct Estatisticas {
    int vitorias;
    int derrotas;
    int fugas;
    int danoCausado;
    int danoRecebido;

    Estatisticas();
};

/**
 * @brief Personagem com experiencia, ouro, inventario e equipamentos.
 *
 * Cada classe define o atributo primario, a habilidade especial e como os
 * atributos crescem ao subir de nivel.
 */
class Heroi : public Personagem {
public:
    Heroi(const std::string& nome, int vidaMax, int manaMax,
          const Atributos& atributos);

    int experiencia() const;
    int experienciaParaProximoNivel() const;
    int ouro() const;
    const Inventario& inventario() const;
    Inventario& inventario();
    const Estatisticas& estatisticas() const;
    Estatisticas& estatisticas();
    std::shared_ptr<Arma> armaEquipada() const;
    std::shared_ptr<Armadura> armaduraEquipada() const;

    /**
     * @brief Acumula experiencia e sobe quantos niveis forem alcancados.
     * @return Quantos niveis foram ganhos.
     */
    int ganharExperiencia(int quantidade);

    void ganharOuro(int quantidade);

    /** @throws RecursoInsuficiente se faltar ouro. */
    void gastarOuro(int quantidade);

    /**
     * @brief Move um item do inventario para o slot de arma ou armadura.
     *
     * O equipamento anterior volta ao inventario.
     * @throws ItemNaoEncontrado se o item nao estiver no inventario.
     * @throws AcaoInvalida      se o item for uma pocao.
     */
    void equipar(const std::string& nome);

    /** @throws AcaoInvalida se nao houver arma equipada. */
    void desequiparArma();

    /** @throws AcaoInvalida se nao houver armadura equipada. */
    void desequiparArmadura();

    /**
     * @brief Consome uma pocao do inventario.
     * @return Quantidade recuperada.
     * @throws ItemNaoEncontrado
     * @throws AcaoInvalida se o item nao for uma pocao.
     */
    int usarPocao(const std::string& nome);

    /** @brief Gasta a mana da habilidade e devolve o dano bruto dela. */
    int usarHabilidade(const Dado& dado);

    /** @brief Metade da vida, mana cheia, metade do ouro. */
    void reviver();

    /** @brief Usado pela persistencia para recompor um heroi salvo. */
    void restaurarEstado(int nivel, int experiencia, int vida, int vidaMax,
                         int mana, int manaMax, const Atributos& atributos,
                         int ouro);

    int poderDeAtaque() const override;
    int defesaTotal() const override;

    virtual int atributoPrimario() const = 0;
    virtual std::string nomeHabilidade() const = 0;
    virtual int custoHabilidade() const = 0;

protected:
    virtual int calcularDanoHabilidade(const Dado& dado) const = 0;
    virtual void aoSubirNivel() = 0;

private:
    void subirNivel();

    int experiencia_;
    int ouro_;
    Inventario inventario_;
    Estatisticas estatisticas_;
    std::shared_ptr<Arma> arma_;
    std::shared_ptr<Armadura> armadura_;
};

/** @brief Muita vida e forca. Golpe Devastador: dobra o poder de ataque. */
class Guerreiro : public Heroi {
public:
    explicit Guerreiro(const std::string& nome);

    std::string classe() const override;
    int atributoPrimario() const override;
    std::string nomeHabilidade() const override;
    int custoHabilidade() const override;

protected:
    int calcularDanoHabilidade(const Dado& dado) const override;
    void aoSubirNivel() override;
};

/** @brief Pouca vida, muita mana. Bola de Fogo: dano alto com 2d10. */
class Mago : public Heroi {
public:
    explicit Mago(const std::string& nome);

    std::string classe() const override;
    int atributoPrimario() const override;
    std::string nomeHabilidade() const override;
    int custoHabilidade() const override;

protected:
    int calcularDanoHabilidade(const Dado& dado) const override;
    void aoSubirNivel() override;
};

/** @brief Equilibrado, alta destreza. Flecha Dupla: dois ataques comuns. */
class Arqueiro : public Heroi {
public:
    explicit Arqueiro(const std::string& nome);

    std::string classe() const override;
    int atributoPrimario() const override;
    std::string nomeHabilidade() const override;
    int custoHabilidade() const override;

protected:
    int calcularDanoHabilidade(const Dado& dado) const override;
    void aoSubirNivel() override;
};

/**
 * @brief Cria o heroi da classe pedida ("Guerreiro", "Mago" ou "Arqueiro").
 * @throws EntradaInvalida se a classe for desconhecida.
 */
std::unique_ptr<Heroi> criarHeroi(const std::string& classe,
                                  const std::string& nome);

std::vector<std::string> classesDisponiveis();

}  // namespace rpg

#endif  // RPG_HEROI_HPP
