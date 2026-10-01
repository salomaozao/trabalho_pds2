#include "batalha.hpp"
#include "dado.hpp"
#include "doctest.h"
#include "excecoes.hpp"
#include "heroi.hpp"
#include "monstro.hpp"

using namespace rpg;

namespace {

Monstro goblin() {
    return Monstro("Goblin", 1, 25, 0, Atributos(5, 2, 4, 1), 20, 10);
}

}  // namespace

TEST_CASE("batalha comeca em andamento e registra o encontro") {
    Guerreiro heroi("Conan");
    Monstro monstro = goblin();
    DadoFixo dado(3);
    Batalha batalha(heroi, monstro, dado);

    CHECK(batalha.estado() == EstadoBatalha::EM_ANDAMENTO);
    CHECK_FALSE(batalha.terminada());
    CHECK(batalha.rodada() == 0);
    REQUIRE(batalha.registro().size() == 1);
    CHECK(batalha.registro()[0] == "Conan encontra Goblin!");
}

TEST_CASE("nao se inicia batalha com alguem morto") {
    Guerreiro heroi("Conan");
    Monstro monstro = goblin();
    DadoFixo dado(3);

    SUBCASE("heroi morto") {
        heroi.receberDano(1000);
        CHECK_THROWS_AS(Batalha(heroi, monstro, dado), PersonagemMorto);
    }
    SUBCASE("monstro morto") {
        monstro.receberDano(1000);
        CHECK_THROWS_AS(Batalha(heroi, monstro, dado), PersonagemMorto);
    }
}

TEST_CASE("quem tem mais destreza age primeiro") {
    Guerreiro heroi("Conan");
    DadoFixo dado(3);

    Monstro lento = goblin();
    CHECK(Batalha(heroi, lento, dado).heroiAgePrimeiro());

    Monstro lobo("Lobo", 2, 30, 0, Atributos(8, 1, 7, 1), 30, 8);
    Batalha batalha(heroi, lobo, dado);
    CHECK_FALSE(batalha.heroiAgePrimeiro());

    batalha.executarRodada(AcaoDeCombate::ATACAR);
    REQUIRE(batalha.registro().size() == 4);
    CHECK(batalha.registro()[2] == "Lobo ataca Conan e causa 7 de dano.");
    CHECK(batalha.registro()[3] == "Conan ataca Lobo e causa 12 de dano.");
}

TEST_CASE("ataque comum aplica a defesa dos dois lados") {
    Guerreiro heroi("Conan");
    Monstro monstro = goblin();
    DadoFixo dado(3);
    Batalha batalha(heroi, monstro, dado);

    batalha.executarRodada(AcaoDeCombate::ATACAR);

    CHECK(batalha.rodada() == 1);
    CHECK(monstro.vida() == 25 - (10 + 3 - 1));
    CHECK(heroi.vida() == 60 - (5 + 3 - 4));
    CHECK(heroi.estatisticas().danoCausado == 12);
    CHECK(heroi.estatisticas().danoRecebido == 4);
}

TEST_CASE("vitoria concede experiencia e ouro e encerra a batalha") {
    Guerreiro heroi("Conan");
    Monstro monstro = goblin();
    DadoFixo dado(3);
    Batalha batalha(heroi, monstro, dado);

    batalha.executarRodada(AcaoDeCombate::ATACAR);
    batalha.executarRodada(AcaoDeCombate::ATACAR);
    CHECK_FALSE(batalha.terminada());
    batalha.executarRodada(AcaoDeCombate::ATACAR);

    CHECK(batalha.estado() == EstadoBatalha::VITORIA);
    CHECK(batalha.terminada());
    CHECK(batalha.rodada() == 3);
    CHECK_FALSE(monstro.estaVivo());
    CHECK(heroi.experiencia() == 20);
    CHECK(heroi.ouro() == 10);
    CHECK(heroi.estatisticas().vitorias == 1);
    CHECK(batalha.niveisGanhos() == 0);
    CHECK(batalha.registro().back() ==
          "Conan derrota Goblin e ganha 20 de experiencia e 10 de ouro.");

    CHECK_THROWS_AS(batalha.executarRodada(AcaoDeCombate::ATACAR), AcaoInvalida);
}

TEST_CASE("vitoria pode subir o heroi de nivel") {
    Guerreiro heroi("Conan");
    heroi.ganharExperiencia(45);
    Monstro monstro("Rato", 1, 1, 0, Atributos(1, 1, 1, 0), 15, 5);
    DadoFixo dado(1);
    Batalha batalha(heroi, monstro, dado);

    batalha.executarRodada(AcaoDeCombate::ATACAR);
    CHECK(batalha.estado() == EstadoBatalha::VITORIA);
    CHECK(batalha.niveisGanhos() == 1);
    CHECK(heroi.nivel() == 2);
    CHECK(batalha.registro().back() == "Conan sobe para o nivel 2!");
}

TEST_CASE("habilidade gasta mana e usa a formula da classe") {
    Guerreiro heroi("Conan");
    Monstro monstro = goblin();
    DadoFixo dado(3);
    Batalha batalha(heroi, monstro, dado);

    batalha.executarRodada(AcaoDeCombate::HABILIDADE);
    CHECK(monstro.vida() == 25 - (2 * 10 + 3 - 1));
    CHECK(heroi.mana() == 15 - 8);

    SUBCASE("sem mana a rodada nem comeca") {
        heroi.gastarMana(heroi.mana());
        int vidaAntes = heroi.vida();
        CHECK_THROWS_AS(batalha.executarRodada(AcaoDeCombate::HABILIDADE),
                        RecursoInsuficiente);
        CHECK(batalha.rodada() == 1);
        CHECK(heroi.vida() == vidaAntes);
    }
}

TEST_CASE("pocao e usada no turno do heroi e o monstro ainda ataca") {
    Guerreiro heroi("Conan");
    heroi.receberDano(30);
    heroi.inventario().adicionar(
        std::make_shared<Pocao>("Cura", 10, EfeitoPocao::VIDA, 20));
    heroi.inventario().adicionar(std::make_shared<Arma>("Espada", 10, 1));
    Monstro monstro = goblin();
    DadoFixo dado(3);
    Batalha batalha(heroi, monstro, dado);

    batalha.executarRodada(AcaoDeCombate::POCAO, "Cura");
    CHECK(heroi.vida() == 60 - (30 - 4) + 20 - 4);
    CHECK_FALSE(heroi.inventario().contem("Cura"));
    CHECK(batalha.registro()[2] == "Conan usa Cura e recupera 20.");

    SUBCASE("item que nao e pocao e recusado antes da rodada") {
        CHECK_THROWS_AS(batalha.executarRodada(AcaoDeCombate::POCAO, "Espada"),
                        AcaoInvalida);
        CHECK_THROWS_AS(batalha.executarRodada(AcaoDeCombate::POCAO, "Nada"),
                        ItemNaoEncontrado);
        CHECK(batalha.rodada() == 1);
    }
}

TEST_CASE("fuga depende do dado e da destreza") {
    Guerreiro heroi("Conan");
    Monstro monstro = goblin();

    SUBCASE("rolagem alta escapa sem sofrer ataque") {
        DadoFixo dado(6);
        Batalha batalha(heroi, monstro, dado);
        batalha.executarRodada(AcaoDeCombate::FUGIR);
        CHECK(batalha.estado() == EstadoBatalha::FUGA);
        CHECK(heroi.vida() == 60);
        CHECK(heroi.estatisticas().fugas == 1);
        CHECK(nomeDoEstado(batalha.estado()) == "Fuga");
    }

    SUBCASE("rolagem baixa falha e o monstro ataca") {
        DadoFixo dado(1);
        Batalha batalha(heroi, monstro, dado);
        batalha.executarRodada(AcaoDeCombate::FUGIR);
        CHECK(batalha.estado() == EstadoBatalha::EM_ANDAMENTO);
        CHECK(heroi.vida() == 60 - (5 + 1 - 4));
        CHECK(batalha.registro()[2] ==
              "Conan tenta fugir, mas Goblin bloqueia o caminho.");
    }
}

TEST_CASE("derrota encerra a batalha sem recompensa") {
    Mago heroi("Merlin");
    heroi.receberDano(35);
    Monstro troll("Troll", 5, 90, 0, Atributos(15, 1, 2, 5), 110, 50);
    DadoFixo dado(2);
    Batalha batalha(heroi, troll, dado);

    batalha.executarRodada(AcaoDeCombate::ATACAR);
    CHECK(batalha.estado() == EstadoBatalha::DERROTA);
    CHECK_FALSE(heroi.estaVivo());
    CHECK(heroi.experiencia() == 0);
    CHECK(heroi.ouro() == 0);
    CHECK(heroi.estatisticas().derrotas == 1);
    CHECK(batalha.registro().back() == "Merlin cai diante de Troll.");
    CHECK(nomeDoEstado(batalha.estado()) == "Derrota");
}

TEST_CASE("nomeDoEstado cobre todos os estados") {
    CHECK(nomeDoEstado(EstadoBatalha::EM_ANDAMENTO) == "Em andamento");
    CHECK(nomeDoEstado(EstadoBatalha::VITORIA) == "Vitoria");
}
