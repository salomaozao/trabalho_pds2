#include "relatorio.hpp"

#include <sstream>

#include "batalha.hpp"
#include "catalogo.hpp"
#include "heroi.hpp"
#include "inventario.hpp"
#include "monstro.hpp"
#include "utilitarios.hpp"

namespace rpg {
namespace relatorio {

namespace {

const int LARGURA_BARRA = 20;
const std::size_t LARGURA_NOME = 26;
const std::size_t LARGURA_DESCRICAO = 34;

std::string linha() { return util::repetir('-', 60) + "\n"; }

std::string fracao(int atual, int maximo) {
    std::ostringstream fluxo;
    fluxo << atual << "/" << maximo;
    return fluxo.str();
}

}  // namespace

std::string barraDeVida(const Personagem& p) {
    return "Vida " + util::barraDeProgresso(p.vida(), p.vidaMax(), LARGURA_BARRA) +
           " " + fracao(p.vida(), p.vidaMax());
}

std::string barraDeMana(const Personagem& p) {
    if (p.manaMax() == 0) {
        return "Mana " + util::barraDeProgresso(0, 1, LARGURA_BARRA) + " 0/0";
    }
    return "Mana " + util::barraDeProgresso(p.mana(), p.manaMax(), LARGURA_BARRA) +
           " " + fracao(p.mana(), p.manaMax());
}

std::string fichaDoHeroi(const Heroi& h) {
    const Atributos& a = h.atributos();
    std::ostringstream fluxo;

    fluxo << linha();
    fluxo << h.nome() << " - " << h.classe() << " nivel " << h.nivel() << "\n";
    fluxo << linha();
    fluxo << barraDeVida(h) << "\n";
    fluxo << barraDeMana(h) << "\n";
    fluxo << "Experiencia: " << h.experiencia() << "/"
          << h.experienciaParaProximoNivel() << "\n";
    fluxo << "Ouro: " << h.ouro() << "\n\n";
    fluxo << "Forca " << a.forca << " | Inteligencia " << a.inteligencia
          << " | Destreza " << a.destreza << " | Defesa " << a.defesa << "\n";
    fluxo << "Ataque total: " << h.poderDeAtaque()
          << " | Defesa total: " << h.defesaTotal() << "\n";
    fluxo << "Habilidade: " << h.nomeHabilidade() << " (" << h.custoHabilidade()
          << " de mana)\n\n";
    fluxo << "Arma: "
          << (h.armaEquipada() ? h.armaEquipada()->nome() : std::string("nenhuma"))
          << "\n";
    fluxo << "Armadura: "
          << (h.armaduraEquipada() ? h.armaduraEquipada()->nome()
                                   : std::string("nenhuma"))
          << "\n";
    fluxo << linha();
    return fluxo.str();
}

std::string estatisticas(const Heroi& h) {
    const Estatisticas& e = h.estatisticas();
    int batalhas = e.vitorias + e.derrotas + e.fugas;
    std::ostringstream fluxo;

    fluxo << "Batalhas: " << batalhas << " | Vitorias: " << e.vitorias
          << " | Derrotas: " << e.derrotas << " | Fugas: " << e.fugas << "\n";
    fluxo << "Dano causado: " << e.danoCausado
          << " | Dano recebido: " << e.danoRecebido << "\n";
    if (batalhas > 0) {
        fluxo << "Taxa de vitoria: " << (100 * e.vitorias) / batalhas << "%\n";
    }
    return fluxo.str();
}

std::string tabelaInventario(const Inventario& inv) {
    std::ostringstream fluxo;
    fluxo << "Inventario (" << inv.tamanho() << "/" << inv.capacidade() << ")\n";
    if (inv.vazio()) {
        fluxo << "  (vazio)\n";
        return fluxo.str();
    }
    const std::vector<std::shared_ptr<Item> >& itens = inv.itens();
    for (std::size_t i = 0; i < itens.size(); i++) {
        fluxo << "  " << util::alinharEsquerda(itens[i]->nome(), LARGURA_NOME)
              << util::alinharEsquerda(itens[i]->descricao(), LARGURA_DESCRICAO)
              << "vende por " << itens[i]->preco() / 2 << "\n";
    }
    return fluxo.str();
}

std::string tabelaCatalogo(const CatalogoItens& catalogo) {
    std::ostringstream fluxo;
    const std::vector<std::shared_ptr<Item> >& itens = catalogo.itens();
    fluxo << "  " << util::alinharEsquerda("Item", LARGURA_NOME)
          << util::alinharEsquerda("Efeito", LARGURA_DESCRICAO) << "Preco\n";
    for (std::size_t i = 0; i < itens.size(); i++) {
        fluxo << "  " << util::alinharEsquerda(itens[i]->nome(), LARGURA_NOME)
              << util::alinharEsquerda(itens[i]->descricao(), LARGURA_DESCRICAO)
              << itens[i]->preco() << "\n";
    }
    return fluxo.str();
}

std::string listaMonstros(const std::vector<Monstro>& monstros) {
    std::ostringstream fluxo;
    for (std::size_t i = 0; i < monstros.size(); i++) {
        const Monstro& m = monstros[i];
        fluxo << "  " << util::alinharEsquerda(m.nome(), LARGURA_NOME) << "Nv "
              << m.nivel() << " | Vida " << m.vidaMax() << " | Ataque "
              << m.poderDeAtaque() << " | Defesa " << m.defesaTotal() << " | "
              << m.experienciaRecompensa() << " xp, " << m.ouroRecompensa()
              << " ouro\n";
    }
    return fluxo.str();
}

std::string painelDeCombate(const Batalha& b) {
    const Heroi& h = b.heroi();
    const Monstro& m = b.monstro();
    std::ostringstream fluxo;

    fluxo << linha();
    fluxo << util::alinharEsquerda(h.nome() + " (Nv " + std::to_string(h.nivel()) + ")",
                                   30)
          << m.nome() << " (Nv " << m.nivel() << ")\n";
    fluxo << util::alinharEsquerda(barraDeVida(h), 36) << "  " << barraDeVida(m) << "\n";
    fluxo << util::alinharEsquerda(barraDeMana(h), 36) << "\n";
    fluxo << linha();
    return fluxo.str();
}

std::string resumoBatalha(const Batalha& b) {
    std::ostringstream fluxo;
    fluxo << "Resultado: " << nomeDoEstado(b.estado()) << " em " << b.rodada()
          << " rodada(s).\n";
    if (b.estado() == EstadoBatalha::VITORIA) {
        fluxo << "Recompensa: " << b.monstro().experienciaRecompensa()
              << " de experiencia e " << b.monstro().ouroRecompensa()
              << " de ouro.\n";
        if (b.niveisGanhos() > 0) {
            fluxo << b.heroi().nome() << " subiu " << b.niveisGanhos()
                  << " nivel(is) e agora esta no nivel " << b.heroi().nivel()
                  << ".\n";
        }
    }
    return fluxo.str();
}

}  // namespace relatorio
}  // namespace rpg
