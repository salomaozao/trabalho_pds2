#include "doctest.h"
#include "excecoes.hpp"
#include "inventario.hpp"

using namespace rpg;

namespace {

std::shared_ptr<Item> arma(const std::string& nome) {
    return std::make_shared<Arma>(nome, 10, 1);
}

std::shared_ptr<Item> pocao(const std::string& nome) {
    return std::make_shared<Pocao>(nome, 10, EfeitoPocao::VIDA, 10);
}

}  // namespace

TEST_CASE("inventario comeca vazio e respeita a capacidade") {
    Inventario inv(2);
    CHECK(inv.vazio());
    CHECK_FALSE(inv.cheio());

    inv.adicionar(arma("Espada"));
    inv.adicionar(arma("Adaga"));
    CHECK(inv.tamanho() == 2);
    CHECK(inv.cheio());
    CHECK_THROWS_AS(inv.adicionar(arma("Machado")), InventarioCheio);
}

TEST_CASE("capacidade zero e item nulo sao recusados") {
    CHECK_THROWS_AS(Inventario(0), EntradaInvalida);
    Inventario inv(3);
    CHECK_THROWS_AS(inv.adicionar(std::shared_ptr<Item>()), EntradaInvalida);
}

TEST_CASE("buscar e contem ignoram maiusculas e espacos") {
    Inventario inv(3);
    inv.adicionar(arma("Espada Longa"));
    CHECK(inv.contem("espada longa"));
    CHECK(inv.contem("  ESPADA LONGA "));
    CHECK(inv.buscar("espada longa")->nome() == "Espada Longa");
    CHECK_FALSE(inv.contem("Adaga"));
    CHECK_THROWS_AS(inv.buscar("Adaga"), ItemNaoEncontrado);
}

TEST_CASE("remover devolve o item e libera a vaga") {
    Inventario inv(2);
    inv.adicionar(arma("Espada"));
    inv.adicionar(pocao("Cura"));

    std::shared_ptr<Item> removido = inv.remover("cura");
    CHECK(removido->nome() == "Cura");
    CHECK(inv.tamanho() == 1);
    CHECK_FALSE(inv.contem("Cura"));
    CHECK_THROWS_AS(inv.remover("Cura"), ItemNaoEncontrado);
}

TEST_CASE("remover tira apenas a primeira ocorrencia de itens repetidos") {
    Inventario inv(3);
    inv.adicionar(pocao("Cura"));
    inv.adicionar(pocao("Cura"));
    inv.remover("Cura");
    CHECK(inv.contem("Cura"));
    CHECK(inv.tamanho() == 1);
}

TEST_CASE("contar agrupa por tipo e limpar esvazia") {
    Inventario inv(5);
    inv.adicionar(arma("Espada"));
    inv.adicionar(pocao("Cura"));
    inv.adicionar(pocao("Cura"));
    CHECK(inv.contar(TipoItem::ARMA) == 1);
    CHECK(inv.contar(TipoItem::POCAO) == 2);
    CHECK(inv.contar(TipoItem::ARMADURA) == 0);

    inv.limpar();
    CHECK(inv.vazio());
}
