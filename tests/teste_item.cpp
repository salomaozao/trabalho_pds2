#include "doctest.h"
#include "excecoes.hpp"
#include "heroi.hpp"
#include "item.hpp"

using namespace rpg;

TEST_CASE("Item valida nome e preco") {
    CHECK_THROWS_AS(Arma("", 10, 1), EntradaInvalida);
    CHECK_THROWS_AS(Arma("   ", 10, 1), EntradaInvalida);
    CHECK_THROWS_AS(Arma("Espada", -1, 1), EntradaInvalida);
    CHECK_THROWS_AS(Arma("Espada", 10, -1), EntradaInvalida);
    CHECK_THROWS_AS(Armadura("Cota", 10, -1), EntradaInvalida);
    CHECK_THROWS_AS(Pocao("Pocao", 10, EfeitoPocao::VIDA, 0), EntradaInvalida);
}

TEST_CASE("cada tipo de item se descreve e se serializa") {
    Arma espada(" Espada Longa ", 120, 8);
    CHECK(espada.nome() == "Espada Longa");
    CHECK(espada.tipo() == TipoItem::ARMA);
    CHECK(espada.descricao() == "Arma, +8 de ataque");
    CHECK(espada.serializar() == "ARMA;Espada Longa;120;8");

    Armadura cota("Cota de Malha", 150, 5);
    CHECK(cota.tipo() == TipoItem::ARMADURA);
    CHECK(cota.descricao() == "Armadura, +5 de defesa");
    CHECK(cota.serializar() == "ARMADURA;Cota de Malha;150;5");

    Pocao pocao("Pocao de Mana", 30, EfeitoPocao::MANA, 20);
    CHECK(pocao.tipo() == TipoItem::POCAO);
    CHECK(pocao.descricao() == "Pocao, recupera 20 de mana");
    CHECK(pocao.serializar() == "POCAO;Pocao de Mana;30;MANA;20");
}

TEST_CASE("clonar produz copia independente") {
    Arma original("Adaga", 20, 2);
    std::shared_ptr<Item> copia = original.clonar();
    CHECK(copia->nome() == "Adaga");
    CHECK(copia->serializar() == original.serializar());
    CHECK(copia.get() != &original);
}

TEST_CASE("Pocao aplica o efeito no personagem") {
    Guerreiro heroi("Conan");
    heroi.receberDano(50);
    heroi.gastarMana(10);

    Pocao vida("Cura", 10, EfeitoPocao::VIDA, 30);
    CHECK(vida.aplicar(heroi) == 30);

    Pocao mana("Foco", 10, EfeitoPocao::MANA, 50);
    CHECK(mana.aplicar(heroi) == 10);
    CHECK(heroi.mana() == heroi.manaMax());
}

TEST_CASE("criarItemDeLinha monta o item certo para cada tipo") {
    std::shared_ptr<Item> arma = criarItemDeLinha("ARMA;Espada Curta;60;4");
    REQUIRE(arma->tipo() == TipoItem::ARMA);
    CHECK(arma->preco() == 60);

    std::shared_ptr<Item> armadura = criarItemDeLinha("armadura;Couro;60;3");
    REQUIRE(armadura->tipo() == TipoItem::ARMADURA);

    std::shared_ptr<Item> pocao = criarItemDeLinha("POCAO;Cura;25;vida;30");
    REQUIRE(pocao->tipo() == TipoItem::POCAO);
    CHECK(pocao->serializar() == "POCAO;Cura;25;VIDA;30");
}

TEST_CASE("criarItemDeLinha rejeita linhas malformadas") {
    CHECK_THROWS_AS(criarItemDeLinha("ARMA;Espada;60"), EntradaInvalida);
    CHECK_THROWS_AS(criarItemDeLinha("ESCUDO;Broquel;60;4"), EntradaInvalida);
    CHECK_THROWS_AS(criarItemDeLinha("ARMA;Espada;abc;4"), EntradaInvalida);
    CHECK_THROWS_AS(criarItemDeLinha("POCAO;Cura;25;VIDA"), EntradaInvalida);
    CHECK_THROWS_AS(criarItemDeLinha("POCAO;Cura;25;FORCA;30"), EntradaInvalida);
}
