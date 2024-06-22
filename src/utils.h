#ifndef UTILS_H
#define UTILS_H

#include <allegro5/allegro.h>
#include "character.h"

void check_v_key_and_print_position(ALLEGRO_EVENT* ev, player* p1, player* p2);
void printPlayerStatistics(player *p, int i);

#endif
