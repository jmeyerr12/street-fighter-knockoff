#include "character.h"

size** characterSizes() {
    size initSizes[4][8] = {
        {{90, 56}, {92, 57}, {88, 55}, {90, 58}, {89, 57}, {91, 56}, {90, 56}, {92, 57}},
        {{85, 50}, {83, 52}, {86, 53}, {85, 54}, {84, 51}, {85, 52}, {86, 50}, {84, 52}},
        {{95, 60}, {97, 62}, {93, 61}, {95, 63}, {94, 60}, {96, 62}, {95, 60}, {97, 61}},
        {{100, 70}, {98, 71}, {99, 69}, {100, 70}, {99, 68}, {98, 69}, {100, 70}, {99, 71}}
    };

    size** charSizes = malloc(4 * sizeof(size*)); 

    for (int i = 0; i < 4; i++) {
        charSizes[i] = malloc(8 * sizeof(size)); 
        memcpy(charSizes[i], initSizes[i], 8 * sizeof(size)); 
    }

    return charSizes;
}

void freeCharacterSizes(size** charSizes) {
    for (int i = 0; i < 4; i++) {
        free(charSizes[i]);
    }
    free(charSizes); 
}

player* buildPlayer(unsigned int width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned int height) {						
	if ((x < 0) || (x > max_x) || (y < 0) || (y > max_y)) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }			
	player *new = (player*) malloc(sizeof(player));				
    if (!new) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }				
    new->width = width;
    new->height = height;
    new->originalHeight = height;	
    new->isJumping = 0;
    new->isDown = 0;
    new->speed_x = 0;
    new->speed_y = 0;							
	new->x = x;										
	new->y = y;	
    new->health = 1000;									
	new->control = joystick_create();									
	return new;						
}

void updatePlayer(player *element, float time, unsigned short groundLevel, unsigned int bounds) {
    element->speed_y += GRAVITY*time;
    
    element->x += element->speed_x;
    element->y += element->speed_y*time;

    if (element->y >= groundLevel) { 
        element->y = groundLevel;
        element->isJumping = 0;
        element->speed_y = 0;
        element->speed_x = 0;
    }

    if (element->x+element->width*2 >= bounds) {
        element->x = bounds-element->width*2;
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

void movePlayer(player *element, char steps, unsigned char trajectory) {
    switch (trajectory) {
        case 0:  // Move left
            if (!element->isJumping) element->x -= steps * STEP;
            break;
        
        case 1:  // Move right
            if (!element->isJumping) element->x += steps * STEP;
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
