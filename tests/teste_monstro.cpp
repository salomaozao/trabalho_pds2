#include "dado.hpp"
#include "doctest.h"
#include "excecoes.hpp"
#include "monstro.hpp"

using namespace rpg;

TEST_CASE("Monstro guarda nivel e recompensas") {
    Monstro orc("Orc", 3, 55, 0, Atributos(11, 2, 4, 3), 60, 25);
    CHECK(orc.classe() == "Monstro");
    CHECK(orc.nivel() == 3);
    CHECK(orc.experienciaRecompensa() == 60);
    CHECK(orc.ouroRecompensa() == 25);
    CHECK(orc.defesaTotal() == 3);
}

TEST_CASE("Monstro recusa nivel e recompensas invalidas") {
    Atributos a(1, 1, 1, 1);
    CHECK_THROWS_AS(Monstro("X", 0, 10, 0, a, 1, 1), EntradaInvalida);
    CHECK_THROWS_AS(Monstro("X", 1, 10, 0, a, -1, 1), EntradaInvalida);
    CHECK_THROWS_AS(Monstro("X", 1, 10, 0, a, 1, -1), EntradaInvalida);
}

TEST_CASE("poder de ataque usa o maior entre forca e inteligencia") {
    Monstro bruto("Troll", 5, 90, 0, Atributos(15, 1, 2, 5), 1, 1);
    Monstro mago("Cultista", 4, 45, 30, Atributos(5, 12, 5, 2), 1, 1);
    CHECK(bruto.poderDeAtaque() == 15);
    CHECK(mago.poderDeAtaque() == 12);
}

TEST_CASE("dano do monstro e poder de ataque mais d4") {
    Monstro orc("Orc", 3, 55, 0, Atributos(11, 2, 4, 3), 60, 25);
    DadoFixo dado(10);
    CHECK(orc.calcularDano(dado) == 11 + 4);

    orc.receberDano(1000);
    CHECK_THROWS_AS(orc.calcularDano(dado), PersonagemMorto);
}

TEST_CASE("criarMonstroDeLinha e serializar sao inversos") {
    std::string linha = "Goblin;1;25;0;5;2;4;1;20;10";
    Monstro goblin = criarMonstroDeLinha(linha);
    CHECK(goblin.nome() == "Goblin");
    CHECK(goblin.vidaMax() == 25);
    CHECK(goblin.atributos().destreza == 4);
    CHECK(goblin.serializar() == linha);
}

TEST_CASE("criarMonstroDeLinha rejeita linhas malformadas") {
    CHECK_THROWS_AS(criarMonstroDeLinha("Goblin;1;25;0;5;2;4;1;20"),
                    EntradaInvalida);
    CHECK_THROWS_AS(criarMonstroDeLinha("Goblin;1;25;0;5;2;4;1;20;10;extra"),
                    EntradaInvalida);
    CHECK_THROWS_AS(criarMonstroDeLinha("Goblin;1;vinte;0;5;2;4;1;20;10"),
                    EntradaInvalida);
    CHECK_THROWS_AS(criarMonstroDeLinha(";1;25;0;5;2;4;1;20;10"),
                    EntradaInvalida);
}
