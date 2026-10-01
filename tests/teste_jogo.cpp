#include <sstream>

#include "catalogo.hpp"
#include "dado.hpp"
#include "doctest.h"
#include "heroi.hpp"
#include "jogo.hpp"
#include "persistencia.hpp"

using namespace rpg;

namespace {

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}

int ocorrencias(const std::string& texto, const std::string& trecho) {
    int total = 0;
    std::size_t pos = texto.find(trecho);
    while (pos != std::string::npos) {
        total++;
        pos = texto.find(trecho, pos + trecho.size());
    }
    return total;
}

CatalogoItens itensDeTeste() {
    CatalogoItens itens;
    itens.adicionar(std::make_shared<Arma>("Adaga", 20, 2));
    itens.adicionar(std::make_shared<Armadura>("Tunica", 25, 1));
    itens.adicionar(
        std::make_shared<Pocao>("Pocao de Vida Pequena", 25, EfeitoPocao::VIDA, 30));
    return itens;
}

CatalogoMonstros ratos() {
    CatalogoMonstros monstros;
    monstros.adicionar(Monstro("Rato", 1, 1, 0, Atributos(1, 1, 1, 0), 15, 5));
    return monstros;
}

// Roda o jogo com a entrada dada e devolve tudo que foi impresso.
std::string jogar(const std::string& entrada, const CatalogoMonstros& monstros,
                  const Dado& dado, const std::string& diretorio = "build") {
    std::istringstream in(entrada);
    std::ostringstream out;
    CatalogoItens itens = itensDeTeste();
    RepositorioSaves saves(diretorio);
    Jogo jogo(in, out, dado, itens, monstros, saves);
    jogo.executar();
    return out.str();
}

}  // namespace

TEST_CASE("sair no menu inicial encerra o jogo") {
    DadoFixo dado(1);
    std::string saida = jogar("3\n", ratos(), dado);
    CHECK(contem(saida, "RPG POR TURNOS"));
    CHECK(contem(saida, "Ate a proxima!"));
}

TEST_CASE("entrada esgotada encerra sem travar") {
    DadoFixo dado(1);
    CHECK(contem(jogar("", ratos(), dado), "Ate a proxima!"));
    CHECK(contem(jogar("1\nConan\n", ratos(), dado), "Ate a proxima!"));
    CHECK(contem(jogar("1\nConan\n1\n4\n", ratos(), dado), "Ate a proxima!"));
}

TEST_CASE("opcao fora do menu pede de novo") {
    DadoFixo dado(1);
    std::string saida = jogar("abc\n9\n3\n", ratos(), dado);
    CHECK(ocorrencias(saida, "[!] digite um numero entre 1 e 3.") == 2);
    CHECK(contem(saida, "Ate a proxima!"));
}

TEST_CASE("novo heroi recebe ouro, pocao inicial e aparece na ficha") {
    DadoFixo dado(1);
    std::string saida = jogar("1\nConan\n1\n1\n7\n3\n", ratos(), dado);
    CHECK(contem(saida, "Bem-vindo, Conan (Guerreiro, Nv 1)"));
    CHECK(contem(saida, "Conan - Guerreiro nivel 1"));
    CHECK(contem(saida, "Ouro: 50"));
    CHECK(contem(saida, "Batalhas: 0"));
}

TEST_CASE("nome vazio e recusado e o jogador tenta de novo") {
    DadoFixo dado(1);
    std::string saida = jogar("1\n   \n1\nMerlin\n2\n7\n3\n", ratos(), dado);
    CHECK(contem(saida, "[!] Entrada invalida: o nome nao pode ser vazio."));
    CHECK(contem(saida, "Bem-vindo, Merlin (Mago, Nv 1)"));
}

TEST_CASE("loja compra, vende e avisa erros") {
    DadoFixo dado(1);
    std::string saida = jogar(
        "1\nConan\n1\n"
        "3\n1\nadaga\n2\nAdaga\n1\nMachado\n1\nTunica\n1\nTunica\n1\nTunica\n3\n"
        "7\n3\n",
        ratos(), dado);
    CHECK(contem(saida, "Comprou Adaga por 20 de ouro."));
    CHECK(contem(saida, "Vendeu Adaga por 10 de ouro."));
    CHECK(contem(saida, "[!] Item nao encontrado: Machado"));
    CHECK(contem(saida, "Comprou Tunica por 25 de ouro."));
    CHECK(contem(saida, "[!] Falta ouro: sao necessarios 25 e existem apenas 15."));
    CHECK(contem(saida, "Ouro: 15"));
}

TEST_CASE("inventario equipa, desequipa e usa pocao") {
    DadoFixo dado(1);
    std::string saida = jogar(
        "1\nConan\n1\n"
        "3\n1\nAdaga\n3\n"
        "2\n1\nPocao de Vida Pequena\n1\nAdaga\n2\n3\n2\n4\nPocao de Vida Pequena\n"
        "4\nNada\n5\n"
        "7\n3\n",
        ratos(), dado);
    CHECK(contem(saida, "[!] Acao invalida: pocoes nao podem ser equipadas"));
    CHECK(contem(saida, "Adaga equipado."));
    CHECK(contem(saida, "Arma: Adaga | Armadura: nenhuma"));
    CHECK(contem(saida, "Arma guardada no inventario."));
    CHECK(contem(saida, "[!] Acao invalida: nao ha armadura equipada."));
    CHECK(contem(saida, "[!] Acao invalida: nao ha arma equipada."));
    CHECK(contem(saida, "Recuperou 0."));
    CHECK(contem(saida, "[!] Item nao encontrado: Nada"));
}

TEST_CASE("batalha vencida da experiencia e mostra o resumo") {
    DadoFixo dado(6);
    std::string saida = jogar("1\nConan\n1\n4\n1\n1\n7\n3\n", ratos(), dado);
    CHECK(contem(saida, "Conan encontra Rato!"));
    CHECK(contem(saida, "1. Atacar"));
    CHECK(contem(saida, "2. Golpe Devastador (8 de mana)"));
    CHECK(contem(saida, "Resultado: Vitoria em 1 rodada(s)."));
    CHECK(contem(saida, "Experiencia: 15/50"));
    CHECK(contem(saida, "Ouro: 55"));
}

TEST_CASE("batalha aceita habilidade, pocao e fuga pelo menu") {
    CatalogoMonstros monstros;
    monstros.adicionar(Monstro("Lesma", 1, 200, 0, Atributos(1, 1, 1, 0), 1, 1));
    DadoFixo dado(6);
    std::string saida = jogar(
        "1\nConan\n1\n4\n2\n3\nPocao de Vida Pequena\n3\nNada\n2\n2\n4\n7\n3\n",
        monstros, dado);
    CHECK(contem(saida, "Conan usa Golpe Devastador em Lesma"));
    CHECK(contem(saida, "Conan usa Pocao de Vida Pequena e recupera"));
    CHECK(contem(saida, "[!] Item nao encontrado: Nada"));
    CHECK(contem(saida, "[!] Falta mana"));
    CHECK(contem(saida, "Conan escapa da batalha."));
    CHECK(contem(saida, "Resultado: Fuga"));
}

TEST_CASE("derrota revive o heroi na taverna") {
    CatalogoMonstros monstros;
    monstros.adicionar(
        Monstro("Troll", 5, 90, 0, Atributos(15, 1, 2, 5), 110, 50));
    DadoFixo dado(1);
    std::string saida = jogar("1\nMerlin\n2\n4\n1\n1\n1\n1\n7\n3\n", monstros, dado);
    CHECK(contem(saida, "Merlin cai diante de Troll."));
    CHECK(contem(saida, "Resultado: Derrota"));
    CHECK(contem(saida, "Merlin acorda na taverna com metade da vida e metade do ouro."));
    CHECK(contem(saida, "Merlin (Mago, Nv 1) Vida 17/35"));
}

TEST_CASE("descansar cobra ouro e restaura os recursos") {
    DadoFixo dado(1);
    std::string saida = jogar("1\nConan\n1\n5\n7\n3\n", ratos(), dado);
    CHECK(contem(saida, "Descansou por 5 de ouro. Vida e mana restauradas."));
}

TEST_CASE("salvar e carregar pelo menu") {
    DadoFixo dado(1);
    RepositorioSaves repo("build");
    if (repo.existe("Conan")) {
        repo.remover("Conan");
    }

    std::string saida = jogar("1\nConan\n1\n6\n7\n2\n1\n7\n3\n", ratos(), dado);
    CHECK(contem(saida, "Heroi salvo em build/conan.txt"));
    CHECK(contem(saida, "Herois salvos:\n1. Conan"));
    CHECK(contem(saida, "Bem-vindo de volta, Conan (Guerreiro, Nv 1)"));

    repo.remover("Conan");
}

TEST_CASE("carregar sem saves avisa o jogador") {
    DadoFixo dado(1);
    std::string saida = jogar("2\n3\n", ratos(), dado, "build/inexistente");
    CHECK(contem(saida, "Nenhum heroi salvo."));
}
