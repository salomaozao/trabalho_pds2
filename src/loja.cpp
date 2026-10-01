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
    heroi.gastarOuro(item->preco());
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

}  // namespace rpg
