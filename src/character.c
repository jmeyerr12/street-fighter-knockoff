#include "character.h"

player* buildPlayer(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char height){						
	if ((x - side/2 < 0) || (x + side/2 > max_x) || (y - side/2 < 0) || (y + side/2 > max_y)) return NULL;					
	player *new = (player*) malloc(sizeof(player));				
    if (!new) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }				
    new->height = height;
    new->originalHeight = height;
	new->side = side;									
	new->x = x;										
	new->y = y;										
	new->control = joystick_create();									
	return new;						
}

void updatePlayer(player *element, float time, unsigned short groundLevel) {
    element->speed_y += GRAVITY*time;
    
    element->x += element->speed_x;
    element->y += element->speed_y*time;

    if (element->y >= groundLevel) { 
        element->y = groundLevel;
        element->isJumping = false;
        element->speed_y = 0;
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
            if (((element->x - steps * STEP) - element->side / 2 >= 0) && !element->isJumping) element->x -= steps * STEP;
            break;
        
        case 1:  // Move right
            if (((element->x + steps * STEP) + element->side / 2 <= max_x) && !element->isJumping) element->x += steps * STEP;
            break;

        case 2:  // Move up 
            if (!element->isJumping) {
                element->isJumping = true;
                element->speed_y = INITIAL_JUMP_SPEED;
            }
            break;

           
        case 3:  // Move down
            if (!element->isDown) {
                element->isDown = true;
                element->height = element->originalHeight/2;
            }
            break;
        
        
        case 4: // Move up left
            if (!element->isJumping) {
                element->isJumping = true;
                element->speed_y = INITIAL_JUMP_SPEED;
                element->speed_x = -steps;  
            }
            break;
        
        case 5: // Move up right
            if (!element->isJumping) {
                element->isJumping = true;
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
