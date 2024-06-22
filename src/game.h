#ifndef GAME_H
#define GAME_H

/*
    Biblioteca de funções ligadas à lógica do jogo
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "character.h"
#include "constants.h"

// Declarações de funções de lógica do jogo
int verificaFim(player *p1, player *p2, int countdown); // Verifica se o jogo terminou com base na saúde dos jogadores e no tempo restante
void showWinner(ALLEGRO_FONT *font, int winner); // Mostra o vencedor do jogo na tela

// Declaração de funções que controlam o bot no modo single player
player_state determine_state(player* p); // Determina o estado do jogador (normal, defensivo, agressivo) com base na saúde
int choose_attack(player* p, int* movement); // Escolhe um ataque aleatório para o jogador
void handle_bot_input(player* p1, player* p2, int* movement); // Lida com a entrada do bot para movimentação e ataques


#endif
