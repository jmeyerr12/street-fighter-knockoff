#include "game.h"

int verificaFim(player *p1, player *p2, int countdown) {
    if (p1->health <= 0) return 2;
    if (p2->health <= 0) return 1;
    if (countdown <= 0) return p1->health == p2->health ? 0 : (p1->health > p2->health ? 1 : 2); 
    return 3;
}

void showWinner(ALLEGRO_FONT *font, int winner) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (winner == 0) {
        al_draw_text(font, al_map_rgb(255, 255, 0),
                X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "DRAW");
    } else {
        char text[100];
        sprintf(text, "PLAYER %d WINS THE ROUND", winner);
        al_draw_text(font, al_map_rgb(255, 255, 0),
                X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, text);
    }
    al_flip_display();
    al_rest(1);
}

player_state determine_state(player* p) {
    if (p->health <= 250 || p->stamina-STAMINA_DECREASE <= 10) {
        return STATE_DEFENSIVE; //saúde baixa, modo defensivo
    } else if (p->health >= 750) {
        return STATE_AGGRESSIVE; //saúde alta, modo agressivo
    } else {
        return STATE_NORMAL; //saúde intermediária, estado normal
    }
}

int choose_attack(player* p, int* movement) {
    int attack_choice = rand() % 4; // Aleatoriamente escolhe entre os ataques
    switch (attack_choice) {
        case 0:
            *movement = PUNCH;
            p->attack = ATTACK_PUNCH;
            break;
        case 1:
            *movement = KICK;
            p->attack = ATTACK_KICK;
            break;
        case 2:
            *movement = DOWN_KICK;
            p->attack = ATTACK_DOWN_KICK;
            break;
        case 3:
            *movement = DOWN_PUNCH;
            p->attack = ATTACK_DOWN_PUNCH;
            break;
        default:
            break;
    }
    return *movement;
}

// Função para lidar com a entrada do bot
void handle_bot_input(player* p1, player* p2, int* movement) {
    if (p2->isBeingHit == 1) {
        *movement = DAMAGED;
        return;
    }

    int distance;
    if (p2->direction == RIGHT) {
        distance = p1->SE.x - p2->SW.x;
    } else {
        distance = p1->SW.x - p2->SE.x;
    }

    player_state state = determine_state(p2); // Determina o estado do bot

    if (state == STATE_DEFENSIVE) {
        if (distance > 15) {
            joystick_left(p2->control);
            *movement = WALK;
        } else {
            joystick_right(p2->control);
            *movement = WALK;
        }

        if (abs(distance) < 15 && p2->attack == 0) {
            int action_choice = rand() % 4; 
            if (action_choice == 0) {
                *movement = GET_DOWN;
                p2->isDown = 1;
            } else if (action_choice == 1 || action_choice == 2) { // Prioriza mais a defesa
                p2->isDefending = (p2->direction == LEFT) ? ((p2->control->left) ? 1 : 2) : ((p2->control->right) ? 1 : 2);
                *movement = (p2->isDefending == 1) ? DEFENDING : DEFENDING_DOWN;
            } else if (p2->stamina >= STAMINA_DECREASE) { // Raramente ataca
                p2->stamina -= STAMINA_DECREASE;
                choose_attack(p2, movement);
            }
        }

    } else if (state == STATE_AGGRESSIVE) {
        // Modo agressivo, tenta se aproximar e atacar o jogador 1
        if (abs(distance) > 20 && p2->attack == 0) {
            if (distance > 15) {
                joystick_right(p2->control);
            } else {
                joystick_left(p2->control);
            }
            *movement = WALK;
        } else if (abs(distance) <= 60 && p2->attack == 0 && p2->stamina >= STAMINA_DECREASE) {
            choose_attack(p2, movement);
            p2->stamina -= STAMINA_DECREASE;
        }

    } else {
        // Estado normal, comportamento padrão
        if (abs(distance) > 100) {
            if (distance > 15) {
                joystick_right(p2->control);
            } else {
                joystick_left(p2->control);
            }
            *movement = WALK;
        } else {
            *movement = IDLE;
            resetPlayer(p2);
        }

        if (abs(distance) <= 60 && p2->attack == 0 && p2->stamina >= STAMINA_DECREASE) {
            choose_attack(p2, movement);
            p2->stamina -= STAMINA_DECREASE;
        }
    }
}
