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
    if (p->health <= 250) {
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
    int distance;
    if (p2->direction == RIGHT) {
        distance = p1->SE.x - p2->SW.x;
    } else {
        distance = p1->SW.x - p2->SE.x;
    }

    player_state state = determine_state(p2); // Determina o estado do bot

    if (state == STATE_DEFENSIVE) {
        // Modo defensivo, tenta se afastar do jogador 1 e se agachar
        if (distance > 0) {
            *movement = LEFT;
            joystick_left(p2->control);
        } else {
            *movement = RIGHT;
            joystick_right(p2->control);
        }

        if (abs(distance) < 20 && p2->attack == 0) {
            int action_choice = rand() % 2; // Aleatoriamente escolhe entre se agachar e atacar
            if (action_choice == 0) {
                *movement = GET_DOWN;
                p2->isDown = 1;
            } else {
                choose_attack(p2, movement);
            }
        }

    } else if (state == STATE_AGGRESSIVE) {
        // Modo agressivo, tenta se aproximar e atacar o jogador 1
        if (abs(distance) > 20 && p2->attack == 0) {
            if (distance > 0) {
                *movement = RIGHT;
                joystick_right(p2->control);
            } else {
                *movement = LEFT;
                joystick_left(p2->control);
            }
        } else if (abs(distance) <= 60 && p2->attack == 0) {
            // Atacar se estiver perto o suficiente
            choose_attack(p2, movement);
        }

    } else {
        // Estado normal, comportamento padrão
        if (abs(distance) > 100) {
            if (distance > 0) {
                *movement = RIGHT;
                joystick_right(p2->control);
            } else {
                *movement = LEFT;
                joystick_left(p2->control);
            }
        } else {
            *movement = IDLE;
            resetPlayer(p2);
        }

        if (abs(distance) <= 60 && p2->attack == 0) {
            choose_attack(p2, movement);
        }
    }
}