//Compilação: gcc main.c character.c joystick.c street_fighter.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include "street_fighter.h"		

unsigned char collision_2D(player *element_first, player *element_second){
	if ((((element_first->y+element_first->side/2 >= element_second->y-element_second->side/2) && (element_second->y-element_second->side/2 >= element_first->y-element_first->side/2)) || 	
		((element_second->y+element_second->side/2 >= element_first->y-element_first->side/2) && (element_first->y-element_first->side/2 >= element_second->y-element_second->side/2))) && 	
		(((element_first->x+element_first->side/2 >= element_second->x-element_second->side/2) && (element_second->x-element_second->side/2 >= element_first->x-element_first->side/2)) || 
		((element_second->x+element_second->side/2 >= element_first->x-element_first->side/2) && (element_first->x-element_first->side/2 >= element_second->x-element_second->side/2)))) return 1;		
	else return 0;
}

void update_position(player *player_1, player *player_2, float time){																															
    if (player_1->control->up_right) {																																							
		movePlayer(player_1, 1, 5, X_SCREEN, Y_SCREEN);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 5, X_SCREEN, Y_SCREEN);																								
	}
	if (player_1->control->up_left) {																																							
		movePlayer(player_1, 1, 4, X_SCREEN, Y_SCREEN);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 4, X_SCREEN, Y_SCREEN);																								
	}
	if (player_1->control->left) {																																							
		movePlayer(player_1, 1, 0, X_SCREEN, Y_SCREEN);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 0, X_SCREEN, Y_SCREEN);																								
	}
	if (player_1->control->right) {																																							
		movePlayer(player_1, 1, 1, X_SCREEN, Y_SCREEN);																																	
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 1, X_SCREEN, Y_SCREEN);																							
	}
	if (player_1->control->up) {																															
		movePlayer(player_1, 1, 2, X_SCREEN, Y_SCREEN);																								
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 2, X_SCREEN, Y_SCREEN);															
	}
	if (player_1->control->down) {																											
		movePlayer(player_1, 1, 3, X_SCREEN, Y_SCREEN);																						
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 3, X_SCREEN, Y_SCREEN);										
	}

    if (player_2->control->up_right) {																																		
		movePlayer(player_2, 1, 5, X_SCREEN, Y_SCREEN);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 5, X_SCREEN, Y_SCREEN);																								
	}
	if (player_2->control->up_left) {																																					
		movePlayer(player_2, 1, 4, X_SCREEN, Y_SCREEN);																																		
		if (collision_2D(player_1, player_2)) movePlayer(player_1, -1, 4, X_SCREEN, Y_SCREEN);																								
	}
	if (player_2->control->left) {														
		movePlayer(player_2, 1, 0, X_SCREEN, Y_SCREEN);						
		if (collision_2D(player_2, player_1)) movePlayer(player_2, -1, 0, X_SCREEN, Y_SCREEN);		
	}
	
	if (player_2->control->right) { 						
		movePlayer(player_2, 1, 1, X_SCREEN, Y_SCREEN);		
		if (collision_2D(player_2, player_1)) movePlayer(player_2, -1, 1, X_SCREEN, Y_SCREEN);		
	}
	
	if (player_2->control->up) {									
		movePlayer(player_2, 1, 2, X_SCREEN, Y_SCREEN);		
		if (collision_2D(player_2, player_1)) movePlayer(player_2, -1, 2, X_SCREEN, Y_SCREEN);	
	}
	if (player_2->control->down) {		
		movePlayer(player_2, 1, 3, X_SCREEN, Y_SCREEN);						
		if (collision_2D(player_2, player_1)) movePlayer(player_2, -1, 3, X_SCREEN, Y_SCREEN);						
	}
    updatePlayer(player_1, time, Y_SCREEN-player_1->height/2);
    updatePlayer(player_2, time, Y_SCREEN-player_2->height/2);
}

void draw_players(player* player_1, player* player_2) {
    al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->height/2,
                             player_1->x+player_1->side/2, player_1->y+player_1->height/2, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->height/2,
                             player_2->x+player_2->side/2, player_2->y+player_2->height/2, al_map_rgb(0, 0, 255));
}

void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, player* player_1, player* player_2) {
    al_get_keyboard_state(key_state);

    // Player 1 controls
    if (al_key_down(key_state, ALLEGRO_KEY_W)) {
        if (al_key_down(key_state, ALLEGRO_KEY_D)) {
            joystick_up_right(player_1->control);
        } else if (al_key_down(key_state, ALLEGRO_KEY_A)) {
            joystick_up_left(player_1->control);
        } else {
            joystick_up(player_1->control);
        }
    } else if (al_key_down(key_state, ALLEGRO_KEY_A)) {
        joystick_left(player_1->control);
    } else if (al_key_down(key_state, ALLEGRO_KEY_D)) {
        joystick_right(player_1->control);
    } else if (al_key_down(key_state, ALLEGRO_KEY_S)) {
        joystick_down(player_1->control);
    }

    // Player 2 controls
    if (al_key_down(key_state, ALLEGRO_KEY_UP)) {
        if (al_key_down(key_state, ALLEGRO_KEY_RIGHT)) {
            joystick_up_right(player_2->control);
        } else if (al_key_down(key_state, ALLEGRO_KEY_LEFT)) {
            joystick_up_left(player_2->control);
        } else {
            joystick_up(player_2->control);
        }
    } else if (al_key_down(key_state, ALLEGRO_KEY_LEFT)) {
        joystick_left(player_2->control);
    } else if (al_key_down(key_state, ALLEGRO_KEY_RIGHT)) {
        joystick_right(player_2->control);
    } else if (al_key_down(key_state, ALLEGRO_KEY_DOWN)) {
        joystick_down(player_2->control);
    }
    
    if (!al_key_down(key_state, DOWN_1)) player_1->height = player_1->originalHeight;	
    else {
        player_1->y = Y_SCREEN;		
        player_1->height = player_1->originalHeight/2;		
    }		
    if (!al_key_down(key_state, DOWN_2)) player_2->height = player_1->originalHeight;	
    else {
        player_2->y = Y_SCREEN;		
        player_2->height = player_2->originalHeight/2;		
    }																															

}

void run_game(ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT_QUEUE* queue, player* player_1, player* player_2, int* state, char* filename, ALLEGRO_FONT *font) {
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    double lastTime = al_get_time();
    ALLEGRO_KEYBOARD_STATE key_state;

    int running = 1;
    background bg;
    init_animated_background(&bg, 24.0, filename);  //supondo que já foi definido em algum lugar

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
                double currentTime = al_get_time();
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;

                update_animated_background(&bg);
                al_clear_to_color(al_map_rgb(0, 0, 0));
                draw_animated_background(&bg);

                ALLEGRO_KEYBOARD_STATE key_state;
                al_get_keyboard_state(&key_state);
                handle_input(&key_state, player_1, player_2);

                update_position(player_1, player_2, deltaTime); 
                draw_scoreboard(100,100,X_SCREEN,Y_SCREEN,font);                                       
                draw_players(player_1, player_2);

                al_flip_display();

                resetPlayer(player_1);
                resetPlayer(player_2);
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

    player* player_1 = buildPlayer(20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 40);
    player* player_2 = buildPlayer(20, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 40);

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
                //filename = file_choose;
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

