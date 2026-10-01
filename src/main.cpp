/**
 * @file main.cpp
 * @brief Carrega os dados, monta o Jogo e protege contra excecoes nao tratadas.
 */

#include <exception>
#include <iostream>

#include "catalogo.hpp"
#include "dado.hpp"
#include "excecoes.hpp"
#include "jogo.hpp"
#include "persistencia.hpp"

int main() {
    try {
        rpg::CatalogoItens itens;
        itens.carregarArquivo("data/itens.txt");

        rpg::CatalogoMonstros monstros;
        monstros.carregarArquivo("data/monstros.txt");

        rpg::RepositorioSaves saves("data/saves");
        rpg::DadoAleatorio dado;

        rpg::Jogo jogo(std::cin, std::cout, dado, itens, monstros, saves);
        jogo.executar();
        return 0;
    } catch (const rpg::ErroDeJogo& erro) {
        std::cerr << "\n[ERRO] " << erro.what() << "\n";
        return 1;
    } catch (const std::exception& erro) {
        std::cerr << "\n[ERRO INESPERADO] " << erro.what() << "\n";
        return 2;
    }
}
