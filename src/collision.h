#ifndef COLLISION_H
#define COLLISION_H

/*
    Biblioteca com funcões responsáveis por detectar, impedir e resolver colisões entre players e com as bordas do display
*/

#include "character.h"
#include "constants.h"
#include "player_type.h"

unsigned char collision_players(player* p1, player* p2); // Verifica a colisão entre dois jogadores
unsigned char collision_wall(player* p); // Verifica se o jogador colidiu com a parede
unsigned char collision_2D(player* p1, player* p2); // Verifica a colisão entre dois jogadores e com as paredes
void resolve_collision(player* p1, player* p2); // Resolve a colisão entre dois jogadores, ajustando suas posições
void out_of_bounds(player* p); // Verifica e ajusta a posição do jogador se estiver fora dos limites

#endif
