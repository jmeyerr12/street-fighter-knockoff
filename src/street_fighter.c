#include "street_fighter.h"

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

                //printf("isDefending1: %d\n", player_1->isDefending);
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
                else if (movement2 == DEFENDING_DOWN) movement2 = GET_DOWN; 

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