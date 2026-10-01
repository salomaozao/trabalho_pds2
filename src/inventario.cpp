#include "inventario.hpp"

#include "excecoes.hpp"
#include "utilitarios.hpp"

namespace rpg {

Inventario::Inventario(std::size_t capacidade) : capacidade_(capacidade) {
    if (capacidade == 0) {
        throw EntradaInvalida("a capacidade do inventario deve ser positiva.");
    }
}

std::size_t Inventario::capacidade() const { return capacidade_; }
std::size_t Inventario::tamanho() const { return itens_.size(); }
bool Inventario::vazio() const { return itens_.empty(); }
bool Inventario::cheio() const { return itens_.size() >= capacidade_; }

void Inventario::adicionar(std::shared_ptr<Item> item) {
    if (!item) {
        throw EntradaInvalida("item nulo nao pode ser guardado.");
    }
    if (cheio()) {
        throw InventarioCheio(capacidade_);
    }
    itens_.push_back(item);
}

std::shared_ptr<Item> Inventario::remover(const std::string& nome) {
    std::size_t i = indiceDe(nome);
    std::shared_ptr<Item> item = itens_[i];
    itens_.erase(itens_.begin() + i);
    return item;
}

std::shared_ptr<Item> Inventario::buscar(const std::string& nome) const {
    return itens_[indiceDe(nome)];
}

bool Inventario::contem(const std::string& nome) const {
    for (std::size_t i = 0; i < itens_.size(); i++) {
        if (util::iguaisSemCaixa(itens_[i]->nome(), nome)) {
            return true;
        }
    }
    return false;
}

std::size_t Inventario::contar(TipoItem tipo) const {
    std::size_t total = 0;
    for (std::size_t i = 0; i < itens_.size(); i++) {
        if (itens_[i]->tipo() == tipo) {
            total++;
        }
    }
    return total;
}

const std::vector<std::shared_ptr<Item> >& Inventario::itens() const {
    return itens_;
}

void Inventario::limpar() { itens_.clear(); }

std::size_t Inventario::indiceDe(const std::string& nome) const {
    for (std::size_t i = 0; i < itens_.size(); i++) {
        if (util::iguaisSemCaixa(itens_[i]->nome(), nome)) {
            return i;
        }
    }
    throw ItemNaoEncontrado(nome);
}

}  // namespace rpg
