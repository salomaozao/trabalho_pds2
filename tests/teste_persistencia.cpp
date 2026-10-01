#include <sstream>

#include "doctest.h"
#include "excecoes.hpp"
#include "heroi.hpp"
#include "persistencia.hpp"

using namespace rpg;

namespace {

std::unique_ptr<Heroi> heroiCompleto() {
    std::unique_ptr<Heroi> heroi = criarHeroi("Arqueiro", "Legolas");
    heroi->ganharExperiencia(70);
    heroi->ganharOuro(130);
    heroi->receberDano(10);
    heroi->gastarMana(5);
    heroi->estatisticas().vitorias = 3;
    heroi->estatisticas().danoCausado = 99;
    heroi->inventario().adicionar(std::make_shared<Arma>("Arco", 65, 4));
    heroi->inventario().adicionar(std::make_shared<Armadura>("Couro", 60, 3));
    heroi->inventario().adicionar(
        std::make_shared<Pocao>("Cura", 25, EfeitoPocao::VIDA, 30));
    heroi->equipar("Arco");
    heroi->equipar("Couro");
    return heroi;
}

}  // namespace

TEST_CASE("escrever e ler recompoem o heroi por completo") {
    std::unique_ptr<Heroi> original = heroiCompleto();

    std::stringstream fluxo;
    RepositorioSaves::escrever(*original, fluxo);
    std::unique_ptr<Heroi> lido = RepositorioSaves::ler(fluxo, "memoria");

    CHECK(lido->classe() == "Arqueiro");
    CHECK(lido->nome() == "Legolas");
    CHECK(lido->nivel() == original->nivel());
    CHECK(lido->experiencia() == original->experiencia());
    CHECK(lido->vida() == original->vida());
    CHECK(lido->vidaMax() == original->vidaMax());
    CHECK(lido->mana() == original->mana());
    CHECK(lido->manaMax() == original->manaMax());
    CHECK(lido->ouro() == 130);
    CHECK(lido->atributos().destreza == original->atributos().destreza);
    CHECK(lido->estatisticas().vitorias == 3);
    CHECK(lido->estatisticas().danoCausado == 99);
    REQUIRE(static_cast<bool>(lido->armaEquipada()));
    CHECK(lido->armaEquipada()->nome() == "Arco");
    REQUIRE(static_cast<bool>(lido->armaduraEquipada()));
    CHECK(lido->armaduraEquipada()->bonusDefesa() == 3);
    CHECK(lido->inventario().tamanho() == 1);
    CHECK(lido->inventario().contem("Cura"));
    CHECK(lido->poderDeAtaque() == original->poderDeAtaque());
}

TEST_CASE("ler aceita save minimo sem estatisticas nem itens") {
    std::istringstream fluxo(
        "classe;Mago\nnome;Merlin\nnivel;2\nexperiencia;10\nvida;20\n"
        "vidaMax;40\nmana;30\nmanaMax;60\nforca;3\ninteligencia;15\n"
        "destreza;5\ndefesa;1\nouro;7\n");
    std::unique_ptr<Heroi> heroi = RepositorioSaves::ler(fluxo, "m");
    CHECK(heroi->classe() == "Mago");
    CHECK(heroi->nivel() == 2);
    CHECK(heroi->estatisticas().vitorias == 0);
    CHECK(heroi->inventario().vazio());
    CHECK_FALSE(static_cast<bool>(heroi->armaEquipada()));
}

TEST_CASE("ler rejeita saves corrompidos") {
    SUBCASE("sem separador") {
        std::istringstream fluxo("classe Mago\n");
        CHECK_THROWS_AS(RepositorioSaves::ler(fluxo, "x"), ArquivoInvalido);
    }
    SUBCASE("campo obrigatorio ausente") {
        std::istringstream fluxo("classe;Mago\nnome;Merlin\n");
        CHECK_THROWS_AS(RepositorioSaves::ler(fluxo, "x"), ArquivoInvalido);
    }
    SUBCASE("classe desconhecida") {
        std::istringstream fluxo("classe;Bardo\nnome;X\n");
        CHECK_THROWS_AS(RepositorioSaves::ler(fluxo, "x"), ArquivoInvalido);
    }
    SUBCASE("vida maior que o maximo") {
        std::istringstream fluxo(
            "classe;Mago\nnome;Merlin\nnivel;1\nexperiencia;0\nvida;99\n"
            "vidaMax;40\nmana;0\nmanaMax;60\nforca;3\ninteligencia;15\n"
            "destreza;5\ndefesa;1\nouro;0\n");
        CHECK_THROWS_AS(RepositorioSaves::ler(fluxo, "x"), ArquivoInvalido);
    }
    SUBCASE("item invalido") {
        std::istringstream fluxo(
            "classe;Mago\nnome;Merlin\nnivel;1\nexperiencia;0\nvida;10\n"
            "vidaMax;40\nmana;0\nmanaMax;60\nforca;3\ninteligencia;15\n"
            "destreza;5\ndefesa;1\nouro;0\nitem;ESCUDO;X;1;1\n");
        CHECK_THROWS_AS(RepositorioSaves::ler(fluxo, "x"), ArquivoInvalido);
    }
}

TEST_CASE("normalizar gera nomes de arquivo seguros") {
    CHECK(RepositorioSaves::normalizar("Legolas") == "legolas");
    CHECK(RepositorioSaves::normalizar("  Sir Lancelot du Lac ") ==
          "sir_lancelot_du_lac");
    CHECK(RepositorioSaves::normalizar("a/b\\c:d") == "a_b_c_d");
    CHECK_THROWS_AS(RepositorioSaves::normalizar("   "), EntradaInvalida);
}

TEST_CASE("repositorio salva, lista, carrega e remove em disco") {
    RepositorioSaves repo("build");
    std::unique_ptr<Heroi> heroi = heroiCompleto();

    CHECK(repo.diretorio() == "build/");
    CHECK(repo.caminhoDoSave("Legolas") == "build/legolas.txt");

    repo.salvar(*heroi);
    CHECK(repo.existe("legolas"));
    REQUIRE(repo.listar().size() == 1);
    CHECK(repo.listar()[0] == "Legolas");

    SUBCASE("salvar de novo nao duplica no indice") {
        repo.salvar(*heroi);
        CHECK(repo.listar().size() == 1);
    }

    std::unique_ptr<Heroi> lido = repo.carregar("Legolas");
    CHECK(lido->nome() == "Legolas");
    CHECK(lido->ouro() == 130);

    repo.remover("Legolas");
    CHECK_FALSE(repo.existe("Legolas"));
    CHECK(repo.listar().empty());
    CHECK_THROWS_AS(repo.carregar("Legolas"), ArquivoInvalido);
    CHECK_THROWS_AS(repo.remover("Legolas"), ArquivoInvalido);
}

TEST_CASE("repositorio em diretorio inexistente falha ao salvar") {
    RepositorioSaves repo("nao/existe/aqui");
    Guerreiro heroi("Conan");
    CHECK_THROWS_AS(repo.salvar(heroi), ArquivoInvalido);
    CHECK(repo.listar().empty());
}
