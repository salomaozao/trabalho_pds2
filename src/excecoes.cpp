#include "excecoes.hpp"

#include <sstream>

namespace rpg {

// Funcoes auxiliares locais que montam as mensagens das excecoes. Ficam em um
// namespace anonimo porque so interessam a este arquivo.
namespace {

std::string mensagemInventarioCheio(std::size_t capacidade) {
    std::ostringstream fluxo;
    fluxo << "O inventario ja possui os " << capacidade
          << " itens que cabem nele.";
    return fluxo.str();
}

std::string mensagemRecursoInsuficiente(const std::string& recurso,
                                        int disponivel, int necessario) {
    std::ostringstream fluxo;
    fluxo << "Falta " << recurso << ": sao necessarios " << necessario
          << " e existem apenas " << disponivel << ".";
    return fluxo.str();
}

}  // namespace

ErroDeJogo::ErroDeJogo(const std::string& mensagem)
    : std::runtime_error(mensagem) {}

ArquivoInvalido::ArquivoInvalido(const std::string& caminho,
                                 const std::string& motivo)
    : ErroDeJogo("Problema no arquivo '" + caminho + "': " + motivo),
      caminho_(caminho) {}

const std::string& ArquivoInvalido::caminho() const { return caminho_; }

EntradaInvalida::EntradaInvalida(const std::string& mensagem)
    : ErroDeJogo("Entrada invalida: " + mensagem) {}

ItemNaoEncontrado::ItemNaoEncontrado(const std::string& nome)
    : ErroDeJogo("Item nao encontrado: " + nome), nome_(nome) {}

const std::string& ItemNaoEncontrado::nomeProcurado() const { return nome_; }

InventarioCheio::InventarioCheio(std::size_t capacidade)
    : ErroDeJogo(mensagemInventarioCheio(capacidade)) {}

RecursoInsuficiente::RecursoInsuficiente(const std::string& recurso,
                                         int disponivel, int necessario)
    : ErroDeJogo(mensagemRecursoInsuficiente(recurso, disponivel,
                                             necessario)) {}

PersonagemMorto::PersonagemMorto(const std::string& nome)
    : ErroDeJogo("O personagem esta fora de combate e nao pode agir: " + nome) {}

AcaoInvalida::AcaoInvalida(const std::string& mensagem)
    : ErroDeJogo("Acao invalida: " + mensagem) {}

}  // namespace rpg
