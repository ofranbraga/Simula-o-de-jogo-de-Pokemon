#include "Pokemon.hpp"
#include <iostream>
#include <algorithm> // Para std::max
#include <iomanip>   // Para std::setw

// Função auxiliar para calcular multiplicador de tipo (simplificado)
double calcularMultiplicadorTipo(const std::string& tipo_ataque, const std::string& tipo_alvo) {
    if (tipo_ataque == "Fogo") {
        if (tipo_alvo == "Grama") return 2.0;
        if (tipo_alvo == "Agua" || tipo_alvo == "Fogo" || tipo_alvo == "Rocha") return 0.5;
    } else if (tipo_ataque == "Agua") {
        if (tipo_alvo == "Fogo" || tipo_alvo == "Rocha" || tipo_alvo == "Terra") return 2.0;
        if (tipo_alvo == "Grama" || tipo_alvo == "Agua" || tipo_alvo == "Dragao") return 0.5;
    } else if (tipo_ataque == "Grama") {
        if (tipo_alvo == "Agua" || tipo_alvo == "Rocha" || tipo_alvo == "Terra") return 2.0;
        if (tipo_alvo == "Fogo" || tipo_alvo == "Grama" || tipo_alvo == "Veneno" || tipo_alvo == "Voador" || tipo_alvo == "Inseto" || tipo_alvo == "Dragao" || tipo_alvo == "Aco") return 0.5;
    } else if (tipo_ataque == "Eletrico") {
        if (tipo_alvo == "Agua" || tipo_alvo == "Voador") return 2.0;
        if (tipo_alvo == "Grama" || tipo_alvo == "Eletrico" || tipo_alvo == "Dragao") return 0.5;
        if (tipo_alvo == "Terra") return 0.0; // Imune
    } else if (tipo_ataque == "Normal") {
        if (tipo_alvo == "Rocha" || tipo_alvo == "Aco") return 0.5;
        if (tipo_alvo == "Fantasma") return 0.0; // Imune
    } else if (tipo_ataque == "Aco") {
        if (tipo_alvo == "Gelo" || tipo_alvo == "Rocha" || tipo_alvo == "Fada") return 2.0;
        if (tipo_alvo == "Fogo" || tipo_alvo == "Agua" || tipo_alvo == "Eletrico" || tipo_alvo == "Aco") return 0.5;
    }
    // Adicionar mais interações de tipo conforme necessário
    return 1.0; // Dano normal
}


Pokemon::Pokemon(std::string nome, std::string tipo_pokemon, int hp, int ataque_stat, int defesa_stat, std::vector<Ataque> ataques_iniciais)
    : nome(nome), tipo_pokemon(tipo_pokemon), hp_max(hp), hp_atual(hp), ataque_stat(ataque_stat), defesa_stat(defesa_stat), ataques(ataques_iniciais) {}

std::string Pokemon::getNome() const {
    return nome;
}

int Pokemon::getHPAtual() const {
    return hp_atual;
}

int Pokemon::getHPMax() const {
    return hp_max;
}

std::string Pokemon::getTipoPokemon() const {
    return tipo_pokemon;
}

bool Pokemon::estaVivo() const {
    return hp_atual > 0;
}

const std::vector<Ataque>& Pokemon::getAtaques() const {
    return ataques;
}

void Pokemon::mostrarAtaques() const {
    std::cout << "Ataques de " << nome << ":\n";
    for (size_t i = 0; i < ataques.size(); ++i) {
        std::cout << i + 1 << ". " << std::left << std::setw(18) << ataques[i].nome
                  << " (Tipo: " << std::setw(8) << ataques[i].tipo
                  << ", Dano: " << std::setw(3) << ataques[i].dano_base
                  << ", PP: " << ataques[i].pp_atual << "/" << ataques[i].pp_max << ")\n";
    }
}

bool Pokemon::usarAtaque(int indice_ataque, Pokemon &alvo) {
    if (indice_ataque < 0 || indice_ataque >= static_cast<int>(ataques.size())) {
        std::cout << "Índice de ataque inválido!\n";
        return false;
    }

    Ataque& ataque_usado = ataques[indice_ataque];

    if (ataque_usado.pp_atual <= 0) {
        std::cout << nome << " tentou usar " << ataque_usado.nome << ", mas não tem PP suficiente!\n";
        return false;
    }

    ataque_usado.pp_atual--;

    std::cout << nome << " usou " << ataque_usado.nome << " em " << alvo.getNome() << "!\n";

    double multiplicador_tipo = calcularMultiplicadorTipo(ataque_usado.tipo, alvo.getTipoPokemon());
    
    int dano_calculado = static_cast<int>(
        (ataque_usado.dano_base * (this->ataque_stat / 50.0) + (this->ataque_stat * 0.2)) * (1.0 - alvo.defesa_stat / 200.0) * multiplicador_tipo
    );

    if (multiplicador_tipo > 0) {
        dano_calculado = std::max(1, dano_calculado);
    } else {
        dano_calculado = 0; 
    }
    
    if (multiplicador_tipo > 1.5) {
        std::cout << "Foi super efetivo!\n";
    } else if (multiplicador_tipo < 0.7 && multiplicador_tipo > 0.0) {
        std::cout << "Não foi muito efetivo...\n";
    } else if (multiplicador_tipo == 0.0) {
        std::cout << alvo.getNome() << " é imune ao ataque!\n";
    }

    alvo.receberDano(dano_calculado);
    return true;
}

void Pokemon::receberDano(int dano) {
    hp_atual -= dano;
    if (hp_atual < 0) hp_atual = 0;
    std::cout << nome << " recebeu " << dano << " de dano. HP restante: " << hp_atual << "/" << hp_max << "\n";
    if (!estaVivo()) {
        std::cout << nome << " desmaiou!\n";
    }
}

void Pokemon::mostrarStatus() const {
    std::cout << nome << " [Tipo: " << tipo_pokemon << ", HP: " << hp_atual << "/" << hp_max << "]\n";
}

void Pokemon::restaurarPP() {
    for (auto& ataque : ataques) {
        ataque.pp_atual = ataque.pp_max;
    }
    std::cout << "PP de " << nome << " foram restaurados!\n";
}