#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream> // Para std::cin, std::cout
#include <limits>   // Para std::numeric_limits
#include <cstdlib>  // Para std::system

// Função para limpar o terminal
inline void limparTerminal() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    // Assume POSIX (Linux, macOS)
    std::system("clear");
#endif
}

// Função para limpar o buffer de entrada
inline void limparBufferEntrada() {
    std::cin.clear(); // Limpa flags de erro do cin
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta caracteres até a próxima nova linha
}

#endif // UTILS_HPP