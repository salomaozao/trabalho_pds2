#include "jogo.hpp"

#include <sstream>

#include "batalha.hpp"
#include "catalogo.hpp"
#include "dado.hpp"
#include "excecoes.hpp"
#include "heroi.hpp"
#include "loja.hpp"
#include "monstro.hpp"
#include "persistencia.hpp"
#include "relatorio.hpp"
#include "utilitarios.hpp"

namespace rpg {

namespace {

const int OURO_INICIAL = 50;
const char* const POCAO_INICIAL = "Pocao de Vida Pequena";

}  // namespace

Jogo::Jogo(std::istream& entrada, std::ostream& saida, const Dado& dado,
           const CatalogoItens& itens, const CatalogoMonstros& monstros,
           RepositorioSaves& saves)
    : entrada_(entrada),
      saida_(saida),
      dado_(dado),
      itens_(itens),
      monstros_(monstros),
      saves_(saves),
      encerrado_(false) {}

Jogo::~Jogo() {}

const Heroi* Jogo::heroi() const { return heroi_.get(); }

void Jogo::executar() {
    imprimirTitulo("RPG POR TURNOS");
    while (!encerrado_ && menuInicial()) {
        menuPrincipal();
    }
    imprimir("Ate a proxima!");
}

// --------------------------------------------------------------- Menus

bool Jogo::menuInicial() {
    heroi_.reset();
    while (!encerrado_) {
        imprimir("\n1. Novo heroi\n2. Carregar heroi\n3. Sair");
        int opcao = lerOpcao(1, 3);
        try {
            switch (opcao) {
                case 1:
                    novoHeroi();
                    break;
                case 2:
                    carregarHeroi();
                    break;
                default:
                    return false;
            }
        } catch (const ErroDeJogo& erro) {
            imprimirErro(erro.what());
        }
        if (heroi_) {
            return true;
        }
    }
    return false;
}

void Jogo::menuPrincipal() {
    while (!encerrado_) {
        imprimir("\n" + heroi_->resumo());
        imprimir("1. Ver ficha\n2. Inventario\n3. Loja\n4. Procurar batalha\n"
                 "5. Descansar\n6. Salvar\n7. Voltar ao menu inicial");
        int opcao = lerOpcao(1, 7);
        try {
            switch (opcao) {
                case 1:
                    imprimir(relatorio::fichaDoHeroi(*heroi_) +
                             relatorio::estatisticas(*heroi_));
                    break;
                case 2:
                    menuInventario();
                    break;
                case 3:
                    menuLoja();
                    break;
                case 4:
                    batalhar();
                    break;
                case 5:
                    descansar();
                    break;
                case 6:
                    salvar();
                    break;
                default:
                    return;
            }
        } catch (const ErroDeJogo& erro) {
            imprimirErro(erro.what());
        }
    }
}

void Jogo::menuInventario() {
    while (!encerrado_) {
        imprimir("\n" + relatorio::tabelaInventario(heroi_->inventario()));
        imprimir("Arma: " + (heroi_->armaEquipada()
                                 ? heroi_->armaEquipada()->nome()
                                 : std::string("nenhuma")) +
                 " | Armadura: " +
                 (heroi_->armaduraEquipada()
                      ? heroi_->armaduraEquipada()->nome()
                      : std::string("nenhuma")));
        imprimir("1. Equipar\n2. Desequipar arma\n3. Desequipar armadura\n"
                 "4. Usar pocao\n5. Voltar");
        int opcao = lerOpcao(1, 5);
        try {
            switch (opcao) {
                case 1: {
                    std::string nome = lerTexto("Item: ");
                    heroi_->equipar(nome);
                    imprimir(nome + " equipado.");
                    break;
                }
                case 2:
                    heroi_->desequiparArma();
                    imprimir("Arma guardada no inventario.");
                    break;
                case 3:
                    heroi_->desequiparArmadura();
                    imprimir("Armadura guardada no inventario.");
                    break;
                case 4: {
                    std::string nome = lerTexto("Pocao: ");
                    int recuperado = heroi_->usarPocao(nome);
                    std::ostringstream texto;
                    texto << "Recuperou " << recuperado << ".";
                    imprimir(texto.str());
                    break;
                }
                default:
                    return;
            }
        } catch (const ErroDeJogo& erro) {
            imprimirErro(erro.what());
        }
    }
}

void Jogo::menuLoja() {
    Loja loja(itens_);
    while (!encerrado_) {
        std::ostringstream ouro;
        ouro << "\nOuro: " << heroi_->ouro();
        imprimir(ouro.str());
        imprimir(relatorio::tabelaCatalogo(itens_));
        imprimir("1. Comprar\n2. Vender\n3. Voltar");
        int opcao = lerOpcao(1, 3);
        try {
            switch (opcao) {
                case 1: {
                    std::string nome = lerTexto("Comprar: ");
                    std::shared_ptr<Item> item = loja.comprar(*heroi_, nome);
                    std::ostringstream texto;
                    texto << "Comprou " << item->nome() << " por " << item->preco()
                          << " de ouro.";
                    imprimir(texto.str());
                    break;
                }
                case 2: {
                    std::string nome = lerTexto("Vender: ");
                    int valor = loja.vender(*heroi_, nome);
                    std::ostringstream texto;
                    texto << "Vendeu " << nome << " por " << valor << " de ouro.";
                    imprimir(texto.str());
                    break;
                }
                default:
                    return;
            }
        } catch (const ErroDeJogo& erro) {
            imprimirErro(erro.what());
        }
    }
}

// -------------------------------------------------------------- Acoes

void Jogo::batalhar() {
    Monstro monstro = monstros_.sortear(heroi_->nivel() + 1, dado_);
    Batalha batalha(*heroi_, monstro, dado_);
    std::size_t lidas = 0;

    while (!encerrado_) {
        const std::vector<std::string>& registro = batalha.registro();
        for (; lidas < registro.size(); lidas++) {
            imprimir("  " + registro[lidas]);
        }
        if (batalha.terminada()) {
            break;
        }

        imprimir(relatorio::painelDeCombate(batalha));
        std::ostringstream menu;
        menu << "1. Atacar\n2. " << heroi_->nomeHabilidade() << " ("
             << heroi_->custoHabilidade() << " de mana)\n3. Usar pocao\n4. Fugir";
        imprimir(menu.str());

        int opcao = lerOpcao(1, 4);
        try {
            switch (opcao) {
                case 1:
                    batalha.executarRodada(AcaoDeCombate::ATACAR);
                    break;
                case 2:
                    batalha.executarRodada(AcaoDeCombate::HABILIDADE);
                    break;
                case 3:
                    batalha.executarRodada(AcaoDeCombate::POCAO,
                                           lerTexto("Pocao: "));
                    break;
                case 4:
                    batalha.executarRodada(AcaoDeCombate::FUGIR);
                    break;
                default:
                    return;
            }
        } catch (const ErroDeJogo& erro) {
            imprimirErro(erro.what());
        }
    }

    if (batalha.terminada()) {
        imprimir(relatorio::resumoBatalha(batalha));
        if (batalha.estado() == EstadoBatalha::DERROTA) {
            heroi_->reviver();
            imprimir(heroi_->nome() +
                     " acorda na taverna com metade da vida e metade do ouro.");
        }
    }
}

void Jogo::descansar() {
    int custo = 5 * heroi_->nivel();
    heroi_->gastarOuro(custo);
    heroi_->restaurarCompletamente();
    std::ostringstream texto;
    texto << "Descansou por " << custo << " de ouro. Vida e mana restauradas.";
    imprimir(texto.str());
}

void Jogo::salvar() {
    saves_.salvar(*heroi_);
    imprimir("Heroi salvo em " + saves_.caminhoDoSave(heroi_->nome()));
}

void Jogo::novoHeroi() {
    std::string nome = lerTexto("Nome do heroi: ");
    if (encerrado_) {
        return;
    }
    if (nome.empty()) {
        throw EntradaInvalida("o nome nao pode ser vazio.");
    }

    std::vector<std::string> classes = classesDisponiveis();
    std::ostringstream menu;
    menu << "Classe:";
    for (std::size_t i = 0; i < classes.size(); i++) {
        menu << "\n" << i + 1 << ". " << classes[i];
    }
    imprimir(menu.str());

    int opcao = lerOpcao(1, static_cast<int>(classes.size()));
    if (opcao < 0) {
        return;
    }

    heroi_ = criarHeroi(classes[opcao - 1], nome);
    heroi_->ganharOuro(OURO_INICIAL);
    if (itens_.contem(POCAO_INICIAL)) {
        heroi_->inventario().adicionar(itens_.criar(POCAO_INICIAL));
    }
    imprimir("Bem-vindo, " + heroi_->resumo());
}

void Jogo::carregarHeroi() {
    std::vector<std::string> nomes = saves_.listar();
    if (nomes.empty()) {
        imprimir("Nenhum heroi salvo.");
        return;
    }

    std::ostringstream menu;
    menu << "Herois salvos:";
    for (std::size_t i = 0; i < nomes.size(); i++) {
        menu << "\n" << i + 1 << ". " << nomes[i];
    }
    imprimir(menu.str());

    int opcao = lerOpcao(1, static_cast<int>(nomes.size()));
    if (opcao < 0) {
        return;
    }
    heroi_ = saves_.carregar(nomes[opcao - 1]);
    imprimir("Bem-vindo de volta, " + heroi_->resumo());
}

// --------------------------------------------------------- Entrada/saida

std::string Jogo::lerLinha() {
    std::string linha;
    if (!std::getline(entrada_, linha)) {
        encerrado_ = true;
        return "";
    }
    return util::aparar(linha);
}

std::string Jogo::lerTexto(const std::string& pergunta) {
    saida_ << pergunta;
    return lerLinha();
}

int Jogo::lerOpcao(int minimo, int maximo) {
    while (!encerrado_) {
        std::string texto = lerTexto("> ");
        if (encerrado_) {
            break;
        }
        try {
            int valor = util::paraInteiro(texto);
            if (valor >= minimo && valor <= maximo) {
                return valor;
            }
        } catch (const EntradaInvalida&) {
        }
        std::ostringstream aviso;
        aviso << "digite um numero entre " << minimo << " e " << maximo << ".";
        imprimirErro(aviso.str());
    }
    return -1;
}

void Jogo::imprimir(const std::string& texto) { saida_ << texto << "\n"; }

void Jogo::imprimirErro(const std::string& texto) {
    saida_ << "[!] " << texto << "\n";
}

void Jogo::imprimirTitulo(const std::string& texto) {
    saida_ << util::repetir('=', 60) << "\n  " << texto << "\n"
           << util::repetir('=', 60) << "\n";
}

}  // namespace rpg
