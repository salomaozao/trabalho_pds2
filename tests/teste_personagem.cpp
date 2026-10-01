#include "dado.hpp"
#include "doctest.h"
#include "excecoes.hpp"
#include "personagem.hpp"

using namespace rpg;

namespace {

// Implementacao minima para exercitar a classe base isoladamente.
class Boneco : public Personagem {
public:
    Boneco(int vida, int mana, const Atributos& a)
        : Personagem("Boneco", vida, mana, a) {}
    std::string classe() const override { return "Treino"; }
    int poderDeAtaque() const override { return atributos().forca; }
};

}  // namespace

TEST_CASE("Atributos nao aceitam valores negativos") {
    CHECK_NOTHROW(Atributos(0, 0, 0, 0));
    CHECK_THROWS_AS(Atributos(-1, 0, 0, 0), EntradaInvalida);
    CHECK_THROWS_AS(Atributos(0, 0, 0, -5), EntradaInvalida);
}

TEST_CASE("Personagem valida nome, vida e mana na criacao") {
    Atributos a(5, 5, 5, 5);
    CHECK_THROWS_AS(Boneco(0, 10, a), EntradaInvalida);
    CHECK_THROWS_AS(Boneco(10, -1, a), EntradaInvalida);

    Boneco ok(30, 10, a);
    CHECK(ok.vida() == 30);
    CHECK(ok.mana() == 10);
    CHECK(ok.nivel() == 1);
    CHECK(ok.estaVivo());
}

TEST_CASE("receberDano desconta a defesa e causa no minimo 1") {
    Boneco b(30, 0, Atributos(5, 0, 0, 4));

    CHECK(b.receberDano(10) == 6);
    CHECK(b.vida() == 24);

    SUBCASE("golpe fraco ainda tira 1") {
        CHECK(b.receberDano(2) == 1);
        CHECK(b.vida() == 23);
    }

    SUBCASE("dano nao passa da vida restante") {
        CHECK(b.receberDano(1000) == 24);
        CHECK(b.vida() == 0);
        CHECK_FALSE(b.estaVivo());
    }

    SUBCASE("dano negativo e recusado") {
        CHECK_THROWS_AS(b.receberDano(-1), EntradaInvalida);
    }
}

TEST_CASE("personagem morto nao age nem recebe acoes") {
    Boneco b(5, 0, Atributos(5, 0, 0, 0));
    DadoFixo dado(3);
    b.receberDano(100);

    CHECK_THROWS_AS(b.receberDano(1), PersonagemMorto);
    CHECK_THROWS_AS(b.curar(10), PersonagemMorto);
    CHECK_THROWS_AS(b.calcularDano(dado), PersonagemMorto);
}

TEST_CASE("curar respeita o maximo") {
    Boneco b(30, 0, Atributos(5, 0, 0, 0));
    b.receberDano(20);
    CHECK(b.curar(5) == 5);
    CHECK(b.curar(100) == 15);
    CHECK(b.vida() == 30);
    CHECK_THROWS_AS(b.curar(-1), EntradaInvalida);
}

TEST_CASE("mana e gasta e recuperada dentro dos limites") {
    Boneco b(30, 20, Atributos(5, 0, 0, 0));
    b.gastarMana(15);
    CHECK(b.mana() == 5);
    CHECK_THROWS_AS(b.gastarMana(6), RecursoInsuficiente);
    CHECK_THROWS_AS(b.gastarMana(-1), EntradaInvalida);
    CHECK(b.recuperarMana(100) == 15);
    CHECK(b.mana() == 20);
    CHECK_THROWS_AS(b.recuperarMana(-1), EntradaInvalida);
}

TEST_CASE("calcularDano padrao soma poder de ataque e d6") {
    Boneco b(30, 0, Atributos(7, 0, 0, 0));
    DadoFixo dado(4);
    CHECK(b.calcularDano(dado) == 11);
}

TEST_CASE("restaurarCompletamente enche vida e mana mesmo apos a morte") {
    Boneco b(30, 10, Atributos(5, 0, 0, 0));
    b.receberDano(100);
    b.restaurarCompletamente();
    CHECK(b.vida() == 30);
    CHECK(b.mana() == 10);
    CHECK(b.estaVivo());
}

TEST_CASE("resumo mostra nome, classe, nivel e recursos") {
    Boneco b(30, 10, Atributos(5, 0, 0, 0));
    CHECK(b.resumo() == "Boneco (Treino, Nv 1) Vida 30/30 | Mana 10/10");
}
