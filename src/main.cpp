#include <iostream>
#include "Pokemon.hpp"

int main() {
    // Criar Pokémon
    Pokemon pikachu("Pikachu", "Elétrico", 35, 10, 3);
    Pokemon charmander("Charmander", "Fogo", 39, 8, 4);

    std::cout << "Escolha seu Pokémon:\n1. Pikachu\n2. Charmander\n";
    int escolha;
    std::cin >> escolha;

    Pokemon* jogador;
    Pokemon* oponente;

    if (escolha == 1) {
        jogador = &pikachu;
        oponente = &charmander;
    } else {
        jogador = &charmander;
        oponente = &pikachu;
    }

    std::cout << "Você escolheu " << jogador->getNome() << "!\n";
    std::cout << "Oponente: " << oponente->getNome() << "!\n\n";

    // Batalha
    while (jogador->estaVivo() && oponente->estaVivo()) {
        jogador->atacar(*oponente);
        if (!oponente->estaVivo()) break;

        oponente->atacar(*jogador);
    }

    // Resultado
    if (jogador->estaVivo()) {
        std::cout << "\nVocê venceu!\n";
    } else {
        std::cout << "\nVocê perdeu!\n";
    }

    return 0;
}