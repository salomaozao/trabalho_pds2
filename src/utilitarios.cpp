#include "utilitarios.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>

#include "excecoes.hpp"

namespace rpg {
namespace util {

std::string aparar(const std::string& texto) {
    std::size_t inicio = 0;
    while (inicio < texto.size() &&
           std::isspace(static_cast<unsigned char>(texto[inicio]))) {
        inicio++;
    }

    // Se percorremos o texto inteiro, ele so tinha espacos.
    if (inicio == texto.size()) {
        return "";
    }

    std::size_t fim = texto.size() - 1;
    while (fim > inicio && std::isspace(static_cast<unsigned char>(texto[fim]))) {
        fim--;
    }

    return texto.substr(inicio, fim - inicio + 1);
}

std::vector<std::string> dividir(const std::string& texto, char separador) {
    std::vector<std::string> pedacos;
    std::string atual;

    for (std::size_t i = 0; i < texto.size(); i++) {
        if (texto[i] == separador) {
            pedacos.push_back(aparar(atual));
            atual.clear();
        } else {
            atual += texto[i];
        }
    }

    // O ultimo pedaco nao e seguido por separador, entao entra fora do laco.
    pedacos.push_back(aparar(atual));
    return pedacos;
}

int paraInteiro(const std::string& texto) {
    std::string limpo = aparar(texto);

    if (limpo.empty()) {
        throw EntradaInvalida("era esperado um numero, mas o texto esta vazio.");
    }

    std::size_t inicio = 0;
    if (limpo[0] == '+' || limpo[0] == '-') {
        inicio = 1;
        if (limpo.size() == 1) {
            throw EntradaInvalida("'" + limpo + "' nao e um numero inteiro.");
        }
    }

    for (std::size_t i = inicio; i < limpo.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(limpo[i]))) {
            throw EntradaInvalida("'" + limpo + "' nao e um numero inteiro.");
        }
    }

    // A conversao so acontece depois de o formato ter sido validado acima.
    std::istringstream fluxo(limpo);
    long valor = 0;
    fluxo >> valor;

    if (fluxo.fail()) {
        throw EntradaInvalida("'" + limpo + "' nao pode ser convertido.");
    }

    return static_cast<int>(valor);
}

std::string paraMaiusculas(const std::string& texto) {
    std::string resultado = texto;
    for (std::size_t i = 0; i < resultado.size(); i++) {
        resultado[i] = static_cast<char>(
            std::toupper(static_cast<unsigned char>(resultado[i])));
    }
    return resultado;
}

std::string paraMinusculas(const std::string& texto) {
    std::string resultado = texto;
    for (std::size_t i = 0; i < resultado.size(); i++) {
        resultado[i] = static_cast<char>(
            std::tolower(static_cast<unsigned char>(resultado[i])));
    }
    return resultado;
}

bool iguaisSemCaixa(const std::string& a, const std::string& b) {
    return paraMinusculas(aparar(a)) == paraMinusculas(aparar(b));
}

bool linhaIgnoravel(const std::string& linha) {
    std::string limpa = aparar(linha);
    return limpa.empty() || limpa[0] == '#';
}

int limitar(int valor, int minimo, int maximo) {
    if (minimo > maximo) {
        throw EntradaInvalida(
            "o limite minimo nao pode ser maior que o maximo.");
    }
    if (valor < minimo) {
        return minimo;
    }
    if (valor > maximo) {
        return maximo;
    }
    return valor;
}

std::string barraDeProgresso(int atual, int maximo, int largura) {
    if (maximo <= 0) {
        throw EntradaInvalida("o valor maximo da barra deve ser positivo.");
    }
    if (largura <= 0) {
        throw EntradaInvalida("a largura da barra deve ser positiva.");
    }

    int ajustado = limitar(atual, 0, maximo);

    // Regra de tres para descobrir quantos blocos devem ser preenchidos.
    int preenchidos = (ajustado * largura) / maximo;

    std::string barra = "[";
    for (int i = 0; i < largura; i++) {
        barra += (i < preenchidos) ? '#' : '-';
    }
    barra += "]";
    return barra;
}

std::string alinharEsquerda(const std::string& texto, std::size_t largura) {
    if (texto.size() >= largura) {
        return texto;
    }
    return texto + std::string(largura - texto.size(), ' ');
}

std::string repetir(char caractere, std::size_t quantidade) {
    return std::string(quantidade, caractere);
}

}  // namespace util
}  // namespace rpg
