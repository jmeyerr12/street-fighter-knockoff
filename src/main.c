//Compilação: gcc main.c character.c joystick.c street_fighter.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include "street_fighter.h"		

unsigned char collision_2D(player *element_first, player *element_second) {
    //printf("  --  %d  %d  --  ",element_first->x + element_first->width > element_second->x,(element_first->x < element_second->x + element_second->width));
    //printf("  --  %d  %d  --  ",element_first->x, element_second->x);
    //printf("  --  %d  %d  --  ",element_first->x , element_second->x); //+ element_second->width);
    
    //fflush(stdout);

    int overlap_x = (element_first->x+6 > element_second->x) &&  //6 é um threshold
                    (element_first->x < element_second->x+6);

    int overlap_y = (element_first->y > element_second->y - element_second->height) && 
                    (element_first->y - element_first->height < element_second->y);

    return overlap_x && overlap_y;
}

void try_move_player(player* p, player* other, int multiplier, int direction) {
    movePlayer(p, multiplier, direction);
    if (collision_2D(p, other)) {
        movePlayer(p, -multiplier, direction);
    }
}

void update_position(player *player_1, player *player_2, float time) {
    if (player_1->control->up_right) {
        try_move_player(player_1, player_2, 1, 5);
    }
    if (player_1->control->up_left) {
        try_move_player(player_1, player_2, 1, 4);
    }
    // Adicione mais direções aqui conforme necessário
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
        try_move_player(player_2, player_1, 1, 5);
    }
    if (player_2->control->up_left) {
        try_move_player(player_2, player_1, 1, 4);
    }
    // Adicione mais direções aqui conforme necessário
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

    updatePlayer(player_1, time, Y_SCREEN-SPRITE_HEIGHT, X_SCREEN);
    updatePlayer(player_2, time, Y_SCREEN-SPRITE_HEIGHT, X_SCREEN);
}



/*void update_position(player *player_1, player *player_2, float time){																															
    if (player_1->control->up_right) {																																							
		movePlayer(player_1, 1, 5);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 5);																								
	}
	if (player_1->control->up_left) {																																							
		movePlayer(player_1, 1, 4, X_SCREEN, Y_SCREEN);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 4);																								
	}
	if (player_1->control->left) {																																							
		//movePlayer(player_1, 1, 0, X_SCREEN, Y_SCREEN);																																		
		if (!collision_2D(player_1, player_2)) movePlayer(player_1, 1, 0);																								
	}
	if (player_1->control->right) {																																							
		//movePlayer(player_1, 1, 1, X_SCREEN, Y_SCREEN);																																	
		if (!collision_2D(player_1, player_2)) movePlayer(player_1, 1, 1);																							
	}
	if (player_1->control->up) {																															
		movePlayer(player_1, 1, 2);																								
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 2);															
	}
	if (player_1->control->down) {																											
		movePlayer(player_1, 1, 3);																						
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 3);										
	}

    if (player_2->control->up_right) {																																		
		movePlayer(player_2, 1, 5);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_2, -1, 5);																								
	}
	if (player_2->control->up_left) {																																					
		movePlayer(player_2, 1, 4);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_2, -1, 4);																								
	}
	if (!player_2->control->left) {														
		//movePlayer(player_2, 1, 0, X_SCREEN, Y_SCREEN);						
		if (!collision_2D(player_2, player_1)) movePlayer(player_2, 1, 0);		
	}
	
	if (!player_2->control->right) { 						
		//movePlayer(player_2, 1, 1, X_SCREEN, Y_SCREEN);		
		if (!collision_2D(player_2, player_1)) movePlayer(player_2, 1, 1);		
	}
	
	if (player_2->control->up) {									
		movePlayer(player_2, 1, 2);		
		if (collision_2D(player_2, player_1)) movePlayer(player_2, -1, 2);	
	}
	if (player_2->control->down) {		
		movePlayer(player_2, 1, 3);						
		if (collision_2D(player_2, player_1)) movePlayer(player_2, -1, 3);						
	}

    updatePlayer(player_1, time, Y_SCREEN-SPRITE_HEIGHT, X_SCREEN);
    updatePlayer(player_2, time, Y_SCREEN-SPRITE_HEIGHT, X_SCREEN);
}*/

/*void draw_players(ALLEGRO_BITMAP* p1, ALLEGRO_BITMAP* p2, player* player_1, int current, int movement, player* player_2) {
    al_draw_bitmap_region(p1, 112*current, 112*movement, 112, 112, player_1->x, player_1->y, 0);
    al_draw_bitmap_region(p2, 112*current, 112*movement, 112, 112, player_2->x, player_2->y, ALLEGRO_FLIP_HORIZONTAL);
} */

void draw_player(ALLEGRO_BITMAP* p, player* player, int current, int movement, int inverted) {
    al_draw_bitmap_region(p, 112*current, 112*movement, 112, 112, player->x, player->y, inverted? ALLEGRO_FLIP_HORIZONTAL : 0);
}

void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, player* player_1, player* player_2, int *movement1, int *movement2) {
    al_get_keyboard_state(key_state);

    // Player 1 controls
    if (al_key_down(key_state, ALLEGRO_KEY_W)) {
        if (al_key_down(key_state, ALLEGRO_KEY_D)) {
            joystick_up_right(player_1->control);
            *movement1 = JUMP_BCK;
        } else if (al_key_down(key_state, ALLEGRO_KEY_A)) {
            joystick_up_left(player_1->control);
            *movement1 = JUMP_FWD;
        } else {
            joystick_up(player_1->control);
            *movement1 = JUMP;
        }
    } else if (al_key_down(key_state, DOWN_1)) {
        *movement1 = GET_DOWN;
    } else if (al_key_down(key_state, ALLEGRO_KEY_A)) {
        joystick_left(player_1->control);
        *movement1 = WALK;
    } else if (al_key_down(key_state, ALLEGRO_KEY_D)) {
        joystick_right(player_1->control);
        *movement1 = WALK;
    } else if (al_key_down(key_state, ALLEGRO_KEY_R)) {
        player_1->attack = ATTACK_PUNCH;
        *movement1 = PUNCH;
    } else if (al_key_down(key_state, ALLEGRO_KEY_F)) {
        player_1->attack = ATTACK_KICK;
        *movement1 = KICK;
    } else {
        *movement1 = IDLE;
    }

    // Player 2 controls
    if (al_key_down(key_state, ALLEGRO_KEY_UP)) {
        if (al_key_down(key_state, ALLEGRO_KEY_RIGHT)) {
            joystick_up_right(player_2->control);
            *movement2 = JUMP_FWD;
        } else if (al_key_down(key_state, ALLEGRO_KEY_LEFT)) {
            joystick_up_left(player_2->control);
            *movement2 = JUMP_BCK;
        } else {
            joystick_up(player_2->control);
            *movement2 = JUMP;
        }
    } else if (al_key_down(key_state, DOWN_2)) {
        *movement2 = GET_DOWN;
    } else if (al_key_down(key_state, ALLEGRO_KEY_LEFT)) {
        joystick_left(player_2->control);
        *movement2 = WALK;
    } else if (al_key_down(key_state, ALLEGRO_KEY_RIGHT)) {
        joystick_right(player_2->control);
        *movement2 = WALK;
    } else if (al_key_down(key_state, ALLEGRO_KEY_P)) {
        player_2->attack = ATTACK_PUNCH;
        *movement2 = PUNCH;
    } else if (al_key_down(key_state, ALLEGRO_KEY_L)) {
        player_2->attack = ATTACK_KICK;
        *movement2 = KICK;
    } else {
        *movement2 = IDLE;																												
    }
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
        default:
            return -100;
            break;
    }
}

void run_game(ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT_QUEUE* queue, player* player_1, player* player_2, int* state, char* filename, ALLEGRO_FONT *font) {
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    double lastTime = al_get_time();
    ALLEGRO_KEYBOARD_STATE key_state;

    int running = 1;
    int frame1 = 0, frame2 = 0;
    int timer_count = 0;
    int maxFrame1 = 5, maxFrame2 = 5;
    int movement1 = 0, previous_movement1 = 0;
    int movement2 = 0, previous_movement2 = 0;
    background bg;
    init_animated_background(&bg, 24.0, filename);  //supondo que já foi definido em algum lugar
    
    //characters new alteration
    ALLEGRO_BITMAP* player1_sheet;
    ALLEGRO_BITMAP* player2_sheet;
    player1_sheet = al_load_bitmap("../assets/characters/ken.png");
    player2_sheet = al_load_bitmap("../assets/characters/ken.png");

    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = !running;
        } 

        if (running) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                *state = EXIT;
                break;
            }

            if (event.type == ALLEGRO_EVENT_TIMER) {             
                //printf("%d", frame);
                //fflush(stdout);           

                double currentTime = al_get_time();
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                update_animated_background(&bg);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_animated_background(&bg);

                ALLEGRO_KEYBOARD_STATE key_state;
                al_get_keyboard_state(&key_state);

                timer_count++;

                handle_input(&key_state, player_1, player_2, &movement1, &movement2);
                
                if (movement1 == GET_DOWN) {
                    if ((frame1 < maxFrame1) && timer_count % 20 == 0) frame1++;  
                } else {     
                    if (timer_count % 20 == 0) frame1++;
                    if (player_1->attack != 0 && frame1 >= maxFrame1) player_1->attack = 0;
                    if (frame1 > maxFrame1) frame1 = 0;
                }

                if (player_1->isJumping == 1) movement1 = JUMP;
                else if (player_1->isJumping == 2 && (player_1->x > player_2->x)) movement1 = JUMP_FWD;
                else if (player_1->isJumping == 3 && (player_1->x > player_2->x)) movement1 = JUMP_BCK;
                else if (player_1->isJumping == 2) movement1 = JUMP_BCK;
                else if (player_1->isJumping == 3) movement1 = JUMP_FWD;

                if (player_1->attack == ATTACK_KICK) movement1 = KICK;
                else if (player_1->attack == ATTACK_PUNCH) movement1 = PUNCH;

                if (movement1 != previous_movement1) frame1 = 0;
                maxFrame1 = countFrames(movement1);

                if (movement2 == GET_DOWN) {
                    if ((frame2 < maxFrame2) && timer_count % 20 == 0) frame2++;  
                } else {     
                    if (timer_count % 20 == 0) frame2++;
                    if (player_2->attack != 0 && frame2 >= maxFrame2) player_2->attack = 0;
                    if (frame2 > maxFrame2) frame2 = 0;
                }

                if (player_2->isJumping == 1) movement2 = JUMP;
                else if (player_2->isJumping == 2 && (player_2->x > player_1->x)) movement2 = JUMP_FWD;
                else if (player_2->isJumping == 3 && (player_2->x > player_1->x)) movement2 = JUMP_BCK;
                else if (player_2->isJumping == 2) movement2 = JUMP_BCK;
                else if (player_2->isJumping == 3) movement2 = JUMP_FWD;

                if (player_2->attack == ATTACK_KICK) movement2 = KICK;
                else if (player_2->attack == ATTACK_PUNCH) movement2 = PUNCH;

                if (movement2 != previous_movement2) frame2 = 0;
                maxFrame2 = countFrames(movement2);

                update_position(player_1, player_2, deltaTime); 

                if (player_1->x > player_2->x){
                    draw_player(player1_sheet, player_1, frame1, movement1, 1);
                    draw_player(player2_sheet, player_2, frame2, movement2, 0);
                } else {
                    draw_player(player1_sheet, player_1, frame1, movement1, 0);
                    draw_player(player2_sheet, player_2, frame2, movement2, 1);
                }

                draw_scoreboard(player_1->health,player_2->health,X_SCREEN,Y_SCREEN,font);                                       

                al_flip_display();

                resetPlayer(player_1);
                resetPlayer(player_2);
                previous_movement1 = movement1;
                previous_movement2 = movement2;
            }
        } 
    }
    
    al_destroy_timer(timer);
    destroy_animated_background(&bg);
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

    player* player_1 = buildPlayer(61, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92);
    player* player_2 = buildPlayer(61, X_SCREEN-122, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92);

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Failed to load font.\n");
        return -1;
    }
 
    int state = MENU;
    int selected_option = MENU_START;
    int selected_image = 0;
    char filename[100];
    
    while (state != EXIT) {
        ALLEGRO_EVENT event;
        draw_menu(font, selected_option); 
        al_wait_for_event(queue, &event);

        switch (state) {
            case MENU:            
                draw_menu(font, selected_option); 
                if (event.type == ALLEGRO_EVENT_KEY_DOWN) { 
                    state = handle_menu_input(event, &selected_option);
                }
                break;
            case GAME:
                selected_image = show_image_menu(font, queue);
                if (selected_image == 0) strcpy(filename,"destroyed_dojo");
                else if (selected_image == 1) strcpy(filename,"dark_dojo");
                run_game(disp, queue, player_1, player_2, &state, filename, font); 
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