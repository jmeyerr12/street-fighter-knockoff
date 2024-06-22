#ifndef COLLISION_H
#define COLLISION_H

#include "character.h"
#include "constants.h"
#include "player_type.h"

unsigned char collision_players(player* p1, player* p2);
unsigned char collision_wall(player* p);
unsigned char collision_2D(player* p1, player* p2);
void resolve_collision(player* p1, player* p2);
void out_of_bounds(player* p);

#endif
