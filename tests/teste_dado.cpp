#include "dado.hpp"
#include "doctest.h"
#include "excecoes.hpp"

using namespace rpg;

TEST_CASE("DadoFixo devolve sempre o mesmo valor") {
    DadoFixo dado(4);
    CHECK(dado.rolar(6) == 4);
    CHECK(dado.rolar(6) == 4);
    CHECK(dado.rolagens() == 2);
}

TEST_CASE("DadoFixo percorre a sequencia e recomeca") {
    std::vector<int> seq;
    seq.push_back(1);
    seq.push_back(5);
    seq.push_back(3);
    DadoFixo dado(seq);

    CHECK(dado.rolar(6) == 1);
    CHECK(dado.rolar(6) == 5);
    CHECK(dado.rolar(6) == 3);
    CHECK(dado.rolar(6) == 1);
}

TEST_CASE("DadoFixo limita o valor ao numero de faces") {
    DadoFixo dado(10);
    CHECK(dado.rolar(6) == 6);
    CHECK(dado.rolar(20) == 10);
}

TEST_CASE("DadoFixo rejeita valores impossiveis") {
    CHECK_THROWS_AS(DadoFixo(0), EntradaInvalida);
    CHECK_THROWS_AS(DadoFixo(std::vector<int>()), EntradaInvalida);

    std::vector<int> ruim;
    ruim.push_back(2);
    ruim.push_back(-1);
    CHECK_THROWS_AS(DadoFixo dado(ruim), EntradaInvalida);
}

TEST_CASE("rolar exige pelo menos uma face") {
    DadoFixo fixo(1);
    DadoAleatorio aleatorio(7);
    CHECK_THROWS_AS(fixo.rolar(0), EntradaInvalida);
    CHECK_THROWS_AS(aleatorio.rolar(-3), EntradaInvalida);
}

TEST_CASE("rolarVarios soma as rolagens") {
    DadoFixo dado(3);
    CHECK(dado.rolarVarios(4, 6) == 12);
    CHECK_THROWS_AS(dado.rolarVarios(0, 6), EntradaInvalida);
}

TEST_CASE("DadoAleatorio fica dentro do intervalo") {
    DadoAleatorio dado(12345);
    for (int i = 0; i < 200; i++) {
        int valor = dado.rolar(6);
        CHECK(valor >= 1);
        CHECK(valor <= 6);
    }
}

TEST_CASE("DadoAleatorio com a mesma semente repete a sequencia") {
    DadoAleatorio a(99);
    DadoAleatorio b(99);
    for (int i = 0; i < 20; i++) {
        CHECK(a.rolar(20) == b.rolar(20));
    }
}
