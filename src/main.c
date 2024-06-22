//Compilação: gcc main.c character.c joystick.c street_fighter.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include "street_fighter.h"		

void check_v_key_and_print_position(ALLEGRO_EVENT *ev, player* p1, player *p2) {
    if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev->keyboard.keycode == ALLEGRO_KEY_V) { // Tecla 'V'
            int x, y;
            al_get_mouse_cursor_position(&x, &y);
            char position[50];
            snprintf(position, sizeof(position), "Cursor position: (%d, %d)", x, y);
            printf("\n\n%s\n", position);
            printPlayerStatistics(p1, 2);
            printPlayerStatistics(p2, 2);
            fflush(stdout);
        }
    }
}

unsigned char collision_players(player *p1, player *p2) {
    setHitbox(p1);
    setHitbox(p2);

    int overlap_x = (p1->SE.x > p2->SW.x) && (p1->SW.x < p2->SE.x);
    int overlap_y = (p1->SE.y > p2->NW.y) && (p1->NW.y < p2->SE.y);

    return overlap_x && overlap_y;
}

unsigned char collision_wall(player *p) {
    return (p->x <= 6) || (p->SE.x >= X_SCREEN - 6);
}

unsigned char collision_2D(player *p1, player *p2) {
    return collision_players(p1, p2) || collision_wall(p1) || collision_wall(p2);
}

void try_move_player(player *p, player *other, int multiplier, int direction) {
    // Armazena a posição anterior antes de tentar mover
    int original_x = p->x;

    // Atualiza a posição do jogador
    movePlayer(p, multiplier, direction);

    // Se a nova posição resultar em uma colisão, reverta para a posição original
    if (collision_players(p, other)) {
        p->x = original_x;
    }
}

void resolve_collision(player *p1, player *p2) {
    if (p1->direction == LEFT) {
        p1->x -= 2;
        p2->x += 2;
    } else {
        p1->x += 2;
        p2->x -= 2;
    }
}

void out_of_bounds(player *p) {
    if (p->x < 6) {
        p->x = 6;  // Define a borda esquerda como limite mínimo
    } else if (p->x > X_SCREEN - 6 - p->width) {
        p->x = X_SCREEN - 6 - p->width;  // Define a borda direita menos a largura do sprite como limite máximo
    }
}

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

/* void draw_player(ALLEGRO_BITMAP* p, player* player, int current, int movement, int inverted) {
    al_draw_bitmap_region(p, 112*current, 112*movement, 112, 112, player->x, player->y, inverted? ALLEGRO_FLIP_HORIZONTAL : 0);
} */

void draw_player(ALLEGRO_BITMAP* p, player* player, int current, int movement, int inverted) {
  al_draw_bitmap_region(p, 
                          112*current,                        // Posição x inicial no sprite sheet
                          112*movement,                       // Posição y inicial no sprite sheet
                          112,                        // Largura do sprite
                          112,                       // Altura do sprite
                          inverted ? player->x + player->width - 112 : player->x,  // Posição x na tela, ajustada para inversão
                          player->y,                         // Posição y na tela
                          inverted ? ALLEGRO_FLIP_HORIZONTAL : 0);  // Flag para inverter o sprite
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
                p->stamina-=STAMINA_DECREASE;      
            } else if (al_key_down(key_state, keys[5]) && p->attack == 0) { // KICK
                *movement = DOWN_KICK;
                p->attack = ATTACK_DOWN_KICK;
                p->stamina-=STAMINA_DECREASE;
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
                p->stamina-=STAMINA_DECREASE;
            } else {
                p->attack = ATTACK_PUNCH;
                *movement = PUNCH;
                p->stamina-=STAMINA_DECREASE;
            }
        } else if (al_key_down(key_state, keys[5]) && p->attack == 0) { // KICK
            if (p->isJumping) {
                p->attack = ATTACK_JUMPING_KICK;
                *movement = JUMPING_KICK;
                p->stamina-=STAMINA_DECREASE;
            } else if (p->isDown) {
                p->attack = ATTACK_JUMPING_PUNCH;
                *movement = JUMPING_PUNCH;
                p->stamina-=STAMINA_DECREASE;
            } else {
                p->attack = ATTACK_KICK;
                *movement = KICK;
                p->stamina-=STAMINA_DECREASE;
            }
        } else if (al_key_down(key_state, keys[1])) { // LEFT
            joystick_left(p->control);
            *movement = WALK;
        } else if (al_key_down(key_state, keys[2])) { // RIGHT
            joystick_right(p->control);
            *movement = WALK;
        } else *movement = IDLE;
        if (p->stamina < 0) p->stamina = 0;
    } else {
        if (p->isDown || p->isBeingHit == 2) {
            p->isBeingHit = 2;
            *movement = DAMAGED_DOWN;
        } else if (p->isJumping || p->isBeingHit == 3) {
            p->isBeingHit = 3;
            *movement = DAMAGED_JMP;
        } else if (p->isBeingHit == 1) *movement = DAMAGED; //arrumar esse trecho, jump e down nao funcionam
        p->speed_x = p->direction ? 1 : -1;
    }
} 

void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, player* player_1, player* player_2, int *movement1, int *movement2) {
    al_get_keyboard_state(key_state);

    //definições das teclas dos jogadores
    const int player1_keys[] = {ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_S, ALLEGRO_KEY_R, ALLEGRO_KEY_F};
    const int player2_keys[] = {ALLEGRO_KEY_UP, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_P, ALLEGRO_KEY_L};

    //manipulação de input dos jogadores
    handle_player_input(key_state, player_1, player1_keys, movement1);
    handle_player_input(key_state, player_2, player2_keys, movement2);
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


//retorna 0 em caso de empate, 1 em caso de vitoria do player 1 e 2 em caso de vitoria do player 2
int run_round(ALLEGRO_EVENT_QUEUE* queue, player* player_1, player* player_2, int* state, char* filename, 
              ALLEGRO_FONT* font, ALLEGRO_BITMAP* player1_sheet, ALLEGRO_BITMAP* player2_sheet, int round, int p1Wins, int p2Wins) {

    size** charSizes = characterSizes();
    setDimensions(player_1, charSizes[player_1->sprite][IDLE].width, charSizes[player_1->sprite][IDLE].height);
    setDimensions(player_2, charSizes[player_2->sprite][IDLE].width, charSizes[player_2->sprite][IDLE].height);
    
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    double lastTime = al_get_time();
    int running = 1;
    int frame1 = 0, frame2 = 0;
    int timer_count = 0;
    int maxFrame1 = 5, maxFrame2 = 5;
    int movement1 = 0, previous_movement1 = 0;
    int movement2 = 0, previous_movement2 = 0;
    int alreadyDamaged1 = 0, alreadyDamaged2 = 0;
    background bg;
    int countdown = 99;
    int isFim = 3; 
    int pause_option = GAME;
    init_animated_background(&bg, 24.0, filename);  

    update_animated_background(&bg);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_animated_background(&bg);
    if (player_1->x > player_2->x) {
        draw_player(player1_sheet, player_1, frame1, movement1, 1);
        draw_player(player2_sheet, player_2, frame2, movement2, 0);
    } else {
        draw_player(player1_sheet, player_1, frame1, movement1, 0);
        draw_player(player2_sheet, player_2, frame2, movement2, 1);
    }
    draw_scoreboard(player_1->health, player_2->health, X_SCREEN, font, countdown, round, p1Wins, p2Wins, player_1->stamina, player_2->stamina);   

    char roundText[100];
    if (round == 1) strcpy(roundText, "1ST ROUND");
    else if (round == 2) strcpy(roundText, "2ND ROUND");
    else strcpy(roundText, "3RD ROUND");

    al_draw_filled_rectangle(X_SCREEN / 2 - 40, Y_SCREEN / 2 - 4, X_SCREEN / 2 + 40, Y_SCREEN / 2 + 10, al_map_rgb(0, 0, 0));

    al_draw_text(font, al_map_rgb(255, 255, 0),
                 X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, roundText);

    al_flip_display();
    al_rest(1.5); 

    int pausedTime = 0;
    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        ALLEGRO_KEYBOARD_STATE key_state;
        al_get_keyboard_state(&key_state);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = !running;
            if (running) {
                lastTime = al_get_time() - pausedTime; //adjust the timer when resuming
            } else {
                pausedTime = al_get_time() - lastTime; //record the time when pausing
            }
        }

        if (running) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                *state = EXIT;
                break;
            }

            isFim = verificaFim(player_1, player_2, countdown);
            if (isFim != 3) break;

            if (event.type == ALLEGRO_EVENT_TIMER) {
                timer_count++;

                double currentTime = al_get_time();
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                update_animated_background(&bg);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_animated_background(&bg);

                handle_input(&key_state, player_1, player_2, &movement1, &movement2);
                
                handle_down(player_1, movement1, &frame1, maxFrame1, timer_count);
                handle_jump(player_1, player_2, &movement1);
                handle_attack(player_1, player_2, &movement1, &alreadyDamaged1);
                
                if (player_1->isDefending == 1) movement1 = DEFENDING;
                else if (player_1->isDefending == 2) movement1 = DEFENDING_DOWN;
                else if (movement1 == DEFENDING_DOWN) movement1 = GET_DOWN; 

                if (movement1 != previous_movement1) frame1 = 0;
                maxFrame1 = countFrames(movement1);
                previous_movement1 = movement1;

                defense(player_1, movement1, &frame1, maxFrame1);
                being_hit(player_1, movement1, &frame1, maxFrame1);

                handle_down(player_2, movement2, &frame2, maxFrame2, timer_count);
                handle_jump(player_2, player_1, &movement2);
                handle_attack(player_2, player_1, &movement2, &alreadyDamaged2);

                if (player_2->isDefending == 1) movement2 = DEFENDING;
                else if (player_2->isDefending == 2) movement2 = DEFENDING_DOWN;
                else if (movement1 == DEFENDING_DOWN) movement1 = GET_DOWN; 

                if (movement2 != previous_movement2) frame2 = 0;
                maxFrame2 = countFrames(movement2);
                previous_movement2 = movement2;
                defense(player_2, movement2, &frame2, maxFrame2);
                being_hit(player_2, movement2, &frame2, maxFrame2);

                if (timer_count % 4 == 0) {
                    recharge_stamina(player_1);
                    recharge_stamina(player_2);
                }
                
                update_position(player_1, player_2, deltaTime);

                if ((player_1->SE.x + player_1->SW.x)/2 > (player_2->SE.x + player_2->SW.x)/2) {
                    player_1->direction = RIGHT;
                    player_2->direction = LEFT;
                    draw_player(player1_sheet, player_1, frame1, movement1, 1);
                    draw_player(player2_sheet, player_2, frame2, movement2, 0);
                } else {
                    player_1->direction = LEFT;
                    player_2->direction = RIGHT;
                    draw_player(player1_sheet, player_1, frame1, movement1, 0);
                    draw_player(player2_sheet, player_2, frame2, movement2, 1);
                }

                draw_scoreboard(player_1->health, player_2->health, X_SCREEN, font, countdown, round, p1Wins, p2Wins, player_1->stamina, player_2->stamina);
                if (timer_count % 30 == 0) countdown--;

                resetPlayer(player_1);
                resetPlayer(player_2);

                updateDimensions(player_1, charSizes, movement1);
                updateDimensions(player_2, charSizes, movement2);
                
                al_flip_display();
            }
        } else {
            pause_option = draw_pause(font, queue);
            running = 1;
            lastTime = al_get_time() - pausedTime; // Reset the timer when resuming
        }

        if (pause_option == MENU) break;


        check_v_key_and_print_position(&event, player_1, player_2);
        /* if (timer_count % 120 == 0) { 
            printf("\n\n -- %d -- ", timer_count / 30);
            printf("\n---------- %f", al_get_time());
            printPlayerStatistics(player_1, 1);
            printPlayerStatistics(player_2, 2);
        }  */
    }

    al_destroy_timer(timer);
    destroy_animated_background(&bg);
    freeCharacterSizes(charSizes);
    return isFim;

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

// Retorna 0 em caso de empate, 1 em caso de vitória do player 1 e 2 em caso de vitória do player 2
int run_single_player(ALLEGRO_EVENT_QUEUE* queue, player* player_1, player* player_2, int* state, char* filename, 
    ALLEGRO_FONT *font, ALLEGRO_BITMAP* player1_sheet, ALLEGRO_BITMAP* player2_sheet, int round, int p1Wins, int p2Wins) {

    size** charSizes = characterSizes();
    setDimensions(player_1, charSizes[player_1->sprite][IDLE].width, charSizes[player_1->sprite][IDLE].height);
    setDimensions(player_2, charSizes[player_2->sprite][IDLE].width, charSizes[player_2->sprite][IDLE].height);
    
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    double lastTime = al_get_time();
    double pausedTime = 0;
    int running = 1;
    int frame1 = 0, frame2 = 0;
    int timer_count = 0;
    int maxFrame1 = 5, maxFrame2 = 5;
    int movement1 = 0, previous_movement1 = 0;
    int movement2 = 0, previous_movement2 = 0;
    int alreadyDamaged1 = 0, alreadyDamaged2 = 0;
    background bg;
    int countdown = 99;
    int isFim = 3; 
    init_animated_background(&bg, 24.0, filename);  

    update_animated_background(&bg);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_animated_background(&bg);
    if (player_1->x > player_2->x){
        draw_player(player1_sheet, player_1, frame1, movement1, 1);
        draw_player(player2_sheet, player_2, frame2, movement2, 0);
    } else {
        draw_player(player1_sheet, player_1, frame1, movement1, 0);
        draw_player(player2_sheet, player_2, frame2, movement2, 1);
    }
    setHitbox(player_1);
    setHitbox(player_2);
    draw_scoreboard(player_1->health,player_2->health,X_SCREEN,font,countdown,round,p1Wins,p2Wins, player_1->stamina, player_2->stamina);   

    char roundText[100];
    if (round == 1) strcpy(roundText, "1ST ROUND");
    else if (round == 2) strcpy(roundText, "2ND ROUND");
    else strcpy(roundText, "3RD ROUND");

    al_draw_filled_rectangle(X_SCREEN/2 - 40, Y_SCREEN/2 - 4, X_SCREEN/2 + 40, Y_SCREEN/2 + 10, al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 0), X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, roundText);
    al_flip_display();
    al_rest(1.5); 

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        ALLEGRO_KEYBOARD_STATE key_state;
        al_get_keyboard_state(&key_state);
        
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = !running;
            if (running) {
                lastTime = al_get_time() - pausedTime; // reset the timer when resuming
            } else {
                pausedTime = al_get_time() - lastTime; // record the time when pausing
                int pause_option = draw_pause(font, queue);
                if (pause_option == MENU) break;
                running = 1;
                lastTime = al_get_time() - pausedTime; // reset the timer when resuming
            }
        }

        if (running) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                *state = EXIT;
                break;
            }

            isFim = verificaFim(player_1, player_2, countdown);
            if (isFim != 3) break;

            if (event.type == ALLEGRO_EVENT_TIMER) {
                timer_count++;

                double currentTime = al_get_time();
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                update_animated_background(&bg);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_animated_background(&bg);

                const int player1_keys[] = {ALLEGRO_KEY_W, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_S, ALLEGRO_KEY_R, ALLEGRO_KEY_F};
                handle_player_input(&key_state, player_1, player1_keys, &movement1);

                handle_bot_input(player_1, player_2, &movement2);
                
                handle_down(player_1, movement1, &frame1, maxFrame1, timer_count);
                handle_jump(player_1, player_2, &movement1);
                handle_attack(player_1, player_2, &movement1, &alreadyDamaged1);
                if (movement1 != previous_movement1) frame1 = 0;
                maxFrame1 = countFrames(movement1);
                previous_movement1 = movement1;
                being_hit(player_1, movement1, &frame1, maxFrame1);

                handle_down(player_2, movement2, &frame2, maxFrame2, timer_count);
                handle_jump(player_2, player_1, &movement2);
                handle_attack(player_2, player_1, &movement2, &alreadyDamaged2);
                if (movement2 != previous_movement2) frame2 = 0;
                maxFrame2 = countFrames(movement2);
                previous_movement2 = movement2;
                being_hit(player_1, movement1, &frame1, maxFrame1);

                if (timer_count % 4 == 0) {
                    recharge_stamina(player_1);
                    recharge_stamina(player_2);
                }

                update_position(player_1, player_2, deltaTime);

                if (player_1->SE.x > player_2->SW.x) {
                    player_1->direction = RIGHT;
                    player_2->direction = LEFT;
                    draw_player(player1_sheet, player_1, frame1, movement1, 1);
                    draw_player(player2_sheet, player_2, frame2, movement2, 0);
                } else {
                    player_1->direction = LEFT;
                    player_2->direction = RIGHT;
                    draw_player(player1_sheet, player_1, frame1, movement1, 0);
                    draw_player(player2_sheet, player_2, frame2, movement2, 1);
                }

                draw_scoreboard(player_1->health, player_2->health, X_SCREEN, font, countdown, round, p1Wins, p2Wins, player_1->stamina, player_2->stamina);
                if (timer_count % 30 == 0) countdown--;

                resetPlayer(player_1);
                resetPlayer(player_2);

                updateDimensions(player_1, charSizes, movement1);
                updateDimensions(player_2, charSizes, movement2);
                
                al_flip_display();
            }
        }

        if (timer_count % 120 == 0) { 
            printf("\n\n -- %d -- ", timer_count / 30);
            printf("\n---------- %f", al_get_time());
            printPlayerStatistics(player_1, 1);
            printPlayerStatistics(player_2, 2);
        }
    }

    al_destroy_timer(timer);
    destroy_animated_background(&bg);
    freeCharacterSizes(charSizes);
    return isFim;
}

int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(disp));

    al_register_event_source(queue, al_get_keyboard_event_source());

    player* player_1; 
    player* player_2; 
    
    ALLEGRO_BITMAP* player1_sheet;
    ALLEGRO_BITMAP* player2_sheet;

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Failed to load font.\n");
        return -1;
    }
 
    int state = MENU;
    int selected_option = MENU_START;
    int selected_image = 0;
    char filename[100];
    int p1Wins = 0;
    int p2Wins = 0;
    int roundCounter = 1;
    int sel1 = 0,sel2 = 1;

    srand(time(NULL));
    
    while (state != EXIT) {
        ALLEGRO_EVENT event;
        draw_menu(font, selected_option); 
        al_wait_for_event(queue, &event);

        switch (state) {
            case MENU:            
                draw_menu(font, selected_option); 
               // if (event.type == ALLEGRO_EVENT_KEY_DOWN)
                    state = handle_menu_input(event, &selected_option);
                break;
            case GAME:
                player_1 = buildPlayer(61, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92, LEFT);
                player_2 = buildPlayer(61, X_SCREEN-300, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92, RIGHT);
                show_characters_menu(queue,&player1_sheet,&player2_sheet,&sel1,&sel2);
                player_1->sprite = sel1;
                player_2->sprite = sel2;
                selected_image = show_image_menu(font, queue);
                if (selected_image == 0) strcpy(filename,"destroyed_dojo");
                else if (selected_image == 1) strcpy(filename,"dark_dojo");
                while ((p1Wins < 2) && (p2Wins < 2) && (roundCounter <= 3)) { 
                    int resultado = run_round(queue, player_1, player_2, &state, filename, font, player1_sheet, player2_sheet, roundCounter, p1Wins, p2Wins); 
                    if (resultado == 0) { //em caso de empate o vencedor é sorteado
                        resultado = 1 + rand() % 2;
                    }
                    if (resultado == 1) p1Wins++;
                    else if (resultado == 2) p2Wins++;
                    else break; //clicou para sair
                    showWinner(font, resultado);
                    resetAttributes(&player_1, 61, 92, 10, Y_SCREEN/2);
                    resetAttributes(&player_2, 61, 92, X_SCREEN-122, Y_SCREEN/2);
                    roundCounter++;
                }
                al_destroy_bitmap(player1_sheet);
                al_destroy_bitmap(player2_sheet);
                joystick_destroy(player_1->control);
                joystick_destroy(player_2->control);
                free(player_1);
                free(player_2);
                state = ENDGAME;
                break;
            case SINGLE_PLAYER:
                player_1 = buildPlayer(61, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92, LEFT);
                player_2 = buildPlayer(61, X_SCREEN-300, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92, RIGHT);
                show_characters_menu(queue,&player1_sheet,&player2_sheet,&sel1,&sel2);
                player_1->sprite = sel1;
                player_2->sprite = sel2;
                selected_image = show_image_menu(font, queue);
                if (selected_image == 0) strcpy(filename,"destroyed_dojo");
                else if (selected_image == 1) strcpy(filename,"dark_dojo");
                while ((p1Wins < 2) && (p2Wins < 2) && (roundCounter <= 3)) {
                    int resultado = run_single_player(queue, player_1, player_2, &state, filename, font, player1_sheet, player2_sheet, roundCounter, p1Wins, p2Wins); 
                    if (resultado == 0) resultado = 1 + rand() % 2;  //em caso de empate o vencedor é sorteado
                    if (resultado == 1) p1Wins++;
                    else if (resultado == 2) p2Wins++;
                    else break; //clicou para sair
                    showWinner(font, resultado);
                    resetAttributes(&player_1, 61, 92, 10, Y_SCREEN/2);
                    resetAttributes(&player_2, 61, 92, X_SCREEN-122, Y_SCREEN/2);
                    roundCounter++;
                }
                al_destroy_bitmap(player1_sheet);
                al_destroy_bitmap(player2_sheet);
                joystick_destroy(player_1->control);
                joystick_destroy(player_2->control);
                free(player_1);
                free(player_2);
                state = ENDGAME; 
                break;
            case ENDGAME:
                //mostrar endgame com vencedor e estatisticas
                p1Wins = 0; p2Wins = 0;
                roundCounter = 0;
                state = MENU;
                break;
        }
        
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            state = EXIT;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    
    return 0;
}

// Arrumar tamanhos (matriz)
// Investigar, com tudo 60 da certo, com os valores setados na mão ele da problema no ataque (chun li(tamanhos variados) vs ken (tamanho tudo 60))
// Arrumar overlap_y no ataque, quando se abaixa nao esta funcionando, toma dano de soco que nao atinge