#include "monstro.hpp"

#include <sstream>

#include "dado.hpp"
#include "excecoes.hpp"
#include "utilitarios.hpp"

namespace rpg {

Monstro::Monstro(const std::string& nome, int nivel, int vidaMax, int manaMax,
                 const Atributos& atributos, int experienciaRecompensa,
                 int ouroRecompensa)
    : Personagem(nome, vidaMax, manaMax, atributos),
      experienciaRecompensa_(experienciaRecompensa),
      ouroRecompensa_(ouroRecompensa) {
    if (experienciaRecompensa < 0 || ouroRecompensa < 0) {
        throw EntradaInvalida("recompensas do monstro nao podem ser negativas.");
    }
    definirNivel(nivel);
}

int Monstro::experienciaRecompensa() const { return experienciaRecompensa_; }
int Monstro::ouroRecompensa() const { return ouroRecompensa_; }

std::string Monstro::classe() const { return "Monstro"; }

int Monstro::poderDeAtaque() const {
    const Atributos& a = atributos();
    return a.forca > a.inteligencia ? a.forca : a.inteligencia;
}

int Monstro::calcularDano(const Dado& dado) const {
    if (!estaVivo()) {
        throw PersonagemMorto(nome());
    }
    return poderDeAtaque() + dado.rolar(4);
}

std::string Monstro::serializar() const {
    const Atributos& a = atributos();
    std::ostringstream fluxo;
    fluxo << nome() << ";" << nivel() << ";" << vidaMax() << ";" << manaMax()
          << ";" << a.forca << ";" << a.inteligencia << ";" << a.destreza
          << ";" << a.defesa << ";" << experienciaRecompensa_ << ";"
          << ouroRecompensa_;
    return fluxo.str();
}

Monstro criarMonstro(const std::vector<std::string>& campos) {
    if (campos.size() != 10) {
        throw EntradaInvalida("linha de monstro precisa de 10 campos.");
    }
    Atributos atributos(util::paraInteiro(campos[4]), util::paraInteiro(campos[5]),
                        util::paraInteiro(campos[6]), util::paraInteiro(campos[7]));
    return Monstro(campos[0], util::paraInteiro(campos[1]),
                   util::paraInteiro(campos[2]), util::paraInteiro(campos[3]),
                   atributos, util::paraInteiro(campos[8]),
                   util::paraInteiro(campos[9]));
}

Monstro criarMonstroDeLinha(const std::string& linha) {
    return criarMonstro(util::dividir(linha, ';'));
}

}  // namespace rpg
