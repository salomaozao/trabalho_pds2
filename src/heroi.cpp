#include "heroi.hpp"

#include "dado.hpp"
#include "excecoes.hpp"
#include "utilitarios.hpp"

namespace rpg {

Estatisticas::Estatisticas()
    : vitorias(0), derrotas(0), fugas(0), danoCausado(0), danoRecebido(0) {}

Heroi::Heroi(const std::string& nome, int vidaMax, int manaMax,
             const Atributos& atributos)
    : Personagem(nome, vidaMax, manaMax, atributos),
      experiencia_(0),
      ouro_(0),
      inventario_(8) {}

int Heroi::experiencia() const { return experiencia_; }
int Heroi::experienciaParaProximoNivel() const { return nivel() * 50; }
int Heroi::ouro() const { return ouro_; }
const Inventario& Heroi::inventario() const { return inventario_; }
Inventario& Heroi::inventario() { return inventario_; }
const Estatisticas& Heroi::estatisticas() const { return estatisticas_; }
Estatisticas& Heroi::estatisticas() { return estatisticas_; }
std::shared_ptr<Arma> Heroi::armaEquipada() const { return arma_; }
std::shared_ptr<Armadura> Heroi::armaduraEquipada() const { return armadura_; }

int Heroi::ganharExperiencia(int quantidade) {
    if (quantidade < 0) {
        throw EntradaInvalida("experiencia nao pode ser negativa.");
    }
    experiencia_ += quantidade;
    int niveis = 0;
    while (experiencia_ >= experienciaParaProximoNivel()) {
        experiencia_ -= experienciaParaProximoNivel();
        subirNivel();
        niveis++;
    }
    return niveis;
}

void Heroi::ganharOuro(int quantidade) {
    if (quantidade < 0) {
        throw EntradaInvalida("ouro recebido nao pode ser negativo.");
    }
    ouro_ += quantidade;
}

void Heroi::gastarOuro(int quantidade) {
    if (quantidade < 0) {
        throw EntradaInvalida("gasto de ouro nao pode ser negativo.");
    }
    if (quantidade > ouro_) {
        throw RecursoInsuficiente("ouro", ouro_, quantidade);
    }
    ouro_ -= quantidade;
}

void Heroi::equipar(const std::string& nome) {
    std::shared_ptr<Item> item = inventario_.buscar(nome);

    if (item->tipo() == TipoItem::POCAO) {
        throw AcaoInvalida("pocoes nao podem ser equipadas, apenas usadas.");
    }

    // O slot que vai ser trocado libera uma vaga no inventario, entao o
    // equipamento antigo sempre cabe de volta.
    inventario_.remover(nome);
    if (item->tipo() == TipoItem::ARMA) {
        if (arma_) {
            inventario_.adicionar(arma_);
        }
        arma_ = std::dynamic_pointer_cast<Arma>(item);
    } else {
        if (armadura_) {
            inventario_.adicionar(armadura_);
        }
        armadura_ = std::dynamic_pointer_cast<Armadura>(item);
    }
}

void Heroi::desequiparArma() {
    if (!arma_) {
        throw AcaoInvalida("nao ha arma equipada.");
    }
    inventario_.adicionar(arma_);
    arma_.reset();
}

void Heroi::desequiparArmadura() {
    if (!armadura_) {
        throw AcaoInvalida("nao ha armadura equipada.");
    }
    inventario_.adicionar(armadura_);
    armadura_.reset();
}

int Heroi::usarPocao(const std::string& nome) {
    std::shared_ptr<Pocao> pocao =
        std::dynamic_pointer_cast<Pocao>(inventario_.buscar(nome));
    if (!pocao) {
        throw AcaoInvalida("'" + nome + "' nao e uma pocao.");
    }
    int recuperado = pocao->aplicar(*this);
    inventario_.remover(nome);
    return recuperado;
}

int Heroi::usarHabilidade(const Dado& dado) {
    if (!estaVivo()) {
        throw PersonagemMorto(nome());
    }
    gastarMana(custoHabilidade());
    return calcularDanoHabilidade(dado);
}

void Heroi::reviver() {
    restaurarCompletamente();
    definirVida(vidaMax() / 2 > 0 ? vidaMax() / 2 : 1);
    ouro_ /= 2;
}

void Heroi::restaurarEstado(int nivel, int experiencia, int vida, int vidaMax,
                            int mana, int manaMax, const Atributos& atributos,
                            int ouro) {
    if (experiencia < 0 || ouro < 0) {
        throw EntradaInvalida("experiencia e ouro nao podem ser negativos.");
    }
    definirNivel(nivel);
    definirVidaMax(vidaMax);
    definirManaMax(manaMax);
    definirVida(vida);
    definirMana(mana);
    definirAtributos(atributos);
    experiencia_ = experiencia;
    ouro_ = ouro;
}

int Heroi::poderDeAtaque() const {
    return atributoPrimario() + (arma_ ? arma_->bonusAtaque() : 0);
}

int Heroi::defesaTotal() const {
    return atributos().defesa + (armadura_ ? armadura_->bonusDefesa() : 0);
}

void Heroi::subirNivel() {
    definirNivel(nivel() + 1);
    aoSubirNivel();
    restaurarCompletamente();
}

// ----------------------------------------------------------- Guerreiro

Guerreiro::Guerreiro(const std::string& nome)
    : Heroi(nome, 60, 15, Atributos(10, 3, 5, 4)) {}

std::string Guerreiro::classe() const { return "Guerreiro"; }
int Guerreiro::atributoPrimario() const { return atributos().forca; }
std::string Guerreiro::nomeHabilidade() const { return "Golpe Devastador"; }
int Guerreiro::custoHabilidade() const { return 8; }

int Guerreiro::calcularDanoHabilidade(const Dado& dado) const {
    return 2 * poderDeAtaque() + dado.rolar(6);
}

void Guerreiro::aoSubirNivel() {
    definirVidaMax(vidaMax() + 10);
    definirManaMax(manaMax() + 2);
    aumentarAtributos(3, 0, 1, 1);
}

// ---------------------------------------------------------------- Mago

Mago::Mago(const std::string& nome)
    : Heroi(nome, 35, 50, Atributos(3, 12, 4, 1)) {}

std::string Mago::classe() const { return "Mago"; }
int Mago::atributoPrimario() const { return atributos().inteligencia; }
std::string Mago::nomeHabilidade() const { return "Bola de Fogo"; }
int Mago::custoHabilidade() const { return 15; }

int Mago::calcularDanoHabilidade(const Dado& dado) const {
    return poderDeAtaque() + dado.rolarVarios(2, 10);
}

void Mago::aoSubirNivel() {
    definirVidaMax(vidaMax() + 5);
    definirManaMax(manaMax() + 10);
    aumentarAtributos(0, 3, 1, 0);
}

// ------------------------------------------------------------ Arqueiro

Arqueiro::Arqueiro(const std::string& nome)
    : Heroi(nome, 45, 25, Atributos(5, 5, 11, 2)) {}

std::string Arqueiro::classe() const { return "Arqueiro"; }
int Arqueiro::atributoPrimario() const { return atributos().destreza; }
std::string Arqueiro::nomeHabilidade() const { return "Flecha Dupla"; }
int Arqueiro::custoHabilidade() const { return 10; }

int Arqueiro::calcularDanoHabilidade(const Dado& dado) const {
    return calcularDano(dado) + calcularDano(dado);
}

void Arqueiro::aoSubirNivel() {
    definirVidaMax(vidaMax() + 7);
    definirManaMax(manaMax() + 5);
    aumentarAtributos(1, 0, 3, 1);
}

// ------------------------------------------------------------- Fabrica

std::unique_ptr<Heroi> criarHeroi(const std::string& classe,
                                  const std::string& nome) {
    if (util::iguaisSemCaixa(classe, "Guerreiro")) {
        return std::unique_ptr<Heroi>(new Guerreiro(nome));
    }
    if (util::iguaisSemCaixa(classe, "Mago")) {
        return std::unique_ptr<Heroi>(new Mago(nome));
    }
    if (util::iguaisSemCaixa(classe, "Arqueiro")) {
        return std::unique_ptr<Heroi>(new Arqueiro(nome));
    }
    throw EntradaInvalida("classe desconhecida: " + classe);
}

std::vector<std::string> classesDisponiveis() {
    std::vector<std::string> classes;
    classes.push_back("Guerreiro");
    classes.push_back("Mago");
    classes.push_back("Arqueiro");
    return classes;
}

}  // namespace rpg
