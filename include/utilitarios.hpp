#ifndef RPG_UTILITARIOS_HPP
#define RPG_UTILITARIOS_HPP

#include <string>
#include <vector>

/**
 * @file utilitarios.hpp
 * @brief Funcoes auxiliares de texto e formatacao usadas por varios modulos.
 *
 * Estas funcoes nao pertencem a nenhuma classe: sao operacoes puras sobre
 * strings e numeros, reunidas no namespace rpg::util para evitar repeticao
 * entre os repositorios, o save e a interface de terminal.
 */

namespace rpg {
namespace util {

/**
 * @brief Remove espacos, tabulacoes e quebras de linha do inicio e do fim.
 * @param texto Texto original.
 * @return Copia do texto sem os espacos das pontas.
 */
std::string aparar(const std::string& texto);

/**
 * @brief Quebra um texto em pedacos usando um caractere separador.
 *
 * Campos vazios sao preservados, de modo que "a;;b" produz tres pedacos.
 *
 * @param texto     Texto a ser dividido.
 * @param separador Caractere que delimita os campos.
 * @return Vetor com os pedacos, ja aparados.
 */
std::vector<std::string> dividir(const std::string& texto, char separador);

/**
 * @brief Converte um texto para inteiro validando o conteudo.
 * @param texto Texto a converter, com ou sem espacos nas pontas.
 * @return O valor inteiro correspondente.
 * @throws EntradaInvalida se o texto estiver vazio ou contiver caracteres que
 *         nao formam um numero inteiro valido.
 */
int paraInteiro(const std::string& texto);

/**
 * @brief Converte todas as letras do texto para maiusculas.
 * @param texto Texto original.
 * @return Copia em maiusculas.
 */
std::string paraMaiusculas(const std::string& texto);

/**
 * @brief Converte todas as letras do texto para minusculas.
 * @param texto Texto original.
 * @return Copia em minusculas.
 */
std::string paraMinusculas(const std::string& texto);

/**
 * @brief Compara dois textos ignorando maiusculas e minusculas.
 * @param a Primeiro texto.
 * @param b Segundo texto.
 * @return true se os textos forem iguais desconsiderando a caixa.
 */
bool iguaisSemCaixa(const std::string& a, const std::string& b);

/**
 * @brief Indica se uma linha de arquivo de dados deve ser ignorada.
 *
 * Sao ignoradas as linhas vazias, as compostas so por espacos e as que
 * comecam com '#', usadas como comentario nos arquivos de data/.
 *
 * @param linha Linha lida do arquivo.
 * @return true se a linha nao contiver dados.
 */
bool linhaIgnoravel(const std::string& linha);

/**
 * @brief Limita um valor a um intervalo fechado.
 * @param valor  Valor a ser ajustado.
 * @param minimo Limite inferior.
 * @param maximo Limite superior.
 * @return O proprio valor, ou o limite mais proximo caso esteja fora.
 * @throws EntradaInvalida se minimo for maior que maximo.
 */
int limitar(int valor, int minimo, int maximo);

/**
 * @brief Monta uma barra de progresso em texto, no estilo [####------].
 * @param atual   Valor atual (por exemplo, a vida do personagem).
 * @param maximo  Valor maximo (por exemplo, a vida maxima).
 * @param largura Quantidade de caracteres dentro dos colchetes.
 * @return Texto da barra pronto para ser impresso.
 * @throws EntradaInvalida se maximo ou largura nao forem positivos.
 */
std::string barraDeProgresso(int atual, int maximo, int largura);

/**
 * @brief Preenche um texto com espacos a direita ate atingir a largura dada.
 *
 * Usada para alinhar as colunas dos relatorios. Textos maiores que a largura
 * sao devolvidos sem alteracao.
 *
 * @param texto   Texto a alinhar.
 * @param largura Largura desejada.
 * @return Texto alinhado a esquerda.
 */
std::string alinharEsquerda(const std::string& texto, std::size_t largura);

/**
 * @brief Repete um caractere para formar uma linha separadora.
 * @param caractere Caractere a repetir.
 * @param quantidade Numero de repeticoes.
 * @return Texto com a linha montada.
 */
std::string repetir(char caractere, std::size_t quantidade);

}  // namespace util
}  // namespace rpg

#endif  // RPG_UTILITARIOS_HPP
