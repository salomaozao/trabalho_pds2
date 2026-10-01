#include "dado.hpp"
#include "doctest.h"
#include "excecoes.hpp"
#include "heroi.hpp"

using namespace rpg;

TEST_CASE("cada classe nasce com seus atributos e atributo primario") {
    Guerreiro g("Conan");
    CHECK(g.classe() == "Guerreiro");
    CHECK(g.vidaMax() == 60);
    CHECK(g.atributoPrimario() == g.atributos().forca);
    CHECK(g.poderDeAtaque() == 10);

    Mago m("Merlin");
    CHECK(m.classe() == "Mago");
    CHECK(m.manaMax() == 50);
    CHECK(m.atributoPrimario() == m.atributos().inteligencia);
    CHECK(m.poderDeAtaque() == 12);

    Arqueiro a("Legolas");
    CHECK(a.classe() == "Arqueiro");
    CHECK(a.atributoPrimario() == a.atributos().destreza);
    CHECK(a.poderDeAtaque() == 11);
}

TEST_CASE("cada classe tem sua habilidade com formula propria") {
    DadoFixo tres(3);

    Guerreiro g("Conan");
    CHECK(g.nomeHabilidade() == "Golpe Devastador");
    CHECK(g.usarHabilidade(tres) == 2 * 10 + 3);
    CHECK(g.mana() == 15 - 8);

    DadoFixo cinco(5);
    Mago m("Merlin");
    CHECK(m.nomeHabilidade() == "Bola de Fogo");
    CHECK(m.usarHabilidade(cinco) == 12 + 5 + 5);
    CHECK(m.mana() == 50 - 15);

    DadoFixo dois(2);
    Arqueiro a("Legolas");
    CHECK(a.nomeHabilidade() == "Flecha Dupla");
    CHECK(a.usarHabilidade(dois) == (11 + 2) + (11 + 2));
    CHECK(a.mana() == 25 - 10);
}

TEST_CASE("habilidade exige mana e heroi vivo") {
    DadoFixo dado(1);
    Guerreiro g("Conan");
    g.gastarMana(10);
    CHECK_THROWS_AS(g.usarHabilidade(dado), RecursoInsuficiente);

    g.receberDano(1000);
    CHECK_THROWS_AS(g.usarHabilidade(dado), PersonagemMorto);
}

TEST_CASE("experiencia acumula e sobe de nivel restaurando os recursos") {
    Guerreiro g("Conan");
    g.receberDano(30);
    CHECK(g.experienciaParaProximoNivel() == 50);

    CHECK(g.ganharExperiencia(30) == 0);
    CHECK(g.nivel() == 1);
    CHECK(g.experiencia() == 30);

    CHECK(g.ganharExperiencia(25) == 1);
    CHECK(g.nivel() == 2);
    CHECK(g.experiencia() == 5);
    CHECK(g.vida() == g.vidaMax());
    CHECK(g.vidaMax() == 70);
    CHECK(g.atributos().forca == 13);
    CHECK(g.experienciaParaProximoNivel() == 100);

    SUBCASE("muita experiencia de uma vez sobe varios niveis") {
        CHECK(g.ganharExperiencia(400) == 2);
        CHECK(g.nivel() == 4);
    }

    SUBCASE("experiencia negativa e recusada") {
        CHECK_THROWS_AS(g.ganharExperiencia(-1), EntradaInvalida);
    }
}

TEST_CASE("progressao de nivel e diferente por classe") {
    Mago m("Merlin");
    m.ganharExperiencia(50);
    CHECK(m.vidaMax() == 40);
    CHECK(m.manaMax() == 60);
    CHECK(m.atributos().inteligencia == 15);

    Arqueiro a("Legolas");
    a.ganharExperiencia(50);
    CHECK(a.vidaMax() == 52);
    CHECK(a.atributos().destreza == 14);
    CHECK(a.atributos().forca == 6);
}

TEST_CASE("ouro e ganho e gasto com validacao") {
    Guerreiro g("Conan");
    g.ganharOuro(100);
    g.gastarOuro(40);
    CHECK(g.ouro() == 60);
    CHECK_THROWS_AS(g.gastarOuro(61), RecursoInsuficiente);
    CHECK_THROWS_AS(g.gastarOuro(-1), EntradaInvalida);
    CHECK_THROWS_AS(g.ganharOuro(-1), EntradaInvalida);
    CHECK(g.ouro() == 60);
}

TEST_CASE("equipar arma e armadura altera ataque e defesa") {
    Guerreiro g("Conan");
    g.inventario().adicionar(std::make_shared<Arma>("Espada", 100, 8));
    g.inventario().adicionar(std::make_shared<Armadura>("Cota", 100, 5));

    g.equipar("espada");
    g.equipar("cota");
    CHECK(g.poderDeAtaque() == 18);
    CHECK(g.defesaTotal() == 9);
    CHECK(g.inventario().vazio());
    REQUIRE(static_cast<bool>(g.armaEquipada()));
    CHECK(g.armaEquipada()->nome() == "Espada");

    SUBCASE("trocar de arma devolve a anterior ao inventario") {
        g.inventario().adicionar(std::make_shared<Arma>("Machado", 200, 12));
        g.equipar("Machado");
        CHECK(g.poderDeAtaque() == 22);
        CHECK(g.inventario().contem("Espada"));
    }

    SUBCASE("desequipar devolve os itens ao inventario") {
        g.desequiparArma();
        g.desequiparArmadura();
        CHECK(g.poderDeAtaque() == 10);
        CHECK(g.defesaTotal() == 4);
        CHECK(g.inventario().tamanho() == 2);
        CHECK_THROWS_AS(g.desequiparArma(), AcaoInvalida);
        CHECK_THROWS_AS(g.desequiparArmadura(), AcaoInvalida);
    }
}

TEST_CASE("equipar recusa pocao e item ausente") {
    Guerreiro g("Conan");
    g.inventario().adicionar(
        std::make_shared<Pocao>("Cura", 10, EfeitoPocao::VIDA, 10));
    CHECK_THROWS_AS(g.equipar("Cura"), AcaoInvalida);
    CHECK_THROWS_AS(g.equipar("Espada"), ItemNaoEncontrado);
    CHECK(g.inventario().contem("Cura"));
}

TEST_CASE("usarPocao consome o item e aplica o efeito") {
    Guerreiro g("Conan");
    g.receberDano(40);
    g.inventario().adicionar(
        std::make_shared<Pocao>("Cura", 10, EfeitoPocao::VIDA, 25));
    g.inventario().adicionar(std::make_shared<Arma>("Espada", 10, 1));

    CHECK(g.usarPocao("cura") == 25);
    CHECK(g.vida() == 60 - (40 - 4) + 25);
    CHECK_FALSE(g.inventario().contem("Cura"));
    CHECK_THROWS_AS(g.usarPocao("Cura"), ItemNaoEncontrado);
    CHECK_THROWS_AS(g.usarPocao("Espada"), AcaoInvalida);
}

TEST_CASE("reviver devolve metade da vida e tira metade do ouro") {
    Guerreiro g("Conan");
    g.ganharOuro(101);
    g.gastarMana(5);
    g.receberDano(1000);

    g.reviver();
    CHECK(g.estaVivo());
    CHECK(g.vida() == 30);
    CHECK(g.mana() == g.manaMax());
    CHECK(g.ouro() == 50);
}

TEST_CASE("restaurarEstado recompoe um heroi salvo") {
    Mago m("Merlin");
    m.restaurarEstado(3, 20, 15, 45, 30, 70, Atributos(3, 18, 6, 1), 250);
    CHECK(m.nivel() == 3);
    CHECK(m.experiencia() == 20);
    CHECK(m.vida() == 15);
    CHECK(m.vidaMax() == 45);
    CHECK(m.mana() == 30);
    CHECK(m.manaMax() == 70);
    CHECK(m.atributos().inteligencia == 18);
    CHECK(m.ouro() == 250);

    SUBCASE("valores incoerentes sao recusados") {
        CHECK_THROWS_AS(m.restaurarEstado(0, 0, 10, 45, 0, 70, Atributos(), 0),
                        EntradaInvalida);
        CHECK_THROWS_AS(m.restaurarEstado(1, 0, 50, 45, 0, 70, Atributos(), 0),
                        EntradaInvalida);
        CHECK_THROWS_AS(m.restaurarEstado(1, -5, 10, 45, 0, 70, Atributos(), 0),
                        EntradaInvalida);
        CHECK_THROWS_AS(m.restaurarEstado(1, 0, 10, 45, 0, 70, Atributos(), -1),
                        EntradaInvalida);
    }
}

TEST_CASE("criarHeroi monta a classe pedida sem depender da caixa") {
    CHECK(criarHeroi("guerreiro", "A")->classe() == "Guerreiro");
    CHECK(criarHeroi("MAGO", "B")->classe() == "Mago");
    CHECK(criarHeroi(" Arqueiro ", "C")->classe() == "Arqueiro");
    CHECK_THROWS_AS(criarHeroi("Ladino", "D"), EntradaInvalida);
    CHECK_THROWS_AS(criarHeroi("Mago", ""), EntradaInvalida);
    CHECK(classesDisponiveis().size() == 3);
}
