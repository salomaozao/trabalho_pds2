#include "catalogo.hpp"
#include "doctest.h"
#include "excecoes.hpp"
#include "heroi.hpp"
#include "loja.hpp"

using namespace rpg;

namespace {

CatalogoItens catalogoDeTeste() {
    CatalogoItens catalogo;
    catalogo.adicionar(std::make_shared<Arma>("Espada", 60, 4));
    catalogo.adicionar(std::make_shared<Pocao>("Cura", 25, EfeitoPocao::VIDA, 30));
    return catalogo;
}

}  // namespace

TEST_CASE("comprar cobra o preco e entrega uma copia do item") {
    CatalogoItens catalogo = catalogoDeTeste();
    Loja loja(catalogo);
    Guerreiro heroi("Conan");
    heroi.ganharOuro(100);

    std::shared_ptr<Item> item = loja.comprar(heroi, "espada");
    CHECK(item->nome() == "Espada");
    CHECK(heroi.ouro() == 40);
    CHECK(heroi.inventario().contem("Espada"));
    CHECK(&loja.catalogo() == &catalogo);
}

TEST_CASE("comprar falha sem ouro, sem espaco ou sem o item") {
    CatalogoItens catalogo = catalogoDeTeste();
    Loja loja(catalogo);
    Guerreiro heroi("Conan");
    heroi.ganharOuro(30);

    CHECK_THROWS_AS(loja.comprar(heroi, "Espada"), RecursoInsuficiente);
    CHECK_THROWS_AS(loja.comprar(heroi, "Machado"), ItemNaoEncontrado);
    CHECK(heroi.ouro() == 30);
    CHECK(heroi.inventario().vazio());

    heroi.ganharOuro(1000);
    while (!heroi.inventario().cheio()) {
        loja.comprar(heroi, "Cura");
    }
    int ouroAntes = heroi.ouro();
    CHECK_THROWS_AS(loja.comprar(heroi, "Cura"), InventarioCheio);
    CHECK(heroi.ouro() == ouroAntes);
}

TEST_CASE("vender paga metade do preco e tira o item do inventario") {
    CatalogoItens catalogo = catalogoDeTeste();
    Loja loja(catalogo);
    Guerreiro heroi("Conan");
    heroi.inventario().adicionar(std::make_shared<Arma>("Espada", 61, 4));

    CHECK(Loja::precoDeVenda(*heroi.inventario().buscar("Espada")) == 30);
    CHECK(loja.vender(heroi, "espada") == 30);
    CHECK(heroi.ouro() == 30);
    CHECK(heroi.inventario().vazio());
    CHECK_THROWS_AS(loja.vender(heroi, "Espada"), ItemNaoEncontrado);
}
