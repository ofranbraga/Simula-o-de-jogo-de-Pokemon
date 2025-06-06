#include "Pokemon.hpp"
#include <iostream>
#include <algorithm>

Pokemon::Pokemon(std::string nome, std::string tipo, int hp, int ataque, int defesa)
    : nome(nome), tipo(tipo), hp(hp), ataque(ataque), defesa(defesa) {}

std::string Pokemon::getNome() const {
    return nome;
}

int Pokemon::getHP() const {
    return hp;
}

bool Pokemon::estaVivo() const {
    return hp > 0;
}

void Pokemon::atacar(Pokemon &alvo) {
    std::cout << nome << " ataca " << alvo.getNome() << "!\n";
    int dano = std::max(1, ataque - alvo.defesa); // dano mínimo de 1
    alvo.receberDano(dano);
}

void Pokemon::receberDano(int dano) {
    hp -= dano;
    if (hp < 0) hp = 0;
    std::cout << nome << " recebeu " << dano << " de dano. HP restante: " << hp << "\n";
}

void Pokemon::mostrarStatus() const {
    std::cout << nome << " [Tipo: " << tipo << ", HP: " << hp << "]\n";
}