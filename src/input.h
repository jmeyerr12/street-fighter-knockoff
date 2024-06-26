#ifndef INPUT_H
#define INPUT_H

/*
    Biblioteca de funções que controlam o input do jogador
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "character.h"
#include "constants.h"
#include "joystick.h"

typedef enum {
    PSYCHO_CRUSHER_IDLE,
    PSYCHO_CRUSHER_BACK_HELD,
    PSYCHO_CRUSHER_FORWARD_PRESSED,
    PSYCHO_CRUSHER_PUNCH
} PsychoCrusherState;

void handle_player_input(ALLEGRO_KEYBOARD_STATE* key_state, player* p, const int keys[], int* movement); // Lida com a entrada do teclado para um jogador e atualiza seu movimento
void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, player* player_1, player* player_2, int *movement1, int *movement2); // Lida com a entrada do teclado para ambos os jogadores e atualiza seus movimentos


#endif
