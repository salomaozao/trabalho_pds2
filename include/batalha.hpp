#ifndef RPG_BATALHA_HPP
#define RPG_BATALHA_HPP

#include <string>
#include <vector>

/**
 * @file batalha.hpp
 * @brief Combate por turnos entre um heroi e um monstro.
 *
 * Cada rodada o heroi escolhe uma acao e o monstro ataca. Quem tem mais
 * destreza age primeiro. Ao vencer, o heroi recebe a experiencia e o ouro
 * do monstro automaticamente.
 */

namespace rpg {

class Dado;
class Heroi;
class Monstro;

enum class AcaoDeCombate { ATACAR, HABILIDADE, POCAO, FUGIR };
enum class EstadoBatalha { EM_ANDAMENTO, VITORIA, DERROTA, FUGA };

std::string nomeDoEstado(EstadoBatalha estado);

class Batalha {
public:
    /** @throws PersonagemMorto se um dos dois ja comecar morto. */
    Batalha(Heroi& heroi, Monstro& monstro, const Dado& dado);

    Heroi& heroi() const;
    Monstro& monstro() const;
    EstadoBatalha estado() const;
    int rodada() const;
    bool terminada() const;
    bool heroiAgePrimeiro() const;
    const std::vector<std::string>& registro() const;
    int niveisGanhos() const;

    /**
     * @brief Executa uma rodada completa.
     * @param acao      Escolha do heroi.
     * @param parametro Nome da pocao quando a acao for POCAO.
     * @throws AcaoInvalida se a batalha ja terminou.
     */
    void executarRodada(AcaoDeCombate acao, const std::string& parametro = "");

private:
    void turnoDoHeroi(AcaoDeCombate acao, const std::string& parametro);
    void turnoDoMonstro();
    void tentarFugir();
    void verificarFim();
    void registrar(const std::string& texto);

    Heroi& heroi_;
    Monstro& monstro_;
    const Dado& dado_;
    EstadoBatalha estado_;
    int rodada_;
    int niveisGanhos_;
    std::vector<std::string> registro_;
};

}  // namespace rpg

#endif  // RPG_BATALHA_HPP
