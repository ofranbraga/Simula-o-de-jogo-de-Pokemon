#include <iostream>
#include <vector>
#include <cstdlib> // Para std::rand e std::srand
#include <ctime>   // Para std::time
#include <algorithm> // Para std::shuffle, std::find
#include <map>
#include "Pokemon.hpp"
#include "../utils.hpp" 


// Estrutura para Itens
enum class TipoEfeitoItem { CURAR_HP, RESTAURAR_PP_UM_ATAQUE, RESTAURAR_PP_TODOS };
struct Item {
    std::string nome;
    std::string descricao;
    TipoEfeitoItem efeito;
    int valor; // Quantidade de HP ou PP
    int quantidade_em_estoque;

    Item(std::string n, std::string d, TipoEfeitoItem e, int v, int q)
        : nome(n), descricao(d), efeito(e), valor(v), quantidade_em_estoque(q) {}
};

// Função para verificar se um time inteiro foi derrotado
bool timeDerrotado(const std::vector<Pokemon*>& time) {
    for (const auto* p : time) {
        if (p->estaVivo()) {
            return false;
        }
    }
    return true;
}

// Estrutura para facilitar a definição inicial dos Pokémon e seus ataques
struct AtaqueDef {
    std::string nome;
    std::string tipo;
    int dano_base;
    int pp_max;
};

struct PokemonDef {
    std::string nome;
    std::string tipo1;
    std::string tipo2; // Pode ser "" se não tiver segundo tipo
    int hp;
    int ataque_stat;
    int defesa_stat;
    std::vector<AtaqueDef> ataques_defs; // Máximo de 4 ataques
};

// Função para inicializar o banco de dados completo de Pokémon
std::vector<Pokemon> inicializarBancoGlobalPokemons() {
    std::vector<PokemonDef> definicoes_pokemons = {
        // Kanto Starters
        {"Bulbasaur", "Grama", "Veneno", 45, 49, 49, {
            {"Investida", "Normal", 40, 35}, {"Folha Navalha", "Grama", 55, 25},
            {"Poison Powder", "Veneno", 0, 35}, {"Leech Seed", "Grama", 0, 10}}}, // Exemplo de ataque de status
        {"Charmander", "Fogo", "", 39, 52, 43, {
            {"Arranhão", "Normal", 40, 35}, {"Brasa", "Fogo", 40, 25},
            {"Rosnar", "Normal", 0, 40}, {"Lança-Chamas", "Fogo", 90, 15}}},
        {"Squirtle", "Agua", "", 44, 48, 65, {
            {"Investida", "Normal", 40, 35}, {"Pistola D'Água", "Agua", 40, 25},
            {"Cauda Chicote", "Normal", 0, 30}, {"Jato D'Água", "Agua", 90, 15}}},
        // Pikachu
        {"Pikachu", "Eletrico", "", 35, 55, 40, {
            {"Choque do Trovão", "Eletrico", 40, 30}, {"Ataque Rápido", "Normal", 40, 30},
            {"Cauda de Ferro", "Aco", 100, 15}, {"Raio", "Eletrico", 90, 15}}},
        // Outros exemplos
        {"Geodude", "Rocha", "Terra", 40, 80, 100, {
            {"Investida", "Normal", 40, 35}, {"Pedrada", "Rocha", 50, 15},
            {"Rolo Compressor", "Rocha", 75, 20}, {"Terremoto", "Terra", 100, 10}}},
        {"Onix", "Rocha", "Terra", 35, 45, 160, {
            {"Investida", "Normal", 40, 35}, {"Aperto", "Normal", 15, 20},
            {"Tumba de Rochas", "Rocha", 60, 15}, {"Terremoto", "Terra", 100, 10}}},
        {"Machop", "Lutador", "", 70, 80, 50, {
            {"Golpe Baixo", "Lutador", 40, 30}, {"Golpe de Karate", "Lutador", 50, 25},
            {"Foco de Energia", "Normal", 0, 30}, {"Chute Duplo", "Lutador", 30, 30}}}, // Chute Duplo ataca 2x, simplificado aqui
        {"Abra", "Psiquico", "", 25, 20, 15, {
            {"Teleporte", "Psiquico", 0, 20}, {"Confusão", "Psiquico", 50, 25},
            {"Raio Psíquico", "Psiquico", 65, 20}, {"Recuperação", "Psiquico", 0, 10}}}, // Recupera HP
        {"Gastly", "Fantasma", "Veneno", 30, 35, 30, {
            {"Lamber", "Fantasma", 30, 30}, {"Olhar Maligno", "Normal", 0, 30},
            {"Bola Sombria", "Fantasma", 80, 15}, {"Hipnose", "Psiquico", 0, 20}}},
        {"Magikarp", "Agua", "", 20, 10, 55, {
            {"Splash", "Normal", 0, 40}, {"Investida", "Normal", 40, 35}, // Dando uma ajudinha
            {"Flail", "Normal", 0, 15}, {"Hidrobomba", "Agua", 110, 5}}}, // Magikarp poderoso!
        // Adicione os outros 141 Pokémon aqui...
        // Exemplo: {"Nome", "Tipo1", "Tipo2", HP, ATK, DEF, {Ataque1Def, Ataque2Def, Ataque3Def, Ataque4Def}},
    };

    std::vector<Pokemon> banco_final;
    for (const auto& p_def : definicoes_pokemons) {
        std::vector<Ataque> ataques_pokemon;
        for (const auto& a_def : p_def.ataques_defs) {
            ataques_pokemon.emplace_back(a_def.nome, a_def.tipo, a_def.dano_base, a_def.pp_max);
        }
        banco_final.emplace_back(p_def.nome, p_def.tipo1, p_def.tipo2, p_def.hp, p_def.ataque_stat, p_def.defesa_stat, ataques_pokemon);
    }
    return banco_final;
}

// Função para exibir a lista de Pokémon para seleção
void exibirListaSelecaoPokemon(const std::vector<Pokemon>& banco_completo, 
                               const std::vector<int>& indices_ja_escolhidos_geral,
                               const std::vector<Pokemon*>& time_parcial_jogador,
                               int numero_pokemon_atual, int tamanho_time_total) {
    limparTerminal();
    std::cout << "--- Montando seu Time (" << numero_pokemon_atual << "/" << tamanho_time_total << ") ---\n";
    if (!time_parcial_jogador.empty()) {
        std::cout << "Seu time até agora:\n";
        for (const auto* p : time_parcial_jogador) {
            std::cout << " - " << p->getNome() << "\n";
        }
        std::cout << "------------------------\n";
    }
    std::cout << "Escolha o próximo Pokémon:\n";
    for (size_t i = 0; i < banco_completo.size(); ++i) {
        bool ja_escolhido = false;
        for (int idx_escolhido : indices_ja_escolhidos_geral) {
            if (static_cast<int>(i) == idx_escolhido) {
                ja_escolhido = true;
                break;
            }
        }
        if (!ja_escolhido) {
            std::cout << i + 1 << ". " << banco_completo[i].getNome()
                      << " (Tipo: " << banco_completo[i].getTipoPokemon1();
            if (!banco_completo[i].getTipoPokemon2().empty()) {
                std::cout << "/" << banco_completo[i].getTipoPokemon2();
            }
            std::cout << ")\n";
        }
    }
    std::cout << "------------------------\n";
}

// Função para o jogador montar seu time
std::vector<Pokemon*> montarTime(std::vector<Pokemon>& banco_geral, int tamanho_time, const std::string& nome_time) {
    std::vector<Pokemon*> time_montado;
    std::vector<int> indices_ja_escolhidos_neste_time;

    for (int k = 0; k < tamanho_time; ++k) {
        exibirListaSelecaoPokemon(banco_geral, indices_ja_escolhidos_neste_time, time_montado, k + 1, tamanho_time);
        int escolha_idx_banco;
        while (true) {
            std::cout << "Escolha para " << nome_time << " (Pokémon " << k + 1 << "): ";
            if (!(std::cin >> escolha_idx_banco) || escolha_idx_banco < 1 || escolha_idx_banco > static_cast<int>(banco_geral.size())) {
                std::cout << "Escolha inválida. Tente novamente.\n";
                limparBufferEntrada();
                continue;
            }
            bool ja_escolhido_loop = false;
            for (int idx_escolhido : indices_ja_escolhidos_neste_time) {
                if (escolha_idx_banco - 1 == idx_escolhido) {
                    ja_escolhido_loop = true;
                    break;
                }
            }
            if (ja_escolhido_loop) {
                std::cout << "Pokémon já escolhido para este time. Tente novamente.\n";
            } else {
                time_montado.push_back(&banco_geral[escolha_idx_banco - 1]);
                indices_ja_escolhidos_neste_time.push_back(escolha_idx_banco - 1);
                break;
            }
        }
        limparBufferEntrada(); // Limpa o buffer após uma escolha válida
    }
    limparTerminal();
    std::cout << nome_time << " montado!\n";
    for(const auto* p : time_montado) std::cout << "- " << p->getNome() << "\n";
    std::cout << "Pressione Enter para continuar...";
    std::cin.get(); 
    return time_montado;
}

// Função para montar um time aleatório
std::vector<Pokemon*> montarTimeAleatorio(std::vector<Pokemon>& banco_geral, int tamanho_time, const std::vector<Pokemon*>& time_a_evitar = {}) {
    std::vector<Pokemon*> time_montado;
    std::vector<int> indices_banco_geral;
    for(size_t i = 0; i < banco_geral.size(); ++i) indices_banco_geral.push_back(i);

    std::random_shuffle(indices_banco_geral.begin(), indices_banco_geral.end()); // Requer <algorithm> e <random> se quiser um gerador melhor

    for(int idx_banco : indices_banco_geral) {
        if (time_montado.size() >= static_cast<size_t>(tamanho_time)) break;

        Pokemon* candidato = &banco_geral[idx_banco];
        bool ja_no_time_evitar = false;
        for(const auto* p_evitar : time_a_evitar) {
            if (p_evitar == candidato) {
                ja_no_time_evitar = true;
                break;
            }
        }
        if (!ja_no_time_evitar) {
            time_montado.push_back(candidato);
        }
    }
    return time_montado;
}


int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed para números aleatórios

    std::vector<Pokemon> banco_global_pokemons = inicializarBancoGlobalPokemons();

    // Inventário do Jogador
    std::vector<Item> inventario_jogador = {
        Item("Poção", "Restaura 20 HP", TipoEfeitoItem::CURAR_HP, 20, 3),
        Item("Super Poção", "Restaura 50 HP", TipoEfeitoItem::CURAR_HP, 50, 1),
        Item("Éter", "Restaura 10 PP de um ataque", TipoEfeitoItem::RESTAURAR_PP_UM_ATAQUE, 10, 2),
        Item("Elixir", "Restaura todo PP de todos os ataques", TipoEfeitoItem::RESTAURAR_PP_TODOS, 0, 1)
    };

    limparTerminal();
    std::cout << "****************************************\n";
    std::cout << "Bem-vindo à Batalha Pokémon!\n";
    std::cout << "****************************************\n\n";
    
    std::vector<Pokemon*> time_jogador;
    std::vector<Pokemon*> time_oponente;
    int tamanho_time = 3; // Jogador escolhe 3 Pokémon

    std::cout << "Escolha o modo de jogo:\n";
    std::cout << "1. Modo Clássico (Escolha seu time)\n";
    std::cout << "2. Modo Aleatório (Times aleatórios)\n";
    std::cout << "Escolha (1-2): ";
    int modo_jogo;
    std::cin >> modo_jogo;
    limparBufferEntrada();

    if (modo_jogo == 1) {
        std::cout << "\n--- Modo Clássico Selecionado ---\n";
        time_jogador = montarTime(banco_global_pokemons, tamanho_time, "Seu Time");
        // Oponente pode ser aleatório ou também escolhido, por simplicidade, será aleatório
        std::cout << "\nMontando time do oponente...\n";
        time_oponente = montarTimeAleatorio(banco_global_pokemons, tamanho_time, time_jogador);
         std::cout << "Time do Oponente Montado Aleatoriamente!\n";
         for(const auto* p : time_oponente) std::cout << "- " << p->getNome() << "\n";
         std::cout << "Pressione Enter para continuar...";
         std::cin.get();

    } else if (modo_jogo == 2) {
        std::cout << "\n--- Modo Aleatório Selecionado ---\n";
        std::cout << "Montando seu time aleatoriamente...\n";
        time_jogador = montarTimeAleatorio(banco_global_pokemons, tamanho_time);
        std::cout << "Seu Time Aleatório:\n";
        for(const auto* p : time_jogador) std::cout << "- " << p->getNome() << "\n";
        
        std::cout << "\nMontando time do oponente aleatoriamente...\n";
        time_oponente = montarTimeAleatorio(banco_global_pokemons, tamanho_time, time_jogador);
        std::cout << "Time do Oponente Aleatório:\n";
        for(const auto* p : time_oponente) std::cout << "- " << p->getNome() << "\n";
        std::cout << "Pressione Enter para continuar...";
        std::cin.get();
    } else {
        std::cout << "Modo inválido. Saindo.\n";
        return 1;
    }

    Pokemon* pokemon_ativo_jogador = time_jogador[0];
    Pokemon* pokemon_ativo_oponente = time_oponente[0];

    std::cout << "\n========================================\n";
    std::cout << "Seu Time:\n";
    for(const auto* p : time_jogador) std::cout << "- " << p->getNome() << "\n";
    std::cout << "========================================\n";
    std::cout << "Time do Oponente:\n";
    for(const auto* p : time_oponente) std::cout << "- " << p->getNome() << "\n";
    std::cout << "========================================\n";

    limparTerminal();
    std::cout << "\n>>> Você envia " << pokemon_ativo_jogador->getNome() << "!\n";
    pokemon_ativo_jogador->mostrarStatus();
    std::cout << "\n>>> Oponente envia " << pokemon_ativo_oponente->getNome() << "!\n";
    pokemon_ativo_oponente->mostrarStatus();
    std::cout << "\n<<<<< BATALHA INICIADA! >>>>>\n";

    int turno = 1;
    bool batalha_continua = true;

    while (batalha_continua) {
        std::cout << "\n--- Turno " << turno << " --- \n";
        // Vez do Jogador
        if (pokemon_ativo_jogador->estaVivo()) {
            std::cout << "\n----------------------------------------\n";
            std::cout << " Vez de " << pokemon_ativo_jogador->getNome() << " (Jogador):\n";
            std::cout << "----------------------------------------\n";
            pokemon_ativo_jogador->mostrarStatus();
            pokemon_ativo_oponente->mostrarStatus();
            std::cout << "\nO que " << pokemon_ativo_jogador->getNome() << " deve fazer?\n";
            std::cout << "  1. Lutar\n  2. Usar Item\n  3. Trocar Pokémon\nEscolha (1-3): ";
            int acao_jogador;
            std::cin >> acao_jogador;
            limparBufferEntrada();

            bool turno_jogador_concluido = false;
            if (acao_jogador == 1) { // Lutar
                pokemon_ativo_jogador->mostrarAtaques();
                int escolha_ataque_idx;
                bool ataque_valido = false;
                while(!ataque_valido) {
                    std::cout << "Escolha seu ataque (1-" << pokemon_ativo_jogador->getAtaques().size() << "): ";
                    if (!(std::cin >> escolha_ataque_idx) || escolha_ataque_idx < 1 || escolha_ataque_idx > static_cast<int>(pokemon_ativo_jogador->getAtaques().size())) {
                        std::cout << "Escolha de ataque inválida.\n";
                        limparBufferEntrada();
                    } else {
                        ataque_valido = pokemon_ativo_jogador->usarAtaque(escolha_ataque_idx - 1, *pokemon_ativo_oponente);
                    }
                }
                limparBufferEntrada();
                turno_jogador_concluido = true;
            } else if (acao_jogador == 2) { // Usar Item
                std::cout << "\n--- Inventário ---\n";
                for(size_t i = 0; i < inventario_jogador.size(); ++i) {
                    if(inventario_jogador[i].quantidade_em_estoque > 0) {
                        std::cout << i + 1 << ". " << inventario_jogador[i].nome << " (" << inventario_jogador[i].descricao << ") - Restam: " << inventario_jogador[i].quantidade_em_estoque << "\n";
                    }
                }
                std::cout << "------------------\n";
                std::cout << "Escolha um item para usar (ou 0 para cancelar): ";
                int escolha_item_idx;
                std::cin >> escolha_item_idx;
                limparBufferEntrada();
                if (escolha_item_idx > 0 && escolha_item_idx <= static_cast<int>(inventario_jogador.size()) && inventario_jogador[escolha_item_idx-1].quantidade_em_estoque > 0) {
                    Item& item_usado = inventario_jogador[escolha_item_idx-1];
                    std::cout << "Usando " << item_usado.nome << "...\n";
                    Pokemon* alvo_item = pokemon_ativo_jogador; // Por padrão, em si mesmo
                    // Poderia adicionar lógica para escolher alvo se o item for para outro Pokémon do time

                    if (item_usado.efeito == TipoEfeitoItem::CURAR_HP) {
                        alvo_item->curar(item_usado.valor);
                    } else if (item_usado.efeito == TipoEfeitoItem::RESTAURAR_PP_UM_ATAQUE) {
                        std::cout << "Qual ataque de " << alvo_item->getNome() << " restaurar PP?\n";
                        alvo_item->mostrarAtaques();
                        int idx_ataque_pp;
                        std::cin >> idx_ataque_pp;
                        limparBufferEntrada();
                        if(idx_ataque_pp > 0 && idx_ataque_pp <= static_cast<int>(alvo_item->getAtaques().size())) {
                            alvo_item->restaurarPPUmAtaque(idx_ataque_pp - 1, item_usado.valor);
                        } else { std::cout << "Escolha de ataque inválida.\n"; continue; /* Não consome item nem turno */ }
                    } else if (item_usado.efeito == TipoEfeitoItem::RESTAURAR_PP_TODOS) {
                        alvo_item->restaurarPP();
                    }
                    item_usado.quantidade_em_estoque--;
                    turno_jogador_concluido = true;
                } else { std::cout << "Nenhum item usado ou escolha inválida.\n"; }

            } else if (acao_jogador == 3) { // Trocar Pokémon
                std::cout << "\n--- Trocar Pokémon ---\n";
                int pokemons_disponiveis_troca = 0;
                for (size_t i = 0; i < time_jogador.size(); ++i) {
                    if (time_jogador[i]->estaVivo() && time_jogador[i] != pokemon_ativo_jogador) {
                        std::cout << i + 1 << ". " << time_jogador[i]->getNome() << " (HP: " << time_jogador[i]->getHPAtual() << "/" << time_jogador[i]->getHPMax() << ")\n";
                        pokemons_disponiveis_troca++;
                    }
                }
                 std::cout << "----------------------\n";
                if (pokemons_disponiveis_troca == 0) {
                    std::cout << "Nenhum Pokémon disponível para troca!\n";
                } else {                    
                    int escolha_troca_idx;
                    std::cin >> escolha_troca_idx;
                    limparBufferEntrada();
                    if (escolha_troca_idx > 0 && escolha_troca_idx <= static_cast<int>(time_jogador.size()) &&
                        time_jogador[escolha_troca_idx - 1]->estaVivo() && time_jogador[escolha_troca_idx - 1] != pokemon_ativo_jogador) {
                        pokemon_ativo_jogador = time_jogador[escolha_troca_idx - 1];
                        std::cout << "Vai, " << pokemon_ativo_jogador->getNome() << "!\n";
                        turno_jogador_concluido = true;
                    } else { std::cout << "Troca inválida.\n"; }
                }
            } else {
                std::cout << "Ação inválida.\n";
            }
            if (!turno_jogador_concluido) continue; // Repete o turno do jogador se a ação não foi concluída
        }

        // Verificar se o Pokémon do oponente desmaiou
        if (!pokemon_ativo_oponente->estaVivo()) {
            std::cout << "\n!!! " << pokemon_ativo_oponente->getNome() << " do oponente desmaiou! !!!\n";
            if (timeDerrotado(time_oponente)) {
                std::cout << "\nTodos os Pokémon do oponente foram derrotados!\n";
                batalha_continua = false;
                break;
            }
            // Oponente escolhe o próximo Pokémon
            Pokemon* proximo_oponente = nullptr;
            for (Pokemon* p : time_oponente) {
                if (p->estaVivo()) {
                    proximo_oponente = p;
                    break;
                }
            }
            if (proximo_oponente) {
                pokemon_ativo_oponente = proximo_oponente;
                std::cout << "\n>>> Oponente envia " << pokemon_ativo_oponente->getNome() << "!\n";
                pokemon_ativo_oponente->mostrarStatus();
            } // else (should not happen if timeDerrotado is false)
        }
        if (!batalha_continua) break;

        std::cout << "\n---\n";
        // Vez do Oponente
        if (pokemon_ativo_oponente->estaVivo()) {
            std::cout << "\n----------------------------------------\n";
            std::cout << " Vez de " << pokemon_ativo_oponente->getNome() << " (Oponente):\n";
            std::cout << "----------------------------------------\n";
            const auto& ataques_oponente = pokemon_ativo_oponente->getAtaques();
            std::vector<int> ataques_disponiveis_oponente_idx;
            for(size_t i = 0; i < ataques_oponente.size(); ++i) {
                if (ataques_oponente[i].pp_atual > 0) {
                    ataques_disponiveis_oponente_idx.push_back(i);
                }
            }
            if (!ataques_disponiveis_oponente_idx.empty()) {
                int indice_ataque_oponente_escolhido = ataques_disponiveis_oponente_idx[std::rand() % ataques_disponiveis_oponente_idx.size()];
                pokemon_ativo_oponente->usarAtaque(indice_ataque_oponente_escolhido, *pokemon_ativo_jogador);
            } else {
                std::cout << pokemon_ativo_oponente->getNome() << " não tem mais ataques com PP e perdeu a vez!\n";
            }
        }

        // Verificar se o Pokémon do jogador desmaiou
        if (!pokemon_ativo_jogador->estaVivo()) {
            std::cout << "\n!!! " << pokemon_ativo_jogador->getNome() << " desmaiou! !!!\n";
            if (timeDerrotado(time_jogador)) {
                std::cout << "\nTodos os seus Pokémon foram derrotados!\n";
                batalha_continua = false;
                break;
            }
            // Jogador escolhe o próximo Pokémon
            std::cout << "\n--- Escolha seu próximo Pokémon ---\n";
            int pokemons_vivos_jogador = 0;
            for (size_t i = 0; i < time_jogador.size(); ++i) {
                if (time_jogador[i]->estaVivo()) {
                    std::cout << i + 1 << ". " << time_jogador[i]->getNome() << " (HP: " << time_jogador[i]->getHPAtual() << "/" << time_jogador[i]->getHPMax() << ")\n";
                    pokemons_vivos_jogador++;
                }
            }
            std::cout << "---------------------------------\n";
            if (pokemons_vivos_jogador > 0) {
                 int escolha_proximo_idx;
                 while(true) {
                    std::cin >> escolha_proximo_idx;
                    limparBufferEntrada();
                    if (escolha_proximo_idx > 0 && escolha_proximo_idx <= static_cast<int>(time_jogador.size()) && time_jogador[escolha_proximo_idx - 1]->estaVivo()) {
                        pokemon_ativo_jogador = time_jogador[escolha_proximo_idx - 1];
                        std::cout << "\n>>> Vai, " << pokemon_ativo_jogador->getNome() << "!\n";
                        pokemon_ativo_jogador->mostrarStatus();
                        break;
                    } else { std::cout << "Escolha inválida.\n"; }
                 }
            } // else (should not happen if timeDerrotado is false)
        }
        if (!batalha_continua) break;
        turno++;
    }

    std::cout << "\n****************************************\n";
    std::cout << "           FIM DA BATALHA           \n";
    std::cout << "****************************************\n";
    if (!timeDerrotado(time_jogador) && timeDerrotado(time_oponente)) {
        std::cout << "Você venceu a batalha!\n";
    } else if (timeDerrotado(time_jogador) && !timeDerrotado(time_oponente)) {
        std::cout << "Você perdeu a batalha!\n";
    } else {
        // Caso de empate ou ambos os times derrotados simultaneamente (raro com esta lógica)
        std::cout << "A batalha terminou.\n";
    }
    return 0;
}