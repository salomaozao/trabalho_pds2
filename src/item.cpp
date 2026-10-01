#include "item.hpp"

#include <sstream>

#include "excecoes.hpp"
#include "personagem.hpp"
#include "utilitarios.hpp"

namespace rpg {

std::string nomeDoTipo(TipoItem tipo) {
    switch (tipo) {
        case TipoItem::ARMA:
            return "ARMA";
        case TipoItem::ARMADURA:
            return "ARMADURA";
        case TipoItem::POCAO:
            return "POCAO";
    }
    return "";
}

std::string nomeDoEfeito(EfeitoPocao efeito) {
    return efeito == EfeitoPocao::VIDA ? "VIDA" : "MANA";
}

Item::Item(const std::string& nome, int preco)
    : nome_(util::aparar(nome)), preco_(preco) {
    if (nome_.empty()) {
        throw EntradaInvalida("o nome do item nao pode ser vazio.");
    }
    if (preco < 0) {
        throw EntradaInvalida("o preco do item nao pode ser negativo.");
    }
}

Item::~Item() {}

const std::string& Item::nome() const { return nome_; }
int Item::preco() const { return preco_; }

std::string Item::serializar() const {
    std::ostringstream fluxo;
    fluxo << nomeDoTipo(tipo()) << ";" << nome_ << ";" << preco_
          << camposExtras();
    return fluxo.str();
}

// ---------------------------------------------------------------- Arma

Arma::Arma(const std::string& nome, int preco, int bonusAtaque)
    : Item(nome, preco), bonusAtaque_(bonusAtaque) {
    if (bonusAtaque < 0) {
        throw EntradaInvalida("o bonus de ataque nao pode ser negativo.");
    }
}

int Arma::bonusAtaque() const { return bonusAtaque_; }
TipoItem Arma::tipo() const { return TipoItem::ARMA; }

std::string Arma::descricao() const {
    std::ostringstream fluxo;
    fluxo << "Arma, +" << bonusAtaque_ << " de ataque";
    return fluxo.str();
}

std::shared_ptr<Item> Arma::clonar() const {
    return std::make_shared<Arma>(*this);
}

std::string Arma::camposExtras() const {
    std::ostringstream fluxo;
    fluxo << ";" << bonusAtaque_;
    return fluxo.str();
}

// ------------------------------------------------------------ Armadura

Armadura::Armadura(const std::string& nome, int preco, int bonusDefesa)
    : Item(nome, preco), bonusDefesa_(bonusDefesa) {
    if (bonusDefesa < 0) {
        throw EntradaInvalida("o bonus de defesa nao pode ser negativo.");
    }
}

int Armadura::bonusDefesa() const { return bonusDefesa_; }
TipoItem Armadura::tipo() const { return TipoItem::ARMADURA; }

std::string Armadura::descricao() const {
    std::ostringstream fluxo;
    fluxo << "Armadura, +" << bonusDefesa_ << " de defesa";
    return fluxo.str();
}

std::shared_ptr<Item> Armadura::clonar() const {
    return std::make_shared<Armadura>(*this);
}

std::string Armadura::camposExtras() const {
    std::ostringstream fluxo;
    fluxo << ";" << bonusDefesa_;
    return fluxo.str();
}

// --------------------------------------------------------------- Pocao

Pocao::Pocao(const std::string& nome, int preco, EfeitoPocao efeito,
             int quantidade)
    : Item(nome, preco), efeito_(efeito), quantidade_(quantidade) {
    if (quantidade < 1) {
        throw EntradaInvalida("a quantidade da pocao deve ser positiva.");
    }
}

EfeitoPocao Pocao::efeito() const { return efeito_; }
int Pocao::quantidade() const { return quantidade_; }

int Pocao::aplicar(Personagem& alvo) const {
    if (efeito_ == EfeitoPocao::VIDA) {
        return alvo.curar(quantidade_);
    }
    return alvo.recuperarMana(quantidade_);
}

TipoItem Pocao::tipo() const { return TipoItem::POCAO; }

std::string Pocao::descricao() const {
    std::ostringstream fluxo;
    fluxo << "Pocao, recupera " << quantidade_ << " de "
          << util::paraMinusculas(nomeDoEfeito(efeito_));
    return fluxo.str();
}

std::shared_ptr<Item> Pocao::clonar() const {
    return std::make_shared<Pocao>(*this);
}

std::string Pocao::camposExtras() const {
    std::ostringstream fluxo;
    fluxo << ";" << nomeDoEfeito(efeito_) << ";" << quantidade_;
    return fluxo.str();
}

// ------------------------------------------------------------- Fabrica

std::shared_ptr<Item> criarItem(const std::vector<std::string>& campos) {
    if (campos.size() < 4) {
        throw EntradaInvalida("linha de item com menos de 4 campos.");
    }

    std::string tipo = util::paraMaiusculas(campos[0]);
    const std::string& nome = campos[1];
    int preco = util::paraInteiro(campos[2]);

    if (tipo == "ARMA") {
        return std::make_shared<Arma>(nome, preco, util::paraInteiro(campos[3]));
    }
    if (tipo == "ARMADURA") {
        return std::make_shared<Armadura>(nome, preco,
                                          util::paraInteiro(campos[3]));
    }
    if (tipo == "POCAO") {
        if (campos.size() < 5) {
            throw EntradaInvalida("pocao precisa de efeito e quantidade.");
        }
        std::string efeito = util::paraMaiusculas(campos[3]);
        if (efeito != "VIDA" && efeito != "MANA") {
            throw EntradaInvalida("efeito de pocao desconhecido: " + campos[3]);
        }
        return std::make_shared<Pocao>(
            nome, preco, efeito == "VIDA" ? EfeitoPocao::VIDA : EfeitoPocao::MANA,
            util::paraInteiro(campos[4]));
    }
    throw EntradaInvalida("tipo de item desconhecido: " + campos[0]);
}

std::shared_ptr<Item> criarItemDeLinha(const std::string& linha) {
    return criarItem(util::dividir(linha, ';'));
}

}  // namespace rpg
