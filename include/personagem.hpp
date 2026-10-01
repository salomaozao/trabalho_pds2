#ifndef RPG_PERSONAGEM_HPP
#define RPG_PERSONAGEM_HPP

#include <string>

/**
 * @file personagem.hpp
 * @brief Classe base de herois e monstros.
 */

namespace rpg {

class Dado;

/** @brief Atributos numericos de um personagem. */
struct Atributos {
    int forca;         ///< Base do dano fisico.
    int inteligencia;  ///< Base do dano magico.
    int destreza;      ///< Base do dano a distancia e da iniciativa.
    int defesa;        ///< Reducao de todo dano recebido.

    Atributos();

    /** @throws EntradaInvalida se algum valor for negativo. */
    Atributos(int forca, int inteligencia, int destreza, int defesa);
};

/**
 * @brief Entidade que participa de combates: tem vida, mana, nivel e atributos.
 *
 * O poder de ataque e a defesa total sao virtuais porque cada tipo de
 * personagem os compoe de um jeito (atributo primario, equipamentos).
 */
class Personagem {
public:
    /**
     * @throws EntradaInvalida se o nome for vazio, a vida nao for positiva ou
     *         a mana for negativa.
     */
    Personagem(const std::string& nome, int vidaMax, int manaMax,
               const Atributos& atributos);
    virtual ~Personagem();

    const std::string& nome() const;
    int vida() const;
    int vidaMax() const;
    int mana() const;
    int manaMax() const;
    int nivel() const;
    const Atributos& atributos() const;
    bool estaVivo() const;

    virtual std::string classe() const = 0;
    virtual int poderDeAtaque() const = 0;

    /** @brief Defesa efetiva. Por padrao, apenas o atributo de defesa. */
    virtual int defesaTotal() const;

    /**
     * @brief Dano bruto de um ataque comum: poder de ataque + d6.
     * @throws PersonagemMorto se o personagem estiver morto.
     */
    virtual int calcularDano(const Dado& dado) const;

    /**
     * @brief Aplica dano descontando a defesa. Todo golpe causa ao menos 1.
     * @return Dano efetivamente subtraido da vida.
     * @throws EntradaInvalida se o dano for negativo.
     * @throws PersonagemMorto se o personagem ja estiver morto.
     */
    int receberDano(int danoBruto);

    /**
     * @brief Recupera vida sem passar do maximo.
     * @return Quantidade efetivamente curada.
     * @throws PersonagemMorto se o personagem estiver morto.
     */
    int curar(int quantidade);

    /** @throws RecursoInsuficiente se nao houver mana suficiente. */
    void gastarMana(int quantidade);

    /** @return Quantidade efetivamente recuperada. */
    int recuperarMana(int quantidade);

    /** @brief Enche vida e mana. Funciona mesmo com o personagem morto. */
    void restaurarCompletamente();

    /** @brief Linha curta: nome, classe, nivel, vida e mana. */
    std::string resumo() const;

protected:
    void definirNivel(int nivel);
    void definirVida(int vida);
    void definirMana(int mana);
    void definirVidaMax(int vidaMax);
    void definirManaMax(int manaMax);
    void definirAtributos(const Atributos& atributos);
    void aumentarAtributos(int forca, int inteligencia, int destreza,
                           int defesa);

private:
    std::string nome_;
    int vidaMax_;
    int vida_;
    int manaMax_;
    int mana_;
    int nivel_;
    Atributos atributos_;
};

}  // namespace rpg

#endif  // RPG_PERSONAGEM_HPP
