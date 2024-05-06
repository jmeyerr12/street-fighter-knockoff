//Compilação: gcc main.c character.c joystick.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 --libs --cflags)

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

int main(){
	al_init();																	
	al_init_primitives_addon();					
	
	al_install_keyboard();							

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);						
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();			
	ALLEGRO_FONT* font = al_create_builtin_font();							
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);							

	al_register_event_source(queue, al_get_keyboard_event_source());		
	al_register_event_source(queue, al_get_display_event_source(disp));	
	al_register_event_source(queue, al_get_timer_event_source(timer));	

	player *player_1 = buildPlayer(20, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 40);
    if (!player_1) {
        fprintf(stderr, "Failed to create player 1\n");
        return 1;
    }											
	player *player_2 = buildPlayer(20, X_SCREEN-10, Y_SCREEN/2, X_SCREEN, Y_SCREEN,40);
    if (!player_2) {
        fprintf(stderr, "Failed to create player 1\n");
        return 2;
    }															

	ALLEGRO_EVENT event;																
	al_start_timer(timer);			
	
    double lastTime = al_get_time(); 	
    
    ALLEGRO_KEYBOARD_STATE key_state;
    
	while(event.type != 42){																			
		al_wait_for_event(queue, &event);	
		al_get_keyboard_state(&key_state);
								
		if (event.type == 30) {
		    double currentTime = al_get_time();
            float deltaTime = currentTime - lastTime; 
            lastTime = currentTime; 
		
			update_position(player_1, player_2, deltaTime);										
			al_clear_to_color(al_map_rgb(0, 0, 0));										
			al_draw_filled_rectangle(player_1->x-player_1->side/2, player_1->y-player_1->height/2, player_1->x+player_1->side/2, player_1->y+player_1->height/2, al_map_rgb(255, 0, 0));
			al_draw_filled_rectangle(player_2->x-player_2->side/2, player_2->y-player_2->height/2, player_2->x+player_2->side/2, player_2->y+player_2->height/2, al_map_rgb(0, 0, 255));			
    		al_flip_display();
    		
    		player_1->control->right = 0;
    		player_1->control->left = 0;	
    		player_1->control->up = 0;
    		//player_1->control->down = 0;
    		player_1->control->up_left = 0;
    		player_1->control->up_right = 0;	
    		player_2->control->right = 0;
    		player_2->control->left = 0;	
    		player_2->control->up = 0;
    		//player_2->control->down = 0;
    		player_2->control->up_left = 0;
    		player_2->control->up_right = 0;																																						
		} 
        if (al_key_down(&key_state, UP_1)) {
            if (al_key_down(&key_state, RIGHT_1)) {
                joystick_up_right(player_1->control);
            } else if (al_key_down(&key_state, LEFT_1)) {
                joystick_up_left(player_1->control);
            } else {
                joystick_up(player_1->control);
            }
        } else if (al_key_down(&key_state, LEFT_1)) {
            joystick_left(player_1->control);
        } else if (al_key_down(&key_state, RIGHT_1)) {
            joystick_right(player_1->control);
        } else if (al_key_down(&key_state, DOWN_1)) {
            joystick_down(player_1->control);
        }

        // Repeat similar checks for player 2
        if (al_key_down(&key_state, UP_2)) {
            if (al_key_down(&key_state, RIGHT_2)) {
                joystick_up_right(player_2->control);
            } else if (al_key_down(&key_state, LEFT_2)) {
                joystick_up_left(player_2->control);
            } else {
                joystick_up(player_2->control);
            }
        } else if (al_key_down(&key_state, LEFT_2)) {
            joystick_left(player_2->control);
        } else if (al_key_down(&key_state, RIGHT_2)) {
            joystick_right(player_2->control);
        } else if (al_key_down(&key_state, DOWN_2)) {
            joystick_down(player_2->control);
        }
        
        if (!al_key_down(&key_state, DOWN_1)) player_1->height = player_1->originalHeight;	
        else {
            player_1->y = Y_SCREEN;		
            player_1->height = player_1->originalHeight/2;		
        }		
        if (!al_key_down(&key_state, DOWN_2)) player_2->height = player_1->originalHeight;	
        else {
            player_2->y = Y_SCREEN;		
            player_2->height = player_2->originalHeight/2;		
        }																															
																																	
	}

	al_destroy_font(font);																																											
	al_destroy_display(disp);																																										
	al_destroy_timer(timer);																																										
	al_destroy_event_queue(queue);																																									

	return 0;
}
