#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "character.h"
#include "constants.h"

// Declarações de funções de lógica do jogo
void update_position(player *player_1, player *player_2, float time);
int countFrames(int movement);
int verificaFim(player *p1, player *p2, int countdown);
void showWinner(ALLEGRO_FONT *font, int winner);
void updateDimensions(player *p, size** charSizes, int movement);
void handle_down(player *p, int mv, int *frame, int maxFrames, int timer_count);
void being_hit(player *p, int mv, int *frame, int maxFrames);
void defense(player *p, int mv, int *frame, int maxFrames);
void handle_jump(player *p, player *opponent, int *movement);
void check_and_apply_damage(player *p, player *opponent, int damage, int *alreadyDamaged, int attackType);
void handle_attack(player *p, player *opponent, int *movement, int *alreadyDamaged);
player_state determine_state(player* p);
int choose_attack(player* p, int* movement);
void handle_bot_input(player* p1, player* p2, int* movement);

#endif
