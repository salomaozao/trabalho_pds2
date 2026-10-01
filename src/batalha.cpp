#include "batalha.hpp"

#include <sstream>

#include "dado.hpp"
#include "excecoes.hpp"
#include "heroi.hpp"
#include "monstro.hpp"

namespace rpg {

std::string nomeDoEstado(EstadoBatalha estado) {
    switch (estado) {
        case EstadoBatalha::EM_ANDAMENTO:
            return "Em andamento";
        case EstadoBatalha::VITORIA:
            return "Vitoria";
        case EstadoBatalha::DERROTA:
            return "Derrota";
        case EstadoBatalha::FUGA:
            return "Fuga";
    }
    return "";
}

Batalha::Batalha(Heroi& heroi, Monstro& monstro, const Dado& dado)
    : heroi_(heroi),
      monstro_(monstro),
      dado_(dado),
      estado_(EstadoBatalha::EM_ANDAMENTO),
      rodada_(0),
      niveisGanhos_(0) {
    if (!heroi_.estaVivo()) {
        throw PersonagemMorto(heroi_.nome());
    }
    if (!monstro_.estaVivo()) {
        throw PersonagemMorto(monstro_.nome());
    }
    registrar(heroi_.nome() + " encontra " + monstro_.nome() + "!");
}

Heroi& Batalha::heroi() const { return heroi_; }
Monstro& Batalha::monstro() const { return monstro_; }
EstadoBatalha Batalha::estado() const { return estado_; }
int Batalha::rodada() const { return rodada_; }
bool Batalha::terminada() const {
    return estado_ != EstadoBatalha::EM_ANDAMENTO;
}
int Batalha::niveisGanhos() const { return niveisGanhos_; }

bool Batalha::heroiAgePrimeiro() const {
    return heroi_.atributos().destreza >= monstro_.atributos().destreza;
}

const std::vector<std::string>& Batalha::registro() const { return registro_; }

void Batalha::executarRodada(AcaoDeCombate acao, const std::string& parametro) {
    if (terminada()) {
        throw AcaoInvalida("a batalha ja terminou.");
    }

    // Confere antes de qualquer turno, para o monstro nao ganhar um golpe
    // gratis quando a escolha do jogador nao puder ser executada.
    if (acao == AcaoDeCombate::HABILIDADE &&
        heroi_.mana() < heroi_.custoHabilidade()) {
        throw RecursoInsuficiente("mana", heroi_.mana(),
                                  heroi_.custoHabilidade());
    }
    if (acao == AcaoDeCombate::POCAO &&
        heroi_.inventario().buscar(parametro)->tipo() != TipoItem::POCAO) {
        throw AcaoInvalida("'" + parametro + "' nao e uma pocao.");
    }

    rodada_++;
    std::ostringstream cabecalho;
    cabecalho << "--- Rodada " << rodada_ << " ---";
    registrar(cabecalho.str());

    if (heroiAgePrimeiro()) {
        turnoDoHeroi(acao, parametro);
        if (!terminada()) {
            turnoDoMonstro();
        }
    } else {
        turnoDoMonstro();
        if (!terminada()) {
            turnoDoHeroi(acao, parametro);
        }
    }
}

void Batalha::turnoDoHeroi(AcaoDeCombate acao, const std::string& parametro) {
    std::ostringstream texto;

    switch (acao) {
        case AcaoDeCombate::ATACAR: {
            int dano = monstro_.receberDano(heroi_.calcularDano(dado_));
            heroi_.estatisticas().danoCausado += dano;
            texto << heroi_.nome() << " ataca " << monstro_.nome()
                  << " e causa " << dano << " de dano.";
            break;
        }
        case AcaoDeCombate::HABILIDADE: {
            int dano = monstro_.receberDano(heroi_.usarHabilidade(dado_));
            heroi_.estatisticas().danoCausado += dano;
            texto << heroi_.nome() << " usa " << heroi_.nomeHabilidade()
                  << " em " << monstro_.nome() << " e causa " << dano
                  << " de dano.";
            break;
        }
        case AcaoDeCombate::POCAO: {
            int recuperado = heroi_.usarPocao(parametro);
            texto << heroi_.nome() << " usa " << parametro << " e recupera "
                  << recuperado << ".";
            break;
        }
        case AcaoDeCombate::FUGIR:
            tentarFugir();
            return;
    }

    registrar(texto.str());
    verificarFim();
}

void Batalha::turnoDoMonstro() {
    int dano = heroi_.receberDano(monstro_.calcularDano(dado_));
    heroi_.estatisticas().danoRecebido += dano;

    std::ostringstream texto;
    texto << monstro_.nome() << " ataca " << heroi_.nome() << " e causa "
          << dano << " de dano.";
    registrar(texto.str());
    verificarFim();
}

void Batalha::tentarFugir() {
    int rolagem = dado_.rolar(6) + heroi_.atributos().destreza / 4;
    if (rolagem >= 5) {
        estado_ = EstadoBatalha::FUGA;
        heroi_.estatisticas().fugas++;
        registrar(heroi_.nome() + " escapa da batalha.");
    } else {
        registrar(heroi_.nome() + " tenta fugir, mas " + monstro_.nome() +
                  " bloqueia o caminho.");
    }
}

void Batalha::verificarFim() {
    std::ostringstream texto;

    if (!monstro_.estaVivo()) {
        estado_ = EstadoBatalha::VITORIA;
        heroi_.estatisticas().vitorias++;
        heroi_.ganharOuro(monstro_.ouroRecompensa());
        niveisGanhos_ = heroi_.ganharExperiencia(monstro_.experienciaRecompensa());
        texto << heroi_.nome() << " derrota " << monstro_.nome() << " e ganha "
              << monstro_.experienciaRecompensa() << " de experiencia e "
              << monstro_.ouroRecompensa() << " de ouro.";
        registrar(texto.str());
        if (niveisGanhos_ > 0) {
            std::ostringstream nivel;
            nivel << heroi_.nome() << " sobe para o nivel " << heroi_.nivel()
                  << "!";
            registrar(nivel.str());
        }
    } else if (!heroi_.estaVivo()) {
        estado_ = EstadoBatalha::DERROTA;
        heroi_.estatisticas().derrotas++;
        texto << heroi_.nome() << " cai diante de " << monstro_.nome() << ".";
        registrar(texto.str());
    }
}

void Batalha::registrar(const std::string& texto) { registro_.push_back(texto); }

}  // namespace rpg
