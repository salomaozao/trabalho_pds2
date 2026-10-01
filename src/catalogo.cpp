#include "catalogo.hpp"

#include <fstream>
#include <sstream>

#include "dado.hpp"
#include "excecoes.hpp"
#include "utilitarios.hpp"

namespace rpg {

namespace {

std::string descreverLinha(int numero, const std::string& motivo) {
    std::ostringstream fluxo;
    fluxo << "linha " << numero << ": " << motivo;
    return fluxo.str();
}

}  // namespace

// ------------------------------------------------------- CatalogoItens

void CatalogoItens::carregarArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho.c_str());
    if (!arquivo) {
        throw ArquivoInvalido(caminho, "nao foi possivel abrir.");
    }
    carregar(arquivo, caminho);
}

void CatalogoItens::carregar(std::istream& entrada, const std::string& origem) {
    std::string linha;
    int numero = 0;
    while (std::getline(entrada, linha)) {
        numero++;
        if (util::linhaIgnoravel(linha)) {
            continue;
        }
        try {
            adicionar(criarItemDeLinha(linha));
        } catch (const EntradaInvalida& erro) {
            throw ArquivoInvalido(origem, descreverLinha(numero, erro.what()));
        }
    }
}

void CatalogoItens::adicionar(std::shared_ptr<Item> item) {
    if (!item) {
        throw EntradaInvalida("item nulo nao pode entrar no catalogo.");
    }
    if (contem(item->nome())) {
        throw EntradaInvalida("item duplicado no catalogo: " + item->nome());
    }
    itens_.push_back(item);
}

std::shared_ptr<Item> CatalogoItens::criar(const std::string& nome) const {
    for (std::size_t i = 0; i < itens_.size(); i++) {
        if (util::iguaisSemCaixa(itens_[i]->nome(), nome)) {
            return itens_[i]->clonar();
        }
    }
    throw ItemNaoEncontrado(nome);
}

bool CatalogoItens::contem(const std::string& nome) const {
    for (std::size_t i = 0; i < itens_.size(); i++) {
        if (util::iguaisSemCaixa(itens_[i]->nome(), nome)) {
            return true;
        }
    }
    return false;
}

std::size_t CatalogoItens::tamanho() const { return itens_.size(); }

const std::vector<std::shared_ptr<Item> >& CatalogoItens::itens() const {
    return itens_;
}

std::vector<std::shared_ptr<Item> > CatalogoItens::porTipo(TipoItem tipo) const {
    std::vector<std::shared_ptr<Item> > resultado;
    for (std::size_t i = 0; i < itens_.size(); i++) {
        if (itens_[i]->tipo() == tipo) {
            resultado.push_back(itens_[i]);
        }
    }
    return resultado;
}

// ---------------------------------------------------- CatalogoMonstros

void CatalogoMonstros::carregarArquivo(const std::string& caminho) {
    std::ifstream arquivo(caminho.c_str());
    if (!arquivo) {
        throw ArquivoInvalido(caminho, "nao foi possivel abrir.");
    }
    carregar(arquivo, caminho);
}

void CatalogoMonstros::carregar(std::istream& entrada,
                                const std::string& origem) {
    std::string linha;
    int numero = 0;
    while (std::getline(entrada, linha)) {
        numero++;
        if (util::linhaIgnoravel(linha)) {
            continue;
        }
        try {
            adicionar(criarMonstroDeLinha(linha));
        } catch (const EntradaInvalida& erro) {
            throw ArquivoInvalido(origem, descreverLinha(numero, erro.what()));
        }
    }
}

void CatalogoMonstros::adicionar(const Monstro& monstro) {
    if (contem(monstro.nome())) {
        throw EntradaInvalida("monstro duplicado no catalogo: " + monstro.nome());
    }
    monstros_.push_back(monstro);
}

Monstro CatalogoMonstros::criar(const std::string& nome) const {
    for (std::size_t i = 0; i < monstros_.size(); i++) {
        if (util::iguaisSemCaixa(monstros_[i].nome(), nome)) {
            Monstro copia = monstros_[i];
            copia.restaurarCompletamente();
            return copia;
        }
    }
    throw ItemNaoEncontrado(nome);
}

bool CatalogoMonstros::contem(const std::string& nome) const {
    for (std::size_t i = 0; i < monstros_.size(); i++) {
        if (util::iguaisSemCaixa(monstros_[i].nome(), nome)) {
            return true;
        }
    }
    return false;
}

std::size_t CatalogoMonstros::tamanho() const { return monstros_.size(); }

const std::vector<Monstro>& CatalogoMonstros::todos() const { return monstros_; }

std::vector<Monstro> CatalogoMonstros::ateNivel(int nivelMaximo) const {
    std::vector<Monstro> resultado;
    for (std::size_t i = 0; i < monstros_.size(); i++) {
        if (monstros_[i].nivel() <= nivelMaximo) {
            resultado.push_back(monstros_[i]);
        }
    }
    return resultado;
}

Monstro CatalogoMonstros::sortear(int nivelMaximo, const Dado& dado) const {
    if (monstros_.empty()) {
        throw AcaoInvalida("nao ha monstros cadastrados.");
    }

    std::vector<Monstro> candidatos = ateNivel(nivelMaximo);
    if (candidatos.empty()) {
        int menorNivel = monstros_[0].nivel();
        for (std::size_t i = 1; i < monstros_.size(); i++) {
            if (monstros_[i].nivel() < menorNivel) {
                menorNivel = monstros_[i].nivel();
            }
        }
        candidatos = ateNivel(menorNivel);
    }

    int indice = dado.rolar(static_cast<int>(candidatos.size())) - 1;
    Monstro escolhido = candidatos[indice];
    escolhido.restaurarCompletamente();
    return escolhido;
}

}  // namespace rpg
