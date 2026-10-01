#include "dado.hpp"

#include "excecoes.hpp"

namespace rpg {

namespace {

void validarLados(int lados) {
    if (lados < 1) {
        throw EntradaInvalida("um dado precisa ter pelo menos uma face.");
    }
}

}  // namespace

Dado::~Dado() {}

int Dado::rolarVarios(int quantidade, int lados) const {
    if (quantidade < 1) {
        throw EntradaInvalida("a quantidade de dados deve ser positiva.");
    }
    int soma = 0;
    for (int i = 0; i < quantidade; i++) {
        soma += rolar(lados);
    }
    return soma;
}

DadoAleatorio::DadoAleatorio() : gerador_(std::random_device()()) {}

DadoAleatorio::DadoAleatorio(unsigned int semente) : gerador_(semente) {}

int DadoAleatorio::rolar(int lados) const {
    validarLados(lados);
    std::uniform_int_distribution<int> distribuicao(1, lados);
    return distribuicao(gerador_);
}

DadoFixo::DadoFixo(int valor)
    : sequencia_(1, valor), posicao_(0), rolagens_(0) {
    if (valor < 1) {
        throw EntradaInvalida("o valor fixo do dado deve ser positivo.");
    }
}

DadoFixo::DadoFixo(const std::vector<int>& sequencia)
    : sequencia_(sequencia), posicao_(0), rolagens_(0) {
    if (sequencia_.empty()) {
        throw EntradaInvalida("a sequencia do dado fixo esta vazia.");
    }
    for (std::size_t i = 0; i < sequencia_.size(); i++) {
        if (sequencia_[i] < 1) {
            throw EntradaInvalida("a sequencia do dado fixo tem valor nao positivo.");
        }
    }
}

int DadoFixo::rolar(int lados) const {
    validarLados(lados);
    int valor = sequencia_[posicao_];
    posicao_ = (posicao_ + 1) % sequencia_.size();
    rolagens_++;
    return valor > lados ? lados : valor;
}

std::size_t DadoFixo::rolagens() const { return rolagens_; }

}  // namespace rpg
