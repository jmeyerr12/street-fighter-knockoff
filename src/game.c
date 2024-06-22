#include "game.h"

void update_position(player *player_1, player *player_2, float time) {
    if (player_1->control->up_right) {
        try_move_player(player_1, player_2, 2, 5);
    }
    if (player_1->control->up_left) {
        try_move_player(player_1, player_2, 2, 4);
    }

    if (player_1->control->left) {
        try_move_player(player_1, player_2, 1, 0);
    }
    if (player_1->control->right) {
        try_move_player(player_1, player_2, 1, 1);
    }
    if (player_1->control->up) {
        try_move_player(player_1, player_2, 1, 2);
    }
    if (player_1->control->down) {
        try_move_player(player_1, player_2, 1, 3);
    }

    if (player_2->control->up_right) {
        try_move_player(player_2, player_1, 2, 5);
    }
    if (player_2->control->up_left) {
        try_move_player(player_2, player_1, 2, 4);
    }

    if (player_2->control->left) {
        try_move_player(player_2, player_1, 1, 0);
    }
    if (player_2->control->right) {
        try_move_player(player_2, player_1, 1, 1);
    }
    if (player_2->control->up) {
        try_move_player(player_2, player_1, 1, 2);
    }
    if (player_2->control->down) {
        try_move_player(player_2, player_1, 1, 3);
    }

    if (collision_players(player_1, player_2)) {
        resolve_collision(player_1, player_2);
    }

    out_of_bounds(player_1);
    out_of_bounds(player_2);

    updatePlayer(player_1, time, Y_SCREEN-SPRITE_HEIGHT, X_SCREEN);
    updatePlayer(player_2, time, Y_SCREEN-SPRITE_HEIGHT, X_SCREEN);
}

int countFrames(int movement) {
    switch (movement)
    {
        case IDLE:
            return 5;
            break;
        case WALK:
            return 5;
            break;
        case JUMP:
            return 8;
        case KICK:
            return 4;
        case PUNCH:
            return 2;
        case JUMP_FWD:
            return 8;
            break;
        case JUMP_BCK:
            return 8;
            break;
        case GET_DOWN:
            return 2;
            break;
        case JUMPING_KICK:
            return 8;
            break; 
        case DOWN_PUNCH:
            return 3;
            break; 
        case DOWN_KICK:
            return 3;
            break; 
        case JUMPING_PUNCH:
            return 8;
            break; 
        case DAMAGED:
            return 3;
            break;
        case DAMAGED_JMP:
            return 7;
            break;
        case DAMAGED_DOWN:
            return 3;
            break;
        case DEFENDING:
            return 2;
            break;
        case DEFENDING_DOWN:
            return 2;
            break;
        default:
            return -100;
            break;
    }
}

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

void updateDimensions(player *p, size** charSizes, int movement) {
    p->height = charSizes[p->sprite][movement].height;
}

void handle_down(player *p, int mv, int *frame, int maxFrames, int timer_count) {
    if (mv == GET_DOWN) {
        if (((*frame) < maxFrames) && timer_count % 10 == 0) (*frame)++;  
        if (p->attack != 0 && (*frame) >= maxFrames) p->attack = 0;
    } else {     
        if (timer_count % 10 == 0) (*frame)++;  
        if (p->attack != 0 && (*frame) >= maxFrames) p->attack = 0; //printf("ABAXAXI"); fflush(stdout);}
        if ((*frame) > maxFrames) (*frame) = 0;
        p->isDown = 0;
    }
}

void being_hit(player *p, int mv, int *frame, int maxFrames) {
    if (*frame == maxFrames && (mv == DAMAGED || mv == DAMAGED_DOWN || mv == DAMAGED_JMP)) p->isBeingHit = 0;
}

void defense(player *p, int mv, int *frame, int maxFrames) {
    if (*frame == maxFrames && (mv == DEFENDING || mv == DEFENDING_DOWN)) {p->isDefending = 0;}
}

void handle_jump(player *p, player *opponent, int *movement) {
    if (p->attack == 0 && p->isBeingHit == 0) {
        if (p->isJumping == 1) (*movement) = JUMP;
        else if (p->isJumping == 2 && (p->x > opponent->x)) (*movement) = JUMP_FWD;
        else if (p->isJumping == 3 && (p->x > opponent->x)) (*movement) = JUMP_BCK;
        else if (p->isJumping == 2) (*movement) = JUMP_BCK;
        else if (p->isJumping == 3) (*movement) = JUMP_FWD;
    } else {
        if (p->attack == ATTACK_JUMPING_KICK) (*movement) = JUMPING_KICK;
        else if (p->attack == ATTACK_JUMPING_PUNCH) (*movement) = JUMPING_PUNCH;
    }
}

void check_and_apply_damage(player *p, player *opponent, int damage, int *alreadyDamaged, int attackType) {
    if (isInRange(p, opponent, attackType) && !(*alreadyDamaged)) {
        *alreadyDamaged = 1;
        if ((opponent->direction == LEFT && !(opponent->control->left || opponent->control->down_left)) ||
            (opponent->direction == RIGHT && !(opponent->control->right || opponent->control->down_right))) {
            opponent->health -= damage;
            opponent->isBeingHit = 1;
        } else {
            if (opponent->direction == LEFT) {
                opponent->isDefending = opponent->control->left ? 1 : 2; // 1 para defendendo em pé, 2 para agachado
            } else {
                opponent->isDefending = opponent->control->right ? 1 : 2; 
            }
        }
    }
}

void handle_attack(player *p, player *opponent, int *movement, int *alreadyDamaged) {
    if (p->stamina - STAMINA_DECREASE >= 0) {
        switch (p->attack) {
            case ATTACK_KICK:
                *movement = KICK;
                check_and_apply_damage(p, opponent, 30, alreadyDamaged, KICK);
                break;
            case ATTACK_PUNCH:
                *movement = PUNCH;
                check_and_apply_damage(p, opponent, 20, alreadyDamaged, PUNCH);
                break;
            case ATTACK_JUMPING_KICK:
                *movement = JUMPING_KICK;
                check_and_apply_damage(p, opponent, 35, alreadyDamaged, JUMPING_KICK);
                break;
            case ATTACK_DOWN_PUNCH:
                *movement = DOWN_PUNCH;
                check_and_apply_damage(p, opponent, 25, alreadyDamaged, DOWN_PUNCH);
                break;
            case ATTACK_DOWN_KICK:
                *movement = DOWN_KICK;
                check_and_apply_damage(p, opponent, 30, alreadyDamaged, DOWN_KICK);
                break;
            case ATTACK_JUMPING_PUNCH:
                *movement = JUMPING_PUNCH;
                check_and_apply_damage(p, opponent, 25, alreadyDamaged, JUMPING_PUNCH);
                break;
            default:
                *alreadyDamaged = 0;
                break;
        }
    }
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