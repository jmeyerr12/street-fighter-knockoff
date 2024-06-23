#include "game.h"

int verificaFim(player *p1, player *p2, int countdown) {
    if (p1->health <= 0) return 2;
    if (p2->health <= 0) return 1;
    if (countdown <= 0) return p1->health == p2->health ? 0 : (p1->health > p2->health ? 1 : 2); 
    return 3;
}

player_state determine_state(player* p) {
    if (p->health <= 250 || p->stamina-STAMINA_DECREASE <= 20) {
        return STATE_DEFENSIVE; //saúde baixa, modo defensivo
    } else if (p->health >= 750 || p->stamina-STAMINA_DECREASE <= 50) {
        return STATE_AGGRESSIVE; //saúde alta, modo agressivo
    } else {
        return STATE_NORMAL; //saúde intermediária, estado normal
    }
}

int choose_attack(player* p, int* movement) {
    int attack_choice = rand() % 6; // Aleatoriamente escolhe entre os ataques
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
            p->isDown=1;
            joystick_down(p->control);
            *movement = DOWN_KICK;
            p->attack = ATTACK_DOWN_KICK;
            break;
        case 3:
            p->isDown=1;
            joystick_down(p->control);
            *movement = DOWN_PUNCH;
            p->attack = ATTACK_DOWN_PUNCH;
            break;
        case 4:
            joystick_up(p->control);
            *movement = JUMPING_PUNCH;
            p->attack = ATTACK_JUMPING_PUNCH;
            break;
        case 5:
            joystick_up(p->control);
            *movement = JUMPING_KICK;
            p->attack = ATTACK_JUMPING_KICK;
            break;
        default:
            break;
    }
    return *movement;
}

// Função para lidar com a entrada do bot
void handle_bot_input(player* p1, player* p2, int* movement, int timer_count) {
    if (p2->isBeingHit == 1) {
        *movement = DAMAGED;
        p2->speed_x = p2->direction ? 1 : -1;
        return;
    } else if (p2->isBeingHit == 2) {
        *movement = DAMAGED_DOWN;
        p2->speed_x = p2->direction ? 1 : -1;
        return;
    } else if (p2->isBeingHit == 3) {
        *movement = DAMAGED_JMP;
        p2->speed_x = p2->direction ? 1 : -1;
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
            int action_choice = rand() % 6; // Prioriza mais a defesa
            if (action_choice == 0) {
                *movement = GET_DOWN;
                p2->isDown = 1;
            } else if (action_choice == 1 || action_choice == 2 || action_choice == 3) {
                p2->isDefending = (p2->direction == LEFT) ? ((p2->control->left) ? 1 : 2) : ((p2->control->right) ? 1 : 2);
                *movement = (p2->isDefending == 1) ? DEFENDING : DEFENDING_DOWN;
            } else if (action_choice == 4) {
                if (p2->direction == LEFT) joystick_up_right(p2->control);
                else joystick_up_left(p2->control);
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
        } else if (abs(distance) <= 60 && p2->attack == 0 && p2->stamina >= STAMINA_DECREASE && timer_count % 10 == 0) {
            p2->stamina -= STAMINA_DECREASE;
            choose_attack(p2, movement);
        }

    } else {
        // Estado normal, comportamento padrão
        if (abs(distance) > 50) {
            if (distance > 10) {
                joystick_right(p2->control);
            } else {
                joystick_left(p2->control);
            }
            *movement = WALK;
        } else {
            *movement = IDLE;
            resetPlayer(p2);
        }

        if (abs(distance) <= 30 && p2->attack == 0 && p2->stamina >= STAMINA_DECREASE && timer_count % 15 == 0) {
            p2->stamina -= STAMINA_DECREASE;
            choose_attack(p2, movement);
        }
    }
}
