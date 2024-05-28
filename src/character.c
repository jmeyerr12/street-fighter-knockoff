#include "character.h"

bool isInRange(player *attacker, player *defender, int attack) {
    int attack_range;

    switch (attack) {
        case PUNCH:
            attack_range = 30;  
            break;
        case KICK:
            attack_range = 10;  
            break;
        case JUMPING_KICK:
            attack_range = 10;  
            break;
        case JUMPING_PUNCH:
            attack_range = 10;  
            break;
        case DOWN_KICK:
            attack_range = 10;  
            break;
        case DOWN_PUNCH:
            attack_range = 10;  
            break;
        default:
            return false;  
    }

    int attacker_left = attacker->NW.x;
    int attacker_right = attacker->SE.x;
    int attacker_top = attacker->NW.y;
    int attacker_bottom = attacker->SE.y;

    int defender_left = defender->NW.x;
    int defender_right = defender->SE.x;
    int defender_top = defender->NW.y;
    int defender_bottom = defender->SE.y;

    bool horizontal_overlap = (attacker_right + attack_range >= defender_left) && (attacker_left - attack_range <= defender_right);
    
    bool vertical_overlap = ((attacker_bottom >= defender_bottom) && (attacker_top <= defender_top));

    /* printPlayerStatistics(attacker, 1);
    //printf("attacker right: %d defender left: %d", attacker_right, defender_left);
    printPlayerStatistics(defender, 2);
    //printf("\nattacker_bottom: %d, defender_top: %d\n", attacker_bottom, defender_top);
    //printf("attacker_top: %d, defender_bottom: %d\n", attacker_top, defender_bottom);
    printf("Horizontal overlap: %d, Vertical overlap: %d\n", horizontal_overlap, vertical_overlap); */

    return horizontal_overlap && vertical_overlap;
}


size** characterSizes() { //TRANSFORMAR EM MATRIZ DE VETORES PARA PEGAR TODOS OS SPRITES!
    size initSizes[4][12] = { //                                                          --NADA FEITO POR AQUI                      
        //IDLE     WALKING   PUNCHING   KICKING   DOWN      JUMP     JUMP FWD  JUMP BCK  JUMP_KICK DOWN_PUNCH JUMP_PUNCH DOWN_KICK
        {{60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}},   // ken  -- DONE
        {{60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}, {60,60}},   // chun li  -- DONE  (aproximado (problema com diferenças ao longo do eixo y))
        {{95, 60}, {97, 62}, {93, 61}, {95, 63}, {94, 60}, {96, 62}, {95, 60}, {97, 61}, {48,70}, {61,61}, {48,70}, {61,61}},   // bison 
        {{100, 70}, {98, 71}, {99, 69}, {100, 70}, {99, 68}, {98, 69}, {100, 70}, {99, 71}, {48,70}, {61,61}, {48,70}, {61,61}} // zangief
    };

    size** charSizes = malloc(4 * sizeof(size*)); 

    for (int i = 0; i < 4; i++) {
        charSizes[i] = malloc(12 * sizeof(size)); 
        memcpy(charSizes[i], initSizes[i], 12 * sizeof(size)); 
    }

    return charSizes;
}

void freeCharacterSizes(size** charSizes) {
    for (int i = 0; i < 4; i++) {
        free(charSizes[i]);
    }
    free(charSizes); 
}

void setDimensions(player *p, unsigned int width, unsigned int height) {
    p->width = width;
    p->height = height;
}

player* buildPlayer(unsigned int width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned int height, int direction) {						
	if ((x > max_x) || (y > max_y)) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }			
	player *new = (player*) malloc(sizeof(player));				
    if (!new) {
        fprintf(stderr, "Failed to allocate memory for player\n");
        return NULL;
    }			
    new->direction = direction;	
    new->attack = 0;
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

void resetAttributes(player **p, unsigned int width, unsigned int height, unsigned short x, unsigned short y) {
    (*p)->width = width;
    (*p)->height = height;
    (*p)->originalHeight = height;	
    (*p)->isJumping = 0;
    (*p)->isDown = 0;
    (*p)->speed_x = 0;
    (*p)->speed_y = 0;							
	(*p)->x = x;										
	(*p)->y = y;	
    (*p)->health = 1000;		
}

//p->x, py, p->x+p->width, py, p->x, py+p->height, p->x+p->width, py+p->height,
void setHitbox(player *p) {
    int py = p->y + SPRITE_HEIGHT;

    if (p->direction == LEFT) {
        p->SW.x = p->x;
        p->SW.y = py;
        p->SE.x = p->x + p->width;
        p->SE.y = py;
        p->NW.x = p->x;
        p->NW.y = py - p->height;
        p->NE.x = p->x + p->width;
        p->NE.y = py - p->height;
    } else { // direction == RIGHT
        p->SW.x = p->x - p->width + SPRITE_WIDTH;
        p->SW.y = py;
        p->SE.x = p->x + SPRITE_WIDTH;
        p->SE.y = py;
        p->NW.x = p->x - p->width + SPRITE_WIDTH;
        p->NW.y = py - p->height;
        p->NE.x = p->x + SPRITE_WIDTH;
        p->NE.y = py - p->height;
    }
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

    if ((element->x <= 6) || (element->SE.x >= 600 - 6)) 
        element->speed_x = 0;

    setHitbox(element);
}
																
void resetPlayer(player *element) {
    element->control->right = 0;
    element->control->left = 0;	
    element->control->up = 0;
    element->control->down = 0;
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

void printPlayerStatistics(player *p, int i) {
    printf("\n\n -- player %d -- \nisJumping: %d\nisDown: %d\nattack: %s\nheight: %d\nwidth: %d\nx: %d y: %d\nSW: (%d,%d) | SE: (%d,%d)\nNW: (%d,%d) | NE: (%d,%d)\npunching range: (%d, %d)\nkicking range: (%d, %d)", 
            i, p->isJumping, p->isDown,
            !p->attack ? "NOT ATTACKING" : (p->attack == ATTACK_KICK ? "KICKING" : 
            (p->attack == ATTACK_PUNCH ? "PUNCHING" : 
            ((p->attack == ATTACK_JUMPING_PUNCH) ? "JMP PUNCH" : 
            ((p->attack == ATTACK_JUMPING_KICK) ? "JMP KICK" : 
            ((p->attack == ATTACK_DOWN_KICK) ? "DWN KICK" : 
            ((p->attack == ATTACK_DOWN_PUNCH) ? "DWN PUNCH" : "ERROR")))))),
            p->height, p->width,
            p->x, p->y, 
            p->SW.x, p->SW.y, p->SE.x, p->SE.y, p->NW.x, p->NW.y, p->NE.x, p->NE.y,
            0,0,0,0
            );
    fflush(stdout);
} 
