#ifndef STREET_FIGHTER_H
#define STREET_FIGHTER_H

/*
    Biblioteca para as funções que executam os rounds do jogo, tanto do modo multiplayer, quanto do single player
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "character.h"
#include "joystick.h"
#include "input.h"
#include "collision.h"
#include "graphics.h"
#include "utils.h"
#include "constants.h"
#include "game.h"

// Declarações de funções principais do jogo
int run_round(ALLEGRO_EVENT_QUEUE* queue, player* player_1, player* player_2, int* state, char* filename, 
              ALLEGRO_FONT* font, ALLEGRO_BITMAP* player1_sheet, ALLEGRO_BITMAP* player2_sheet, int round, int p1Wins, int p2Wins); // Executa uma rodada do jogo para dois jogadores

int run_single_player(ALLEGRO_EVENT_QUEUE* queue, player* player_1, player* player_2, int* state, char* filename, 
                      ALLEGRO_FONT *font, ALLEGRO_BITMAP* player1_sheet, ALLEGRO_BITMAP* player2_sheet, int round, int p1Wins, int p2Wins); // Executa uma rodada do jogo para um jogador contra o bot

#endif