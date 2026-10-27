#include "loja.hpp"

#include "catalogo.hpp"
#include "excecoes.hpp"
#include "heroi.hpp"

namespace rpg {

Loja::Loja(const CatalogoItens& catalogo) : catalogo_(catalogo) {}

const CatalogoItens& Loja::catalogo() const { return catalogo_; }

std::shared_ptr<Item> Loja::comprar(Heroi& heroi, const std::string& nome) {
    std::shared_ptr<Item> item = catalogo_.criar(nome);
    if (heroi.inventario().cheio()) {
        throw InventarioCheio(heroi.inventario().capacidade());
    }
    heroi.gastarOuro(precoDeCompra(*item, heroi.nivel()));
    heroi.inventario().adicionar(item);
    return item;
}

int Loja::vender(Heroi& heroi, const std::string& nome) {
    std::shared_ptr<Item> item = heroi.inventario().remover(nome);
    int valor = precoDeVenda(*item);
    heroi.ganharOuro(valor);
    return valor;
}

int Loja::precoDeVenda(const Item& item) { return item.preco() / 2; }

int Loja::precoDeCompra(const Item& item, int nivel) {
    int desconto = (nivel - 1) * 5;
    if (desconto > 25) {
        desconto = 25;
    }
    if (desconto < 0) {
        desconto = 0;
    }
    return item.preco() * (100 - desconto) / 100;
}

}  // namespace rpg
