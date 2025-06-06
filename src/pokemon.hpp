#ifndef POKEMON_HPP
#define POKEMON_HPP

#include <string>
#include <vector>
#include <iostream> // Para std::cout em mostrarAtaques

// Estrutura para representar um ataque
struct Ataque {
    std::string nome;
    std::string tipo; // Tipo do ataque (Fogo, Agua, Eletrico, Normal, etc.)
    int dano_base;
    int pp_max; // Pontos de Poder máximos
    int pp_atual;

    Ataque(std::string n, std::string t, int db, int pp)
        : nome(n), tipo(t), dano_base(db), pp_max(pp), pp_atual(pp) {}
};

class Pokemon {
private:
    std::string nome;
    std::string tipo_pokemon1; // Tipo primário do Pokémon
    std::string tipo_pokemon2; // Tipo secundário do Pokémon (pode ser vazio)
    int hp_max;
    int hp_atual;
    int ataque_stat; // Atributo de ataque do Pokémon
    int defesa_stat; // Atributo de defesa do Pokémon
    std::vector<Ataque> ataques;

public:
    Pokemon(std::string nome, std::string tipo1, std::string tipo2, int hp, int ataque_stat, int defesa_stat, std::vector<Ataque> ataques_iniciais);

    std::string getNome() const;
    int getHPAtual() const;
    int getHPMax() const;
    std::string getTipoPokemon1() const;
    std::string getTipoPokemon2() const;
    bool estaVivo() const;
    const std::vector<Ataque>& getAtaques() const; // Para o oponente escolher um ataque

    void mostrarAtaques() const;
    // Retorna true se o ataque foi bem sucedido, false se não tinha PP
    bool usarAtaque(int indice_ataque, Pokemon &alvo);
    void receberDano(int dano);
    void mostrarStatus() const;
    void restaurarPP(); // Restaura PP de todos os ataques para o máximo
    void curar(int quantidade);
    void restaurarPPUmAtaque(int indice_ataque, int quantidade);
};

#endif