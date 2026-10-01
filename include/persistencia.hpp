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
    /** @brief Cria o repositorio de saves no diretorio informado. */
    explicit RepositorioSaves(const std::string& diretorio);

    /** @brief Diretorio onde os saves sao gravados e lidos. */
    const std::string& diretorio() const;

    /**
     * @brief Grava o heroi em um arquivo de save.
     * @throws ArquivoInvalido se nao conseguir escrever.
     */
    void salvar(const Heroi& heroi);

    /**
     * @brief Carrega o heroi salvo com esse nome.
     * @throws ArquivoInvalido se o arquivo nao existir ou estiver corrompido.
     */
    std::unique_ptr<Heroi> carregar(const std::string& nome) const;

    /** @brief Indica se existe um save com esse nome. */
    bool existe(const std::string& nome) const;
    /** @brief Lista os nomes de todos os saves existentes. */
    std::vector<std::string> listar() const;

    /**
     * @brief Remove o save com esse nome.
     * @throws ArquivoInvalido se o save nao existir.
     */
    void remover(const std::string& nome);

    /** @brief Caminho do arquivo de save para o nome informado. */
    std::string caminhoDoSave(const std::string& nome) const;

    /** @brief Escreve os dados do heroi no formato "chave;valor". */
    static void escrever(const Heroi& heroi, std::ostream& saida);
    /** @brief Le os dados de um heroi no formato "chave;valor". */
    static std::unique_ptr<Heroi> ler(std::istream& entrada,
                                       const std::string& origem);

    /** @brief Minusculas, sem espacos nem simbolos, para virar nome de arquivo. */
    static std::string normalizar(const std::string& nome);

private:
    std::string caminhoDoIndice() const;
    void gravarIndice(const std::vector<std::string>& nomes) const;

    std::string diretorio_;
};

} // namespace rpg

#endif // RPG_PERSISTENCIA_HPP
