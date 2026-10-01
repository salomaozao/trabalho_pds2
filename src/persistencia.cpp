#include "persistencia.hpp"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <map>
#include <sstream>

#include "excecoes.hpp"
#include "heroi.hpp"
#include "utilitarios.hpp"

namespace rpg {

namespace {

const char* const ARQUIVO_INDICE = "indice.txt";

void escreverCampo(std::ostream& saida, const std::string& chave, int valor) {
    saida << chave << ";" << valor << "\n";
}

const std::string& obter(const std::map<std::string, std::string>& campos,
                         const std::string& chave) {
    std::map<std::string, std::string>::const_iterator it = campos.find(chave);
    if (it == campos.end()) {
        throw EntradaInvalida("campo obrigatorio ausente: " + chave);
    }
    return it->second;
}

int obterInteiro(const std::map<std::string, std::string>& campos,
                 const std::string& chave) {
    return util::paraInteiro(obter(campos, chave));
}

int obterOpcional(const std::map<std::string, std::string>& campos,
                  const std::string& chave) {
    std::map<std::string, std::string>::const_iterator it = campos.find(chave);
    return it == campos.end() ? 0 : util::paraInteiro(it->second);
}

}  // namespace

RepositorioSaves::RepositorioSaves(const std::string& diretorio)
    : diretorio_(diretorio) {
    if (!diretorio_.empty() && diretorio_[diretorio_.size() - 1] != '/') {
        diretorio_ += '/';
    }
}

const std::string& RepositorioSaves::diretorio() const { return diretorio_; }

void RepositorioSaves::salvar(const Heroi& heroi) {
    std::string caminho = caminhoDoSave(heroi.nome());
    std::ofstream arquivo(caminho.c_str());
    if (!arquivo) {
        throw ArquivoInvalido(caminho, "nao foi possivel criar o arquivo.");
    }
    escrever(heroi, arquivo);

    std::vector<std::string> nomes = listar();
    bool presente = false;
    for (std::size_t i = 0; i < nomes.size(); i++) {
        if (normalizar(nomes[i]) == normalizar(heroi.nome())) {
            presente = true;
        }
    }
    if (!presente) {
        nomes.push_back(heroi.nome());
        gravarIndice(nomes);
    }
}

std::unique_ptr<Heroi> RepositorioSaves::carregar(const std::string& nome) const {
    std::string caminho = caminhoDoSave(nome);
    std::ifstream arquivo(caminho.c_str());
    if (!arquivo) {
        throw ArquivoInvalido(caminho, "save nao encontrado.");
    }
    return ler(arquivo, caminho);
}

bool RepositorioSaves::existe(const std::string& nome) const {
    std::ifstream arquivo(caminhoDoSave(nome).c_str());
    return arquivo.good();
}

std::vector<std::string> RepositorioSaves::listar() const {
    std::vector<std::string> nomes;
    std::ifstream indice(caminhoDoIndice().c_str());
    std::string linha;
    while (std::getline(indice, linha)) {
        if (!util::linhaIgnoravel(linha)) {
            nomes.push_back(util::aparar(linha));
        }
    }
    return nomes;
}

void RepositorioSaves::remover(const std::string& nome) {
    std::string caminho = caminhoDoSave(nome);
    if (!existe(nome)) {
        throw ArquivoInvalido(caminho, "save nao encontrado.");
    }
    std::remove(caminho.c_str());

    std::vector<std::string> nomes = listar();
    std::vector<std::string> restantes;
    for (std::size_t i = 0; i < nomes.size(); i++) {
        if (normalizar(nomes[i]) != normalizar(nome)) {
            restantes.push_back(nomes[i]);
        }
    }
    gravarIndice(restantes);
}

std::string RepositorioSaves::caminhoDoSave(const std::string& nome) const {
    return diretorio_ + normalizar(nome) + ".txt";
}

void RepositorioSaves::escrever(const Heroi& heroi, std::ostream& saida) {
    const Atributos& a = heroi.atributos();
    const Estatisticas& e = heroi.estatisticas();

    saida << "# Save do RPG por Turnos\n";
    saida << "classe;" << heroi.classe() << "\n";
    saida << "nome;" << heroi.nome() << "\n";
    escreverCampo(saida, "nivel", heroi.nivel());
    escreverCampo(saida, "experiencia", heroi.experiencia());
    escreverCampo(saida, "vida", heroi.vida());
    escreverCampo(saida, "vidaMax", heroi.vidaMax());
    escreverCampo(saida, "mana", heroi.mana());
    escreverCampo(saida, "manaMax", heroi.manaMax());
    escreverCampo(saida, "forca", a.forca);
    escreverCampo(saida, "inteligencia", a.inteligencia);
    escreverCampo(saida, "destreza", a.destreza);
    escreverCampo(saida, "defesa", a.defesa);
    escreverCampo(saida, "ouro", heroi.ouro());
    escreverCampo(saida, "vitorias", e.vitorias);
    escreverCampo(saida, "derrotas", e.derrotas);
    escreverCampo(saida, "fugas", e.fugas);
    escreverCampo(saida, "danoCausado", e.danoCausado);
    escreverCampo(saida, "danoRecebido", e.danoRecebido);

    if (heroi.armaEquipada()) {
        saida << "arma;" << heroi.armaEquipada()->serializar() << "\n";
    }
    if (heroi.armaduraEquipada()) {
        saida << "armadura;" << heroi.armaduraEquipada()->serializar() << "\n";
    }
    const std::vector<std::shared_ptr<Item> >& itens = heroi.inventario().itens();
    for (std::size_t i = 0; i < itens.size(); i++) {
        saida << "item;" << itens[i]->serializar() << "\n";
    }
}

std::unique_ptr<Heroi> RepositorioSaves::ler(std::istream& entrada,
                                             const std::string& origem) {
    std::map<std::string, std::string> campos;
    std::vector<std::string> itens;
    std::string arma;
    std::string armadura;

    std::string linha;
    int numero = 0;
    while (std::getline(entrada, linha)) {
        numero++;
        if (util::linhaIgnoravel(linha)) {
            continue;
        }
        std::size_t separador = linha.find(';');
        if (separador == std::string::npos) {
            std::ostringstream motivo;
            motivo << "linha " << numero << ": falta o separador ';'.";
            throw ArquivoInvalido(origem, motivo.str());
        }
        std::string chave = util::aparar(linha.substr(0, separador));
        std::string valor = util::aparar(linha.substr(separador + 1));

        if (chave == "item") {
            itens.push_back(valor);
        } else if (chave == "arma") {
            arma = valor;
        } else if (chave == "armadura") {
            armadura = valor;
        } else {
            campos[chave] = valor;
        }
    }

    try {
        std::unique_ptr<Heroi> heroi =
            criarHeroi(obter(campos, "classe"), obter(campos, "nome"));

        Atributos atributos(
            obterInteiro(campos, "forca"), obterInteiro(campos, "inteligencia"),
            obterInteiro(campos, "destreza"), obterInteiro(campos, "defesa"));
        heroi->restaurarEstado(
            obterInteiro(campos, "nivel"), obterInteiro(campos, "experiencia"),
            obterInteiro(campos, "vida"), obterInteiro(campos, "vidaMax"),
            obterInteiro(campos, "mana"), obterInteiro(campos, "manaMax"),
            atributos, obterInteiro(campos, "ouro"));

        Estatisticas& e = heroi->estatisticas();
        e.vitorias = obterOpcional(campos, "vitorias");
        e.derrotas = obterOpcional(campos, "derrotas");
        e.fugas = obterOpcional(campos, "fugas");
        e.danoCausado = obterOpcional(campos, "danoCausado");
        e.danoRecebido = obterOpcional(campos, "danoRecebido");

        // Equipamentos passam pelo inventario antes dos itens soltos, quando
        // ainda ha vaga garantida.
        if (!arma.empty()) {
            std::shared_ptr<Item> item = criarItemDeLinha(arma);
            heroi->inventario().adicionar(item);
            heroi->equipar(item->nome());
        }
        if (!armadura.empty()) {
            std::shared_ptr<Item> item = criarItemDeLinha(armadura);
            heroi->inventario().adicionar(item);
            heroi->equipar(item->nome());
        }
        for (std::size_t i = 0; i < itens.size(); i++) {
            heroi->inventario().adicionar(criarItemDeLinha(itens[i]));
        }
        return heroi;
    } catch (const ArquivoInvalido&) {
        throw;
    } catch (const ErroDeJogo& erro) {
        throw ArquivoInvalido(origem, erro.what());
    }
}

std::string RepositorioSaves::normalizar(const std::string& nome) {
    std::string resultado;
    std::string limpo = util::paraMinusculas(util::aparar(nome));
    for (std::size_t i = 0; i < limpo.size(); i++) {
        unsigned char c = static_cast<unsigned char>(limpo[i]);
        resultado += std::isalnum(c) ? limpo[i] : '_';
    }
    if (resultado.empty()) {
        throw EntradaInvalida("nome vazio nao pode virar arquivo.");
    }
    return resultado;
}

std::string RepositorioSaves::caminhoDoIndice() const {
    return diretorio_ + ARQUIVO_INDICE;
}

void RepositorioSaves::gravarIndice(const std::vector<std::string>& nomes) const {
    std::string caminho = caminhoDoIndice();
    std::ofstream indice(caminho.c_str());
    if (!indice) {
        throw ArquivoInvalido(caminho, "nao foi possivel gravar o indice.");
    }
    indice << "# Herois salvos\n";
    for (std::size_t i = 0; i < nomes.size(); i++) {
        indice << nomes[i] << "\n";
    }
}

}  // namespace rpg
