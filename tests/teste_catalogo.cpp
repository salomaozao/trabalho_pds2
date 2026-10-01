#include <sstream>

#include "catalogo.hpp"
#include "dado.hpp"
#include "doctest.h"
#include "excecoes.hpp"

using namespace rpg;

namespace {

const char* ITENS =
    "# comentario\n"
    "\n"
    "ARMA;Espada Curta;60;4\n"
    "   ARMADURA;Cota de Malha;150;5   \n"
    "POCAO;Pocao de Vida;25;VIDA;30\n";

const char* MONSTROS =
    "# Nome;Nivel;Vida;Mana;For;Int;Des;Def;XP;Ouro\n"
    "Rato;1;18;0;4;1;3;0;15;5\n"
    "Goblin;1;25;0;5;2;4;1;20;10\n"
    "Orc;3;55;0;11;2;4;3;60;25\n"
    "Dragao;8;180;40;22;12;8;10;300;200\n";

}  // namespace

TEST_CASE("CatalogoItens ignora comentarios e linhas vazias") {
    std::istringstream entrada(ITENS);
    CatalogoItens catalogo;
    catalogo.carregar(entrada, "teste");

    CHECK(catalogo.tamanho() == 3);
    CHECK(catalogo.contem("espada curta"));
    CHECK(catalogo.porTipo(TipoItem::ARMA).size() == 1);
    CHECK(catalogo.porTipo(TipoItem::POCAO).size() == 1);
}

TEST_CASE("CatalogoItens.criar devolve copias novas") {
    std::istringstream entrada(ITENS);
    CatalogoItens catalogo;
    catalogo.carregar(entrada, "teste");

    std::shared_ptr<Item> a = catalogo.criar("Espada Curta");
    std::shared_ptr<Item> b = catalogo.criar("espada curta");
    CHECK(a->nome() == "Espada Curta");
    CHECK(a.get() != b.get());
    CHECK_THROWS_AS(catalogo.criar("Lanca"), ItemNaoEncontrado);
}

TEST_CASE("CatalogoItens aponta a linha invalida") {
    std::istringstream entrada("ARMA;Espada;60;4\n\nARMA;Quebrada;x;1\n");
    CatalogoItens catalogo;
    try {
        catalogo.carregar(entrada, "itens.txt");
        FAIL("deveria ter lancado");
    } catch (const ArquivoInvalido& erro) {
        CHECK(erro.caminho() == "itens.txt");
        CHECK(std::string(erro.what()).find("linha 3") != std::string::npos);
    }
}

TEST_CASE("CatalogoItens recusa duplicados e nulos") {
    CatalogoItens catalogo;
    catalogo.adicionar(std::make_shared<Arma>("Espada", 10, 1));
    CHECK_THROWS_AS(catalogo.adicionar(std::make_shared<Arma>("espada", 5, 2)),
                    EntradaInvalida);
    CHECK_THROWS_AS(catalogo.adicionar(std::shared_ptr<Item>()), EntradaInvalida);
}

TEST_CASE("carregarArquivo falha com caminho inexistente") {
    CatalogoItens itens;
    CatalogoMonstros monstros;
    CHECK_THROWS_AS(itens.carregarArquivo("nao/existe.txt"), ArquivoInvalido);
    CHECK_THROWS_AS(monstros.carregarArquivo("nao/existe.txt"), ArquivoInvalido);
}

TEST_CASE("os arquivos reais de data/ carregam sem erro") {
    CatalogoItens itens;
    CatalogoMonstros monstros;
    CHECK_NOTHROW(itens.carregarArquivo("data/itens.txt"));
    CHECK_NOTHROW(monstros.carregarArquivo("data/monstros.txt"));
    CHECK(itens.tamanho() > 0);
    CHECK(monstros.tamanho() > 0);
}

TEST_CASE("CatalogoMonstros carrega, filtra por nivel e cria copias cheias") {
    std::istringstream entrada(MONSTROS);
    CatalogoMonstros catalogo;
    catalogo.carregar(entrada, "teste");

    CHECK(catalogo.tamanho() == 4);
    CHECK(catalogo.ateNivel(1).size() == 2);
    CHECK(catalogo.ateNivel(3).size() == 3);
    CHECK(catalogo.ateNivel(0).empty());

    Monstro orc = catalogo.criar("orc");
    CHECK(orc.vida() == 55);
    CHECK_THROWS_AS(catalogo.criar("Hidra"), ItemNaoEncontrado);
}

TEST_CASE("CatalogoMonstros aponta a linha invalida e recusa duplicado") {
    std::istringstream entrada("Rato;1;18;0;4;1;3;0;15;5\nRato;1;18;0;4;1;3;0;15;5\n");
    CatalogoMonstros catalogo;
    CHECK_THROWS_AS(catalogo.carregar(entrada, "m.txt"), ArquivoInvalido);

    std::istringstream curta("Rato;1;18\n");
    CatalogoMonstros outro;
    CHECK_THROWS_AS(outro.carregar(curta, "m.txt"), ArquivoInvalido);
}

TEST_CASE("sortear escolhe entre os monstros do nivel usando o dado") {
    std::istringstream entrada(MONSTROS);
    CatalogoMonstros catalogo;
    catalogo.carregar(entrada, "teste");

    DadoFixo primeiro(1);
    CHECK(catalogo.sortear(1, primeiro).nome() == "Rato");

    DadoFixo segundo(2);
    CHECK(catalogo.sortear(1, segundo).nome() == "Goblin");

    DadoFixo terceiro(3);
    CHECK(catalogo.sortear(5, terceiro).nome() == "Orc");

    SUBCASE("sem monstro no nivel, usa os mais fracos") {
        CatalogoMonstros fortes;
        fortes.adicionar(criarMonstroDeLinha("Dragao;8;180;40;22;12;8;10;300;200"));
        fortes.adicionar(criarMonstroDeLinha("Orc;3;55;0;11;2;4;3;60;25"));
        CHECK(fortes.sortear(1, primeiro).nome() == "Orc");
    }

    SUBCASE("catalogo vazio nao sorteia") {
        CatalogoMonstros vazio;
        CHECK_THROWS_AS(vazio.sortear(1, primeiro), AcaoInvalida);
    }
}
