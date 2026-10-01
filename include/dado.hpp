#ifndef RPG_DADO_HPP
#define RPG_DADO_HPP

#include <cstddef>
#include <random>
#include <vector>

/**
 * @file dado.hpp
 * @brief Rolagem de dados. O jogo usa DadoAleatorio; os testes usam DadoFixo.
 */

namespace rpg {

/** @brief Interface de um dado de N faces. */
class Dado {
public:
    virtual ~Dado();

    /**
     * @brief Rola um dado.
     * @return Valor entre 1 e lados.
     * @throws EntradaInvalida se lados < 1.
     */
    virtual int rolar(int lados) const = 0;

    /** @brief Soma de varias rolagens do mesmo dado. */
    int rolarVarios(int quantidade, int lados) const;
};

/** @brief Dado baseado no Mersenne Twister da biblioteca padrao. */
class DadoAleatorio : public Dado {
public:
    DadoAleatorio();
    explicit DadoAleatorio(unsigned int semente);

    int rolar(int lados) const override;

private:
    // rolar() e const para quem usa, mas o gerador muda de estado.
    mutable std::mt19937 gerador_;
};

/**
 * @brief Dado que devolve uma sequencia fixa de valores, ciclicamente.
 *
 * Valores maiores que o numero de faces sao limitados ao numero de faces.
 */
class DadoFixo : public Dado {
public:
    explicit DadoFixo(int valor);
    explicit DadoFixo(const std::vector<int>& sequencia);

    int rolar(int lados) const override;
    std::size_t rolagens() const;

private:
    std::vector<int> sequencia_;
    mutable std::size_t posicao_;
    mutable std::size_t rolagens_;
};

}  // namespace rpg

#endif  // RPG_DADO_HPP
