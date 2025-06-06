#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>

class Pokemon {
private:
    std::string nome;
    std::string tipo;
    int hp;
    int ataque;
    int defesa;

public:
    Pokemon(std::string nome, std::string tipo, int hp, int ataque, int defesa);
    
    std::string getNome() const;
    int getHP() const;
    bool estaVivo() const;

    void atacar(Pokemon &alvo);
    void receberDano(int dano);
    void mostrarStatus() const;
};

#endif