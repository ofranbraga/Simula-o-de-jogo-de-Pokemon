#include <iostream>
#include <vector>
#include <limits> // Para std::numeric_limits
#include <cstdlib> // Para std::rand e std::srand
#include <ctime>   // Para std::time
#include "Pokemon.hpp"

// Função para limpar o buffer de entrada
void limparBufferEntrada() {
    std::cin.clear(); // Limpa flags de erro
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta o restante da linha
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed para números aleatórios

    // Definir ataques
    Ataque investida("Investida", "Normal", 40, 35);
    Ataque rosnar("Rosnar", "Normal", 0, 40); // Exemplo de ataque que não causa dano, poderia reduzir ataque do oponente
    Ataque choqueDoTrovao("Choque do Trovão", "Eletrico", 40, 30);
    Ataque caudaDeFerro("Cauda de Ferro", "Aco", 100, 15); 
    Ataque raio("Raio", "Eletrico", 90, 15);
    Ataque arranhao("Arranhão", "Normal", 40, 35);
    Ataque brasa("Brasa", "Fogo", 40, 25);
    Ataque lancaChamas("Lança-Chamas", "Fogo", 90, 15);
    Ataque pistolaDAgua("Pistola D'Água", "Agua", 40, 25);
    Ataque jatoDAgua("Jato D'Água", "Agua", 90, 15); // Nome corrigido
    Ataque hidrobomba("Hidrobomba", "Agua", 110, 5);
    Ataque folhaNavalha("Folha Navalha", "Grama", 55, 25);
    Ataque raioSolar("Raio Solar", "Grama", 120, 10); 
    Ataque terremoto("Terremoto", "Terra", 100, 10);
    Ataque pedrada("Pedrada", "Rocha", 50, 15);

    // Criar Pokémon com seus ataques
    Pokemon pikachu("Pikachu", "Eletrico", 135, 55, 40, {choqueDoTrovao, investida, caudaDeFerro, raio});
    Pokemon charmander("Charmander", "Fogo", 139, 52, 43, {arranhao, brasa, lancaChamas});
    Pokemon squirtle("Squirtle", "Agua", 144, 48, 65, {investida, pistolaDAgua, jatoDAgua, hidrobomba});
    Pokemon bulbasaur("Bulbasaur", "Grama", 145, 49, 49, {investida, folhaNavalha, rosnar, raioSolar});
    Pokemon geodude("Geodude", "Rocha", 140, 80, 100, {investida, pedrada, terremoto});
    Pokemon onix("Onix", "Rocha", 135, 45, 160, {investida, pedrada, terremoto, rosnar});

    std::vector<Pokemon*> todosOsPokemons = {&pikachu, &charmander, &squirtle, &bulbasaur, &geodude, &onix};

    std::cout << "Bem-vindo à Batalha Pokémon!\n";
    std::cout << "Escolha seu Pokémon:\n";
    for (size_t i = 0; i < todosOsPokemons.size(); ++i) {
        std::cout << i + 1 << ". " << todosOsPokemons[i]->getNome() << " (Tipo: " << todosOsPokemons[i]->getTipoPokemon() << ")\n";
    }

    int escolha_jogador_idx;
    Pokemon* jogador = nullptr;
    Pokemon* oponente = nullptr;

    while (true) {
        std::cout << "Sua escolha (1-" << todosOsPokemons.size() << "): ";
        if (!(std::cin >> escolha_jogador_idx) || escolha_jogador_idx < 1 || escolha_jogador_idx > static_cast<int>(todosOsPokemons.size())) {
            std::cout << "Escolha inválida. Tente novamente.\n";
            limparBufferEntrada();
        } else {
            jogador = todosOsPokemons[escolha_jogador_idx - 1];
            break;
        }
    }
    limparBufferEntrada(); 

    int escolha_oponente_idx;
    do {
        escolha_oponente_idx = std::rand() % todosOsPokemons.size();
    } while (todosOsPokemons[escolha_oponente_idx] == jogador);
    oponente = todosOsPokemons[escolha_oponente_idx];

    std::cout << "\nVocê escolheu " << jogador->getNome() << "!\n";
    jogador->mostrarStatus();
    std::cout << "Seu oponente é " << oponente->getNome() << "!\n";
    oponente->mostrarStatus();
    std::cout << "\n--- BATALHA INICIADA ---\n";

    int turno = 1;
    // Batalha
    while (jogador->estaVivo() && oponente->estaVivo()) {
        std::cout << "\n--- Turno " << turno << " --- \n";
        std::cout << "Vez de " << jogador->getNome() << ":\n";
        jogador->mostrarStatus();
        oponente->mostrarStatus();
        std::cout << "\n";
        jogador->mostrarAtaques();

        int escolha_ataque_idx;
        bool ataque_valido_jogador = false;
        while(!ataque_valido_jogador) {
            std::cout << "Escolha seu ataque (1-" << jogador->getAtaques().size() << "): ";
            if (!(std::cin >> escolha_ataque_idx) || escolha_ataque_idx < 1 || escolha_ataque_idx > static_cast<int>(jogador->getAtaques().size())) {
                std::cout << "Escolha de ataque inválida. Tente novamente.\n";
                limparBufferEntrada();
            } else {
                 ataque_valido_jogador = jogador->usarAtaque(escolha_ataque_idx - 1, *oponente);
                 if (!ataque_valido_jogador && jogador->getAtaques()[escolha_ataque_idx - 1].pp_atual <=0) {
                    // Se o ataque não foi válido por falta de PP, não precisa limpar o buffer se a entrada foi um número
                 } else if (!ataque_valido_jogador) {
                    limparBufferEntrada(); // Limpa se foi inválido por outro motivo (ex: índice errado após erro)
                 }
            }
        }
        if (!ataque_valido_jogador && std::cin.fail()) limparBufferEntrada(); // Limpa se a última tentativa falhou na leitura

        if (!oponente->estaVivo()) break;

        std::cout << "\n---\n";
        std::cout << "Vez de " << oponente->getNome() << ":\n";
        const auto& ataques_oponente = oponente->getAtaques();
        std::vector<int> ataques_disponiveis_oponente;
        for(size_t i = 0; i < ataques_oponente.size(); ++i) {
            if (ataques_oponente[i].pp_atual > 0) {
                ataques_disponiveis_oponente.push_back(i);
            }
        }

        if (!ataques_disponiveis_oponente.empty()) {
            int indice_ataque_oponente = ataques_disponiveis_oponente[std::rand() % ataques_disponiveis_oponente.size()];
            oponente->usarAtaque(indice_ataque_oponente, *jogador);
        } else {
            std::cout << oponente->getNome() << " não tem mais ataques com PP e perdeu a vez!\n";
        }

        if (!jogador->estaVivo()) break;
        turno++;
    }

    std::cout << "\n--- FIM DA BATALHA ---\n";
    if (jogador->estaVivo()) {
        std::cout << jogador->getNome() << " venceu!\n";
    } else {
        std::cout << oponente->getNome() << " venceu! Você perdeu.\n";
    }

    return 0;
}