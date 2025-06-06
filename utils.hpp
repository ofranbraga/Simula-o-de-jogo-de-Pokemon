#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <limits>
#include <cstdlib> // Para system()

// Função para limpar o buffer de entrada
inline void limparBufferEntrada() {
    std::cin.clear(); // Limpa flags de erro
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta o restante da linha
}

// Função para limpar o terminal
inline void limparTerminal() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

#endif // UTILS_HPP
