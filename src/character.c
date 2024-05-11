#include "character.h"

player* buildPlayer(unsigned char width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char height) {						
	if ((x < 0) || (x > max_x) || (y < 0) || (y > max_y)) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }			
	player *new = (player*) malloc(sizeof(player));				
    if (!new) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }				
    new->height = height;
    new->originalHeight = height;								
	new->x = x;										
	new->y = y;	
    new->health = 1000;									
	new->control = joystick_create();									
	return new;						
}

void updatePlayer(player *element, float time, unsigned short groundLevel, unsigned short bounds) {
    element->speed_y += GRAVITY*time;
    
    element->x += element->speed_x;
    element->y += element->speed_y*time;

    if (element->y >= groundLevel) { 
        element->y = groundLevel;
        element->isJumping = 0;
        element->speed_y = 0;
        element->speed_x = 0;
    }
   
    if (element->x+element->width >= bounds) {
        element->x = bounds-element->width;
        element->speed_x = 0;
    } else if (element->x <= 6) {
        element->x = 6;
        element->speed_x = 0;
    }
}
																
void resetPlayer(player *element) {
    element->control->right = 0;
    element->control->left = 0;	
    element->control->up = 0;
    //element->control->down = 0;
    element->control->up_left = 0;
    element->control->up_right = 0;
}

void movePlayer(player *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y) {
    switch (trajectory) {
        case 0:  // Move left
            if (/*((element->x - steps * STEP) - element->width >= 0) && */ !element->isJumping) element->x -= steps * STEP;
            break;
        
        case 1:  // Move right
            if (/*((element->x + steps * STEP) + element->width <= max_x) && */ !element->isJumping) element->x += steps * STEP;
            break;

        case 2:  // Move up 
            if (!element->isJumping) {
                element->isJumping = 1;
                element->speed_y = INITIAL_JUMP_SPEED;
            }
            break;
        
        
        case 4: // Move up left
            if (!element->isJumping) {
                element->isJumping = 2;
                element->speed_y = INITIAL_JUMP_SPEED;
                element->speed_x = -steps;  
            }
            break;
        
        case 5: // Move up right
            if (!element->isJumping) {
                element->isJumping = 3;
                element->speed_y = INITIAL_JUMP_SPEED;
                element->speed_x = steps;  
            }
            break;
            
        default:
            break;
    }
}

void destroyPlayer(player *element){									
	joystick_destroy(element->control);							
	free(element);						
}