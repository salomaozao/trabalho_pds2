#include "batalha.hpp"
#include "catalogo.hpp"
#include "dado.hpp"
#include "doctest.h"
#include "heroi.hpp"
#include "monstro.hpp"
#include "relatorio.hpp"

using namespace rpg;

namespace {

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}

}  // namespace

TEST_CASE("barras mostram a proporcao e a fracao") {
    Guerreiro heroi("Conan");
    heroi.receberDano(34);
    CHECK(relatorio::barraDeVida(heroi) == "Vida [##########----------] 30/60");
    CHECK(relatorio::barraDeMana(heroi) == "Mana [####################] 15/15");

    Monstro semMana("Rato", 1, 10, 0, Atributos(1, 1, 1, 0), 1, 1);
    CHECK(relatorio::barraDeMana(semMana) == "Mana [--------------------] 0/0");
}

TEST_CASE("ficha do heroi reune recursos, atributos e equipamentos") {
    Mago heroi("Merlin");
    heroi.inventario().adicionar(std::make_shared<Arma>("Cajado", 70, 5));
    heroi.equipar("Cajado");
    heroi.ganharOuro(42);

    std::string ficha = relatorio::fichaDoHeroi(heroi);
    CHECK(contem(ficha, "Merlin - Mago nivel 1"));
    CHECK(contem(ficha, "Ouro: 42"));
    CHECK(contem(ficha, "Inteligencia 12"));
    CHECK(contem(ficha, "Ataque total: 17"));
    CHECK(contem(ficha, "Habilidade: Bola de Fogo (15 de mana)"));
    CHECK(contem(ficha, "Arma: Cajado"));
    CHECK(contem(ficha, "Armadura: nenhuma"));
}

TEST_CASE("estatisticas calculam a taxa de vitoria quando ha batalhas") {
    Guerreiro heroi("Conan");
    CHECK_FALSE(contem(relatorio::estatisticas(heroi), "Taxa"));

    heroi.estatisticas().vitorias = 3;
    heroi.estatisticas().derrotas = 1;
    std::string texto = relatorio::estatisticas(heroi);
    CHECK(contem(texto, "Batalhas: 4"));
    CHECK(contem(texto, "Taxa de vitoria: 75%"));
}

TEST_CASE("tabela do inventario lista itens ou avisa que esta vazio") {
    Inventario inv(3);
    CHECK(contem(relatorio::tabelaInventario(inv), "(vazio)"));

    inv.adicionar(std::make_shared<Pocao>("Cura", 25, EfeitoPocao::VIDA, 30));
    std::string tabela = relatorio::tabelaInventario(inv);
    CHECK(contem(tabela, "Inventario (1/3)"));
    CHECK(contem(tabela, "Cura"));
    CHECK(contem(tabela, "vende por 12"));
}

TEST_CASE("tabelas de catalogo e monstros mostram uma linha por entrada") {
    CatalogoItens itens;
    itens.adicionar(std::make_shared<Arma>("Espada", 60, 4));
    itens.adicionar(std::make_shared<Armadura>("Cota", 150, 5));
    std::string catalogo = relatorio::tabelaCatalogo(itens);
    CHECK(contem(catalogo, "Espada"));
    CHECK(contem(catalogo, "Armadura, +5 de defesa"));
    CHECK(contem(catalogo, "150"));

    std::vector<Monstro> monstros;
    monstros.push_back(criarMonstroDeLinha("Orc;3;55;0;11;2;4;3;60;25"));
    std::string lista = relatorio::listaMonstros(monstros);
    CHECK(contem(lista, "Orc"));
    CHECK(contem(lista, "Nv 3"));
    CHECK(contem(lista, "60 xp, 25 ouro"));
}

TEST_CASE("painel e resumo da batalha refletem o estado") {
    Guerreiro heroi("Conan");
    Monstro rato("Rato", 1, 1, 0, Atributos(1, 1, 1, 0), 15, 5);
    DadoFixo dado(1);
    Batalha batalha(heroi, rato, dado);

    std::string painel = relatorio::painelDeCombate(batalha);
    CHECK(contem(painel, "Conan (Nv 1)"));
    CHECK(contem(painel, "Rato (Nv 1)"));
    CHECK(contem(painel, "1/1"));

    CHECK(contem(relatorio::resumoBatalha(batalha), "Em andamento em 0 rodada(s)"));

    heroi.ganharExperiencia(40);
    batalha.executarRodada(AcaoDeCombate::ATACAR);
    std::string resumo = relatorio::resumoBatalha(batalha);
    CHECK(contem(resumo, "Resultado: Vitoria em 1 rodada(s)."));
    CHECK(contem(resumo, "Recompensa: 15 de experiencia e 5 de ouro."));
    CHECK(contem(resumo, "subiu 1 nivel(is)"));
}
