#include "input.h"

#include "input.h"
#include "constants.h"
#include <stdio.h>

bool detect_hadouken(const ALLEGRO_KEYBOARD_STATE *keystate, player* p, const int keys[]) {
    static HadoukenState state = HADOUKEN_IDLE;
    
    int down_key = keys[3];
    int forward_key = (p->direction == RIGHT) ? keys[1] : keys[2];
    int punch_key = keys[4];

    switch (state) {
        case HADOUKEN_IDLE:
            if (al_key_down(keystate, down_key)) {
                state = HADOUKEN_DOWN;
            }
            break;

        case HADOUKEN_DOWN:
            if (al_key_down(keystate, down_key) && al_key_down(keystate, forward_key)) {
                state = HADOUKEN_DOWN_FORWARD;
            } else if (!al_key_down(keystate, down_key)) {
                state = HADOUKEN_IDLE;
            }
            break;

        case HADOUKEN_DOWN_FORWARD:
            if (al_key_down(keystate, forward_key)) {
                state = HADOUKEN_FORWARD;
            } else if (!al_key_down(keystate, down_key) || !al_key_down(keystate, forward_key)) {
                state = HADOUKEN_IDLE;
            }
            break;

        case HADOUKEN_FORWARD:
            if (al_key_down(keystate, punch_key)) {
                state = HADOUKEN_IDLE;
                return true; // Hadouken detected
            } else if (!al_key_down(keystate, forward_key)) {
                state = HADOUKEN_IDLE;
            }
            break;

        default:
            state = HADOUKEN_IDLE;
            break;
    }

    return false;
}

bool detect_psycho_crusher(const ALLEGRO_KEYBOARD_STATE *keystate, player* p, const int keys[]) {
    static PsychoCrusherState state = PSYCHO_CRUSHER_IDLE;
    static double back_held_time = 0.0;
    static const double CHARGE_TIME = 1.0; // Segure para trás por 1 segundo (ajustar conforme necessário)

    int back_key = (p->direction == RIGHT) ? keys[2] : keys[1]; // Esquerda ou direita baseada na direção
    int forward_key = (p->direction == RIGHT) ? keys[1] : keys[2]; // Esquerda ou direita baseada na direção
    int punch_key = keys[4]; // Chave de Soco

    if (state == PSYCHO_CRUSHER_IDLE) {
        if (al_key_down(keystate, back_key)) {
            printf("back\n");
            fflush(stdout);
            state = PSYCHO_CRUSHER_BACK_HELD;
            back_held_time = al_get_time();
        }
    } else if (state == PSYCHO_CRUSHER_BACK_HELD) {
        if (!al_key_down(keystate, back_key)) {
            state = PSYCHO_CRUSHER_IDLE;
        } else if (al_get_time() - back_held_time >= CHARGE_TIME) {
            printf("tempo\n");
            fflush(stdout);
            state = PSYCHO_CRUSHER_FORWARD_PRESSED;
        }
    } else if (state == PSYCHO_CRUSHER_FORWARD_PRESSED) {
        if (al_key_down(keystate, forward_key)) {
            printf("fwd\n");
            fflush(stdout);
            state = PSYCHO_CRUSHER_PUNCH;
        } else if (!al_key_down(keystate, back_key)) {
            state = PSYCHO_CRUSHER_IDLE;
        }
    } else if (state == PSYCHO_CRUSHER_PUNCH) {
        if (al_key_down(keystate, punch_key)) {
            state = PSYCHO_CRUSHER_IDLE;
            printf("da soco\n");
            fflush(stdout);
            return true; // Psycho Crusher detectado
        } else if (!al_key_down(keystate, forward_key)) {
            state = PSYCHO_CRUSHER_IDLE;
        }
    }
    return false;
}

void handle_player_input(ALLEGRO_KEYBOARD_STATE* key_state, player* p, const int keys[], int* movement) {
    if (!p->isBeingHit) {    
        if (al_key_down(key_state, keys[0])) { // UP
            if (al_key_down(key_state, keys[2])) { // RIGHT
                joystick_up_right(p->control);
                *movement = JUMP_FWD;
            } else if (al_key_down(key_state, keys[1])) { // LEFT
                joystick_up_left(p->control);
                *movement = JUMP_BCK;
            } else {
                joystick_up(p->control);
                *movement = JUMP;
            }
        } else if (al_key_down(key_state, keys[3])) { // DOWN
            if (al_key_down(key_state, keys[4]) && p->attack == 0) { // PUNCH
                *movement = DOWN_PUNCH;
                p->attack = ATTACK_DOWN_PUNCH;
                p->stamina -= STAMINA_DECREASE;      
            } else if (al_key_down(key_state, keys[5]) && p->attack == 0) { // KICK
                *movement = DOWN_KICK;
                p->attack = ATTACK_DOWN_KICK;
                p->stamina -= STAMINA_DECREASE;
            } else if (al_key_down(key_state, keys[1])) {
                joystick_down_left(p->control);
            } else if (al_key_down(key_state, keys[2])) {
                joystick_down_right(p->control);
            } else {
                *movement = GET_DOWN;
                joystick_down(p->control);
            }
            p->isDown = 1;
        } else if (al_key_down(key_state, keys[4]) && p->attack == 0) { // PUNCH
            if (p->isJumping) {
                p->attack = ATTACK_JUMPING_PUNCH;
                *movement = JUMPING_PUNCH;
                p->stamina -= STAMINA_DECREASE;
            } else {
                p->attack = ATTACK_PUNCH;
                *movement = PUNCH;
                p->stamina -= STAMINA_DECREASE;
            }
        } else if (al_key_down(key_state, keys[5]) && p->attack == 0) { // KICK
            if (p->isJumping) {
                p->attack = ATTACK_JUMPING_KICK;
                *movement = JUMPING_KICK;
                p->stamina -= STAMINA_DECREASE;
            } else if (p->isDown) {
                p->attack = ATTACK_DOWN_KICK;
                *movement = DOWN_KICK;
                p->stamina -= STAMINA_DECREASE;
            } else {
                p->attack = ATTACK_KICK;
                *movement = KICK;
                p->stamina -= STAMINA_DECREASE;
            }
        } else if (al_key_down(key_state, keys[1])) { // LEFT
            joystick_left(p->control);
            *movement = WALK;
        } else if (al_key_down(key_state, keys[2])) { // RIGHT
            joystick_right(p->control);
            *movement = WALK;
        } else {
            *movement = IDLE;
        }
        
        if (p->sprite == KEN && detect_hadouken(key_state, p, keys)) {
            *movement = SPECIAL;
            p->attack = ATTACK_HADOUKEN;
        }

        if (p->sprite == BISON && detect_psycho_crusher(key_state, p, keys)) {
            *movement = SPECIAL;
            p->attack = ATTACK_PSYCHO_CRUSHER;
        }

        if (p->stamina < 0) p->stamina = 0;
    } else {
        if (p->isDown || p->isBeingHit == 2) {
            p->isBeingHit = 2;
            *movement = DAMAGED_DOWN;
        } else if (p->isJumping || p->isBeingHit == 3) {
            p->isBeingHit = 3;
            *movement = DAMAGED_JMP;
        } else if (p->isBeingHit == 1) *movement = DAMAGED;
        p->speed_x = p->direction ? 1 : -1;
    }
}

void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, player* player_1, player* player_2, int *movement1, int *movement2) {
    // Definições das teclas dos jogadores
    const int player1_keys[] = {ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_S, ALLEGRO_KEY_R, ALLEGRO_KEY_F};
    const int player2_keys[] = {ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_P, ALLEGRO_KEY_L};

    // Manipulação de input dos jogadores
    if (player_1->attack == 0) handle_player_input(key_state, player_1, player1_keys, movement1);
    if (player_1->attack == 0) handle_player_input(key_state, player_2, player2_keys, movement2);
}
