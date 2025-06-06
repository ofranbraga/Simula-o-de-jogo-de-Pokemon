#include "Pokemon.hpp"
#include <iostream>
#include <algorithm> // Para std::max
#include <iomanip>   // Para std::setw

// Função auxiliar para calcular a efetividade de um tipo de ataque contra um tipo de defesa
double calcularEfetividadeIndividual(const std::string& tipo_ataque, const std::string& tipo_defesa) {
    if (tipo_defesa.empty()) return 1.0; // Sem tipo defensivo (não deve acontecer para tipo1)

    if (tipo_ataque == "Fogo") {
        if (tipo_defesa == "Grama" || tipo_defesa == "Inseto" || tipo_defesa == "Aco" || tipo_defesa == "Gelo") return 2.0;
        if (tipo_defesa == "Agua" || tipo_defesa == "Fogo" || tipo_defesa == "Rocha" || tipo_defesa == "Dragao") return 0.5;
    } else if (tipo_ataque == "Agua") {
        if (tipo_defesa == "Fogo" || tipo_defesa == "Rocha" || tipo_defesa == "Terra") return 2.0;
        if (tipo_defesa == "Grama" || tipo_defesa == "Agua" || tipo_defesa == "Dragao") return 0.5;
    } else if (tipo_ataque == "Grama") {
        if (tipo_defesa == "Agua" || tipo_defesa == "Rocha" || tipo_defesa == "Terra") return 2.0;
        if (tipo_defesa == "Fogo" || tipo_defesa == "Grama" || tipo_defesa == "Veneno" || tipo_defesa == "Voador" || tipo_defesa == "Inseto" || tipo_defesa == "Dragao" || tipo_defesa == "Aco") return 0.5;
    } else if (tipo_ataque == "Eletrico") {
        if (tipo_defesa == "Agua" || tipo_defesa == "Voador") return 2.0;
        if (tipo_defesa == "Grama" || tipo_defesa == "Eletrico" || tipo_defesa == "Dragao") return 0.5;
        if (tipo_defesa == "Terra") return 0.0; // Imune
    } else if (tipo_ataque == "Normal") {
        if (tipo_defesa == "Rocha" || tipo_defesa == "Aco") return 0.5;
        if (tipo_defesa == "Fantasma") return 0.0; // Imune
    } else if (tipo_ataque == "Aco") {
        if (tipo_defesa == "Gelo" || tipo_defesa == "Rocha" || tipo_defesa == "Fada") return 2.0;
        if (tipo_defesa == "Fogo" || tipo_defesa == "Agua" || tipo_defesa == "Eletrico" || tipo_defesa == "Aco") return 0.5;
    } else if (tipo_ataque == "Lutador") {
        if (tipo_defesa == "Normal" || tipo_defesa == "Gelo" || tipo_defesa == "Rocha" || tipo_defesa == "Sombrio" || tipo_defesa == "Aco") return 2.0;
        if (tipo_defesa == "Veneno" || tipo_defesa == "Voador" || tipo_defesa == "Psiquico" || tipo_defesa == "Inseto" || tipo_defesa == "Fada") return 0.5;
        if (tipo_defesa == "Fantasma") return 0.0;
    } else if (tipo_ataque == "Terra") {
        if (tipo_defesa == "Fogo" || tipo_defesa == "Eletrico" || tipo_defesa == "Veneno" || tipo_defesa == "Rocha" || tipo_defesa == "Aco") return 2.0;
        if (tipo_defesa == "Grama" || tipo_defesa == "Inseto") return 0.5;
        if (tipo_defesa == "Voador") return 0.0;
    } else if (tipo_ataque == "Rocha") {
        if (tipo_defesa == "Fogo" || tipo_defesa == "Gelo" || tipo_defesa == "Voador" || tipo_defesa == "Inseto") return 2.0;
        if (tipo_defesa == "Lutador" || tipo_defesa == "Terra" || tipo_defesa == "Aco") return 0.5;
    }
    // Adicionar mais interações de tipo conforme necessário
    return 1.0; // Dano normal
}

// Função para calcular o multiplicador de tipo total considerando os dois tipos do alvo
double calcularMultiplicadorTipoTotal(const std::string& tipo_ataque, const std::string& tipo_alvo1, const std::string& tipo_alvo2) {
    double mult1 = calcularEfetividadeIndividual(tipo_ataque, tipo_alvo1);
    if (mult1 == 0.0) return 0.0; // Imunidade no tipo primário anula tudo

    double mult2 = 1.0;
    if (!tipo_alvo2.empty()) { // Se houver um segundo tipo
        mult2 = calcularEfetividadeIndividual(tipo_ataque, tipo_alvo2);
        if (mult2 == 0.0) return 0.0; // Imunidade no tipo secundário anula tudo
    }
    return mult1 * mult2; // Multiplica as efetividades (ex: 2.0 * 2.0 = 4.0; 0.5 * 0.5 = 0.25)
}

Pokemon::Pokemon(std::string nome, std::string tipo1, std::string tipo2, int hp, int ataque_stat, int defesa_stat, std::vector<Ataque> ataques_iniciais)
    : nome(nome), tipo_pokemon1(tipo1), tipo_pokemon2(tipo2), hp_max(hp), hp_atual(hp), ataque_stat(ataque_stat), defesa_stat(defesa_stat), ataques(ataques_iniciais) {}

std::string Pokemon::getNome() const {
    return nome;
}

int Pokemon::getHPAtual() const {
    return hp_atual;
}

int Pokemon::getHPMax() const {
    return hp_max;
}

std::string Pokemon::getTipoPokemon1() const {
    return tipo_pokemon1;
}

std::string Pokemon::getTipoPokemon2() const {
    return tipo_pokemon2;
}

bool Pokemon::estaVivo() const {
    return hp_atual > 0;
}

const std::vector<Ataque>& Pokemon::getAtaques() const {
    return ataques;
}

void Pokemon::mostrarAtaques() const {
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << " Ataques de " << nome << ":\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "| # | Nome do Ataque    | Tipo       | Dano | PP Atual/Max   |\n";
    std::cout << "|---|-------------------|------------|------|----------------|\n";
    for (size_t i = 0; i < ataques.size(); ++i) {
        std::cout << "| " << std::setw(1) << i + 1 << " | "
                  << std::left << std::setw(17) << ataques[i].nome << " | "
                  << std::left << std::setw(10) << ataques[i].tipo << " | "
                  << std::right << std::setw(4) << ataques[i].dano_base << " | "
                  << std::right << std::setw(2) << ataques[i].pp_atual << "/" << std::left << std::setw(11) << ataques[i].pp_max << " |\n";
    }
    std::cout << "-----------------------------------------------------------------\n";
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

    double multiplicador_tipo = calcularMultiplicadorTipoTotal(ataque_usado.tipo, alvo.getTipoPokemon1(), alvo.getTipoPokemon2());
    
    int dano_calculado = static_cast<int>(
        (ataque_usado.dano_base * (this->ataque_stat / 50.0) + (this->ataque_stat * 0.2)) * (1.0 - alvo.defesa_stat / 200.0) * multiplicador_tipo
    );

    if (multiplicador_tipo > 0) {
        dano_calculado = std::max(1, dano_calculado);
    } else {
        dano_calculado = 0; 
    }
    
    if (multiplicador_tipo >= 3.9) { // 4x
        std::cout << "Foi SUPER EFETIVO AO EXTREMO (4x)!!!\n";
    } else if (multiplicador_tipo >= 1.9) { // 2x
        std::cout << "Foi super efetivo!\n";
    } else if (multiplicador_tipo <= 0.26 && multiplicador_tipo > 0.0) { // 0.25x
        std::cout << "Não foi NADA efetivo... (0.25x)\n";
    } else if (multiplicador_tipo <= 0.6 && multiplicador_tipo > 0.0) { // 0.5x
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
    std::cout << nome << " [Tipo: " << tipo_pokemon1;
    if (!tipo_pokemon2.empty()) {
        std::cout << "/" << tipo_pokemon2;
    }
    std::cout << ", HP: " << hp_atual << "/" << hp_max << "]\n";
}

void Pokemon::restaurarPP() {
    for (auto& ataque : ataques) {
        ataque.pp_atual = ataque.pp_max;
    }
    std::cout << "PP de " << nome << " foram restaurados!\n";
}

void Pokemon::curar(int quantidade) {
    if (!estaVivo()) {
        // Para reviver, seria um item diferente ou lógica adicional
        std::cout << nome << " está desmaiado e não pode ser curado diretamente desta forma.\n";
        return;
    }
    hp_atual += quantidade;
    if (hp_atual > hp_max) {
        hp_atual = hp_max;
    }
    std::cout << nome << " recuperou " << quantidade << " de HP! HP atual: " << hp_atual << "/" << hp_max << "\n";
}

void Pokemon::restaurarPPUmAtaque(int indice_ataque, int quantidade) {
    if (indice_ataque < 0 || indice_ataque >= static_cast<int>(ataques.size())) {
        std::cout << "Índice de ataque inválido para restaurar PP!\n";
        return;
    }
    Ataque& ataque_alvo = ataques[indice_ataque];
    ataque_alvo.pp_atual += quantidade;
    if (ataque_alvo.pp_atual > ataque_alvo.pp_max) {
        ataque_alvo.pp_atual = ataque_alvo.pp_max;
    }
    std::cout << "PP do ataque '" << ataque_alvo.nome << "' de " << nome << " foi restaurado em " << quantidade << "! PP atual: " << ataque_alvo.pp_atual << "/" << ataque_alvo.pp_max << "!\n";
}