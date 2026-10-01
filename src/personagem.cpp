#include "personagem.hpp"

#include <sstream>

#include "dado.hpp"
#include "excecoes.hpp"
#include "utilitarios.hpp"

namespace rpg {

Atributos::Atributos() : forca(0), inteligencia(0), destreza(0), defesa(0) {}

Atributos::Atributos(int forca, int inteligencia, int destreza, int defesa)
    : forca(forca),
      inteligencia(inteligencia),
      destreza(destreza),
      defesa(defesa) {
    if (forca < 0 || inteligencia < 0 || destreza < 0 || defesa < 0) {
        throw EntradaInvalida("atributos nao podem ser negativos.");
    }
}

Personagem::Personagem(const std::string& nome, int vidaMax, int manaMax,
                       const Atributos& atributos)
    : nome_(util::aparar(nome)),
      vidaMax_(vidaMax),
      vida_(vidaMax),
      manaMax_(manaMax),
      mana_(manaMax),
      nivel_(1),
      atributos_(atributos) {
    if (nome_.empty()) {
        throw EntradaInvalida("o nome do personagem nao pode ser vazio.");
    }
    if (vidaMax < 1) {
        throw EntradaInvalida("a vida maxima deve ser positiva.");
    }
    if (manaMax < 0) {
        throw EntradaInvalida("a mana maxima nao pode ser negativa.");
    }
}

Personagem::~Personagem() {}

const std::string& Personagem::nome() const { return nome_; }
int Personagem::vida() const { return vida_; }
int Personagem::vidaMax() const { return vidaMax_; }
int Personagem::mana() const { return mana_; }
int Personagem::manaMax() const { return manaMax_; }
int Personagem::nivel() const { return nivel_; }
const Atributos& Personagem::atributos() const { return atributos_; }
bool Personagem::estaVivo() const { return vida_ > 0; }

int Personagem::defesaTotal() const { return atributos_.defesa; }

int Personagem::calcularDano(const Dado& dado) const {
    if (!estaVivo()) {
        throw PersonagemMorto(nome_);
    }
    return poderDeAtaque() + dado.rolar(6);
}

int Personagem::receberDano(int danoBruto) {
    if (danoBruto < 0) {
        throw EntradaInvalida("dano nao pode ser negativo.");
    }
    if (!estaVivo()) {
        throw PersonagemMorto(nome_);
    }
    int efetivo = danoBruto - defesaTotal();
    if (efetivo < 1) {
        efetivo = 1;
    }
    if (efetivo > vida_) {
        efetivo = vida_;
    }
    vida_ -= efetivo;
    return efetivo;
}

int Personagem::curar(int quantidade) {
    if (quantidade < 0) {
        throw EntradaInvalida("cura nao pode ser negativa.");
    }
    if (!estaVivo()) {
        throw PersonagemMorto(nome_);
    }
    int antes = vida_;
    vida_ = util::limitar(vida_ + quantidade, 0, vidaMax_);
    return vida_ - antes;
}

void Personagem::gastarMana(int quantidade) {
    if (quantidade < 0) {
        throw EntradaInvalida("gasto de mana nao pode ser negativo.");
    }
    if (quantidade > mana_) {
        throw RecursoInsuficiente("mana", mana_, quantidade);
    }
    mana_ -= quantidade;
}

int Personagem::recuperarMana(int quantidade) {
    if (quantidade < 0) {
        throw EntradaInvalida("recuperacao de mana nao pode ser negativa.");
    }
    int antes = mana_;
    mana_ = util::limitar(mana_ + quantidade, 0, manaMax_);
    return mana_ - antes;
}

void Personagem::restaurarCompletamente() {
    vida_ = vidaMax_;
    mana_ = manaMax_;
}

std::string Personagem::resumo() const {
    std::ostringstream fluxo;
    fluxo << nome_ << " (" << classe() << ", Nv " << nivel_ << ") "
          << "Vida " << vida_ << "/" << vidaMax_ << " | Mana " << mana_ << "/"
          << manaMax_;
    return fluxo.str();
}

void Personagem::definirNivel(int nivel) {
    if (nivel < 1) {
        throw EntradaInvalida("nivel deve ser positivo.");
    }
    nivel_ = nivel;
}

void Personagem::definirVida(int vida) {
    if (vida < 0 || vida > vidaMax_) {
        throw EntradaInvalida("vida fora do intervalo permitido.");
    }
    vida_ = vida;
}

void Personagem::definirMana(int mana) {
    if (mana < 0 || mana > manaMax_) {
        throw EntradaInvalida("mana fora do intervalo permitido.");
    }
    mana_ = mana;
}

void Personagem::definirVidaMax(int vidaMax) {
    if (vidaMax < 1) {
        throw EntradaInvalida("a vida maxima deve ser positiva.");
    }
    vidaMax_ = vidaMax;
    if (vida_ > vidaMax_) {
        vida_ = vidaMax_;
    }
}

void Personagem::definirManaMax(int manaMax) {
    if (manaMax < 0) {
        throw EntradaInvalida("a mana maxima nao pode ser negativa.");
    }
    manaMax_ = manaMax;
    if (mana_ > manaMax_) {
        mana_ = manaMax_;
    }
}

void Personagem::definirAtributos(const Atributos& atributos) {
    atributos_ = atributos;
}

void Personagem::aumentarAtributos(int forca, int inteligencia, int destreza,
                                   int defesa) {
    atributos_.forca += forca;
    atributos_.inteligencia += inteligencia;
    atributos_.destreza += destreza;
    atributos_.defesa += defesa;
}

}  // namespace rpg
