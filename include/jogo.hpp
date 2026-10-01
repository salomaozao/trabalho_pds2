#ifndef RPG_JOGO_HPP
#define RPG_JOGO_HPP

#include <istream>
#include <memory>
#include <ostream>
#include <string>

/**
 * @file jogo.hpp
 * @brief Menus do terminal e ciclo principal.
 *
 * Toda entrada e saida passa pelos fluxos recebidos no construtor, entao os
 * testes conseguem dirigir o jogo com um std::istringstream.
 */

namespace rpg {

class CatalogoItens;
class CatalogoMonstros;
class Dado;
class Heroi;
class RepositorioSaves;

class Jogo {
public:
    Jogo(std::istream& entrada, std::ostream& saida, const Dado& dado,
         const CatalogoItens& itens, const CatalogoMonstros& monstros,
         RepositorioSaves& saves);
    ~Jogo();

    /** @brief Roda ate o jogador sair ou a entrada acabar. */
    void executar();

    const Heroi* heroi() const;

private:
    bool menuInicial();
    void menuPrincipal();
    void menuInventario();
    void menuLoja();
    void batalhar();
    void descansar();
    void salvar();
    void novoHeroi();
    void carregarHeroi();

    std::string lerLinha();
    std::string lerTexto(const std::string& pergunta);
    int lerOpcao(int minimo, int maximo);
    void imprimir(const std::string& texto);
    void imprimirErro(const std::string& texto);
    void imprimirTitulo(const std::string& texto);

    std::istream& entrada_;
    std::ostream& saida_;
    const Dado& dado_;
    const CatalogoItens& itens_;
    const CatalogoMonstros& monstros_;
    RepositorioSaves& saves_;
    std::unique_ptr<Heroi> heroi_;
    bool encerrado_;
};

}  // namespace rpg

#endif  // RPG_JOGO_HPP
