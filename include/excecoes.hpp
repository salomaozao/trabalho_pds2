#ifndef RPG_EXCECOES_HPP
#define RPG_EXCECOES_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

/**
 * @file excecoes.hpp
 * @brief Hierarquia de excecoes usada em todo o sistema.
 *
 * Todas as excecoes do jogo derivam de ErroDeJogo, que por sua vez deriva de
 * std::runtime_error. Isso permite que o programa principal capture qualquer
 * falha prevista com um unico `catch (const rpg::ErroDeJogo&)`, sem deixar de
 * ser compativel com um `catch (const std::exception&)` mais generico.
 */

namespace rpg {

/**
 * @brief Classe base de todas as excecoes lancadas pelo jogo.
 */
class ErroDeJogo : public std::runtime_error {
public:
    /**
     * @brief Constroi a excecao com uma mensagem descritiva.
     * @param mensagem Texto exibido ao usuario ou registrado no log.
     */
    explicit ErroDeJogo(const std::string& mensagem);
};

/**
 * @brief Lancada quando um arquivo de dados nao pode ser aberto ou esta mal
 *        formatado.
 */
class ArquivoInvalido : public ErroDeJogo {
public:
    /**
     * @brief Constroi a excecao informando o arquivo problematico.
     * @param caminho Caminho do arquivo que causou a falha.
     * @param motivo  Explicacao do problema encontrado.
     */
    ArquivoInvalido(const std::string& caminho, const std::string& motivo);

    /** @brief Retorna o caminho do arquivo que causou a falha. */
    const std::string& caminho() const;

private:
    std::string caminho_;
};

/**
 * @brief Lancada quando um valor fornecido pelo usuario ou por um arquivo esta
 *        fora do dominio esperado.
 */
class EntradaInvalida : public ErroDeJogo {
public:
    explicit EntradaInvalida(const std::string& mensagem);
};

/**
 * @brief Lancada quando um item procurado nao existe no inventario ou no
 *        catalogo.
 */
class ItemNaoEncontrado : public ErroDeJogo {
public:
    /**
     * @brief Constroi a excecao informando o nome procurado.
     * @param nome Nome do item que nao foi localizado.
     */
    explicit ItemNaoEncontrado(const std::string& nome);

    /** @brief Retorna o nome do item que nao foi localizado. */
    const std::string& nomeProcurado() const;

private:
    std::string nome_;
};

/**
 * @brief Lancada ao tentar guardar um item em um inventario sem espaco livre.
 */
class InventarioCheio : public ErroDeJogo {
public:
    /**
     * @brief Constroi a excecao informando a capacidade esgotada.
     * @param capacidade Numero maximo de itens suportado pelo inventario.
     */
    explicit InventarioCheio(std::size_t capacidade);
};

/**
 * @brief Lancada quando falta mana, ouro ou qualquer outro recurso consumivel.
 */
class RecursoInsuficiente : public ErroDeJogo {
public:
    /**
     * @brief Constroi a excecao detalhando o recurso em falta.
     * @param recurso    Nome do recurso (por exemplo "mana" ou "ouro").
     * @param disponivel Quantidade que o personagem possui.
     * @param necessario Quantidade exigida pela acao.
     */
    RecursoInsuficiente(const std::string& recurso, int disponivel,
                        int necessario);
};

/**
 * @brief Lancada ao tentar fazer um personagem morto agir ou ser alvo de uma
 *        acao de combate.
 */
class PersonagemMorto : public ErroDeJogo {
public:
    /**
     * @brief Constroi a excecao informando o personagem envolvido.
     * @param nome Nome do personagem que ja esta morto.
     */
    explicit PersonagemMorto(const std::string& nome);
};

/**
 * @brief Lancada quando uma acao nao faz sentido no estado atual do jogo.
 *
 * Por exemplo, executar uma rodada de uma batalha que ja terminou.
 */
class AcaoInvalida : public ErroDeJogo {
public:
    explicit AcaoInvalida(const std::string& mensagem);
};

}  // namespace rpg

#endif  // RPG_EXCECOES_HPP
