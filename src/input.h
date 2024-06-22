#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "character.h"
#include "constants.h"
#include "joystick.h"

void handle_player_input(ALLEGRO_KEYBOARD_STATE* key_state, player* p, const int keys[], int* movement);
void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, player* player_1, player* player_2, int *movement1, int *movement2);

#endif
