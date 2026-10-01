/**
 * @file teste_utilitarios.cpp
 * @brief Testes de unidade das funcoes auxiliares de rpg::util.
 */

#include "doctest.h"
#include "excecoes.hpp"
#include "utilitarios.hpp"

using namespace rpg;

TEST_CASE("aparar remove os espacos das pontas") {
    CHECK(util::aparar("  espada  ") == "espada");
    CHECK(util::aparar("espada") == "espada");
    CHECK(util::aparar("\t\n pocao \n") == "pocao");

    SUBCASE("textos sem conteudo viram string vazia") {
        CHECK(util::aparar("") == "");
        CHECK(util::aparar("     ") == "");
        CHECK(util::aparar("\t\t") == "");
    }
}

TEST_CASE("dividir quebra a linha nos separadores") {
    std::vector<std::string> campos = util::dividir("ARMA;Espada;120", ';');

    REQUIRE(campos.size() == 3);
    CHECK(campos[0] == "ARMA");
    CHECK(campos[1] == "Espada");
    CHECK(campos[2] == "120");

    SUBCASE("os campos ja vem aparados") {
        std::vector<std::string> soltos = util::dividir(" a ; b ; c ", ';');
        REQUIRE(soltos.size() == 3);
        CHECK(soltos[0] == "a");
        CHECK(soltos[1] == "b");
        CHECK(soltos[2] == "c");
    }

    SUBCASE("campos vazios sao preservados") {
        std::vector<std::string> vazios = util::dividir("a;;c", ';');
        REQUIRE(vazios.size() == 3);
        CHECK(vazios[1] == "");
    }

    SUBCASE("texto sem separador devolve um unico pedaco") {
        std::vector<std::string> unico = util::dividir("sozinho", ';');
        REQUIRE(unico.size() == 1);
        CHECK(unico[0] == "sozinho");
    }
}

TEST_CASE("paraInteiro converte numeros validos") {
    CHECK(util::paraInteiro("42") == 42);
    CHECK(util::paraInteiro("  7  ") == 7);
    CHECK(util::paraInteiro("-15") == -15);
    CHECK(util::paraInteiro("+3") == 3);
    CHECK(util::paraInteiro("0") == 0);
}

TEST_CASE("paraInteiro rejeita textos que nao sao numeros") {
    CHECK_THROWS_AS(util::paraInteiro(""), EntradaInvalida);
    CHECK_THROWS_AS(util::paraInteiro("   "), EntradaInvalida);
    CHECK_THROWS_AS(util::paraInteiro("12a"), EntradaInvalida);
    CHECK_THROWS_AS(util::paraInteiro("abc"), EntradaInvalida);
    CHECK_THROWS_AS(util::paraInteiro("-"), EntradaInvalida);
    CHECK_THROWS_AS(util::paraInteiro("1.5"), EntradaInvalida);

    SUBCASE("a excecao tambem e capturavel como ErroDeJogo") {
        CHECK_THROWS_AS(util::paraInteiro("xyz"), ErroDeJogo);
    }
}

TEST_CASE("conversao de caixa e comparacao sem caixa") {
    CHECK(util::paraMaiusculas("espada") == "ESPADA");
    CHECK(util::paraMinusculas("ESPADA") == "espada");
    CHECK(util::iguaisSemCaixa("Espada Longa", "espada longa"));
    CHECK(util::iguaisSemCaixa("  ARCO  ", "arco"));
    CHECK_FALSE(util::iguaisSemCaixa("Espada", "Escudo"));
}

TEST_CASE("linhaIgnoravel identifica comentarios e linhas vazias") {
    CHECK(util::linhaIgnoravel(""));
    CHECK(util::linhaIgnoravel("   "));
    CHECK(util::linhaIgnoravel("# comentario"));
    CHECK(util::linhaIgnoravel("   # comentario indentado"));
    CHECK_FALSE(util::linhaIgnoravel("ARMA;Espada;120"));
}

TEST_CASE("limitar mantem o valor dentro do intervalo") {
    CHECK(util::limitar(5, 0, 10) == 5);
    CHECK(util::limitar(-3, 0, 10) == 0);
    CHECK(util::limitar(99, 0, 10) == 10);
    CHECK(util::limitar(0, 0, 0) == 0);

    SUBCASE("intervalo invertido e recusado") {
        CHECK_THROWS_AS(util::limitar(5, 10, 0), EntradaInvalida);
    }
}

TEST_CASE("barraDeProgresso desenha a proporcao correta") {
    CHECK(util::barraDeProgresso(10, 10, 10) == "[##########]");
    CHECK(util::barraDeProgresso(0, 10, 10) == "[----------]");
    CHECK(util::barraDeProgresso(5, 10, 10) == "[#####-----]");

    SUBCASE("valores fora da faixa sao ajustados") {
        CHECK(util::barraDeProgresso(-5, 10, 4) == "[----]");
        CHECK(util::barraDeProgresso(50, 10, 4) == "[####]");
    }

    SUBCASE("parametros invalidos lancam excecao") {
        CHECK_THROWS_AS(util::barraDeProgresso(1, 0, 10), EntradaInvalida);
        CHECK_THROWS_AS(util::barraDeProgresso(1, 10, 0), EntradaInvalida);
    }
}

TEST_CASE("alinharEsquerda e repetir formatam o texto dos relatorios") {
    CHECK(util::alinharEsquerda("abc", 5) == "abc  ");
    CHECK(util::alinharEsquerda("abcdef", 3) == "abcdef");
    CHECK(util::repetir('-', 4) == "----");
    CHECK(util::repetir('=', 0) == "");
}
