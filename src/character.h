#ifndef CHARACTER_H
#define CHARACTER_H

#include <allegro5/allegro.h>
#include <stdio.h>
#include "joystick.h"
#include "constants.h"
#include "collision.h"
#include "player_type.h"

// Funções relacionadas aos jogadores
player* buildPlayer(unsigned int width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned int height, int direction);
void setDimensions(player* p, unsigned width, unsigned int height);
void resetAttributes(player **p, unsigned int width, unsigned int height, unsigned short x, unsigned short y);
void movePlayer(player *element, char steps, unsigned char trajectory);
void setHitbox(player* p);
void updatePlayer(player *element, float time, unsigned short groundLevel, unsigned int bounds);
void resetPlayer(player* p);
void recharge_stamina(player* p);
void try_move_player(player *p, player *other, int multiplier, int direction);
size** characterSizes();
void freeCharacterSizes(size** charSizes);
bool isInRange(player *attacker, player *defender, int attack);

#endif
