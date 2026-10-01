#ifndef RPG_PERSISTENCIA_HPP
#define RPG_PERSISTENCIA_HPP

#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

/**
 * @file persistencia.hpp
 * @brief Grava e recupera herois em arquivos de texto.
 *
 * Cada heroi vira um arquivo diretorio/nome_normalizado.txt com linhas
 * "chave;valor". Um arquivo indice.txt no mesmo diretorio lista os nomes
 * salvos, ja que C++11 nao tem como listar um diretorio de forma portavel.
 */

namespace rpg {

class Heroi;

class RepositorioSaves {
public:
    explicit RepositorioSaves(const std::string& diretorio);

    const std::string& diretorio() const;

    /** @throws ArquivoInvalido se nao conseguir escrever. */
    void salvar(const Heroi& heroi);

    /** @throws ArquivoInvalido se o arquivo nao existir ou estiver corrompido. */
    std::unique_ptr<Heroi> carregar(const std::string& nome) const;

    bool existe(const std::string& nome) const;
    std::vector<std::string> listar() const;

    /** @throws ArquivoInvalido se o save nao existir. */
    void remover(const std::string& nome);

    std::string caminhoDoSave(const std::string& nome) const;

    static void escrever(const Heroi& heroi, std::ostream& saida);
    static std::unique_ptr<Heroi> ler(std::istream& entrada,
                                      const std::string& origem);

    /** @brief Minusculas, sem espacos nem simbolos, para virar nome de arquivo. */
    static std::string normalizar(const std::string& nome);

private:
    std::string caminhoDoIndice() const;
    void gravarIndice(const std::vector<std::string>& nomes) const;

    std::string diretorio_;
};

}  // namespace rpg

#endif  // RPG_PERSISTENCIA_HPP
