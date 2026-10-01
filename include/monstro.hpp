#ifndef RPG_MONSTRO_HPP
#define RPG_MONSTRO_HPP

#include <string>
#include <vector>

#include "personagem.hpp"

/**
 * @file monstro.hpp
 * @brief Inimigo controlado pelo jogo.
 *
 * Linha em data/monstros.txt:
 *   Nome;Nivel;Vida;Mana;Forca;Inteligencia;Destreza;Defesa;XP;Ouro
 */

namespace rpg {

class Monstro : public Personagem {
public:
    /** @throws EntradaInvalida se nivel < 1 ou recompensas negativas. */
    Monstro(const std::string& nome, int nivel, int vidaMax, int manaMax,
            const Atributos& atributos, int experienciaRecompensa,
            int ouroRecompensa);

    int experienciaRecompensa() const;
    int ouroRecompensa() const;

    std::string classe() const override;

    /** @brief Maior entre forca e inteligencia. */
    int poderDeAtaque() const override;

    /** @brief Poder de ataque + d4. */
    int calcularDano(const Dado& dado) const override;

    std::string serializar() const;

private:
    int experienciaRecompensa_;
    int ouroRecompensa_;
};

/** @throws EntradaInvalida se faltarem campos ou houver valor invalido. */
Monstro criarMonstro(const std::vector<std::string>& campos);

Monstro criarMonstroDeLinha(const std::string& linha);

}  // namespace rpg

#endif  // RPG_MONSTRO_HPP
