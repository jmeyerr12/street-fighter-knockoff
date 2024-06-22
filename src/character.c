#include "character.h"

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

bool isInRange(player *attacker, player *defender, int attack) {
    //                 PNCH KCK JMPK JMPP DWNK DWNP
    int ranges[4][6] = {{30, 30, 30, 30, 30, 30}, //chun li
                        {30, 30, 30, 30, 30, 30}, //ken
                        {30, 30, 30, 30, 30, 30}, //zangief
                        {30, 30, 30, 30, 30, 30}}; //bison

    int attacker_left = attacker->NW.x;
    int attacker_right = attacker->SE.x;
    int attacker_top = attacker->NW.y;
    int attacker_bottom = attacker->SE.y;

    int defender_left = defender->NW.x;
    int defender_right = defender->SE.x;
    int defender_top = defender->NW.y;
    int defender_bottom = defender->SE.y;

    switch (attack) {
        case PUNCH:
            attacker_left = attacker->NW.x + (attacker->direction==LEFT ? 0 : -ranges[attacker->sprite][ATTACK_PUNCH-1]);
            attacker_right = attacker->SE.x + (attacker->direction==LEFT ? ranges[attacker->sprite][ATTACK_PUNCH-1] : 0);
            attacker_top = attacker->NW.y;
            attacker_bottom = attacker->SE.y - 60;
            break;
        case KICK:
            attacker_left = attacker->NW.x + (attacker->direction==LEFT ? 0 : -ranges[attacker->sprite][ATTACK_KICK-1]);
            attacker_right = attacker->SE.x + (attacker->direction==LEFT ? ranges[attacker->sprite][ATTACK_KICK-1] : 0);
            attacker_top = attacker->NW.y + 40;
            attacker_bottom = attacker->SE.y;
            break;
        case JUMPING_KICK:
            attacker_left = attacker->NW.x + (attacker->direction==LEFT ? 0 : -ranges[attacker->sprite][ATTACK_JUMPING_KICK-1]);
            attacker_right = attacker->SE.x + (attacker->direction==LEFT ? ranges[attacker->sprite][ATTACK_JUMPING_KICK-1] : 0);
            attacker_top = attacker->NW.y;
            attacker_bottom = attacker->SE.y + 60;
            break;
        case JUMPING_PUNCH:
            attacker_left = attacker->NW.x + (attacker->direction==LEFT ? 0 : -ranges[attacker->sprite][ATTACK_JUMPING_PUNCH-1]);
            attacker_right = attacker->SE.x + (attacker->direction==LEFT ? ranges[attacker->sprite][ATTACK_JUMPING_PUNCH-1] : 0);
            attacker_top = attacker->NW.y;
            attacker_bottom = attacker->SE.y + 60;
            break;
        case DOWN_KICK:
            attacker_left = attacker->NW.x + (attacker->direction==LEFT ? 0 : -ranges[attacker->sprite][ATTACK_DOWN_KICK-1]);
            attacker_right = attacker->SE.x + (attacker->direction==LEFT ? ranges[attacker->sprite][ATTACK_DOWN_KICK-1] : 0);
            attacker_top = attacker->NW.y;
            attacker_bottom = attacker->SE.y + 60;
            break;
        case DOWN_PUNCH:
            attacker_left = attacker->NW.x + (attacker->direction==LEFT ? 0 : -ranges[attacker->sprite][ATTACK_DOWN_PUNCH-1]);
            attacker_right = attacker->SE.x + (attacker->direction==LEFT ? ranges[attacker->sprite][ATTACK_DOWN_PUNCH-1] : 0);
            attacker_top = attacker->NW.y;
            attacker_bottom = attacker->SE.y + 60;
            break;
        default:
            return false;  
    }

    bool horizontal_overlap = attacker->direction == LEFT ? 
        (attacker_left <= defender_right && attacker_right >= defender_left) : 
        (attacker_left <= defender_right && attacker_right >= defender_left);

    bool vertical_overlap = !(defender_bottom < attacker_top || defender_top > attacker_bottom);

    return horizontal_overlap && vertical_overlap;
}


size** characterSizes() {
    // Tamanhos comuns para cada personagem (idle)
    size commonSizes[4] = {
        {72, 85}, // chun li
        {60, 90}, // ken
        {100, 70}, // zangief
        {95, 60}  // bison
    };

    // Tamanhos específicos de salto para cada personagem
    size jumpSizes[4][5] = {
        { {72, 85}, {72, 85}, {72, 85}, {72, 85}, {72, 85} },   // chun li
        { {60, 90}, {60, 90}, {60, 90}, {60, 90}, {60, 90} },   // ken
        { {98, 69}, {100, 70}, {99, 71}, {48, 70}, {48, 70} }, // zangief
        { {96, 62}, {95, 60}, {97, 61}, {48, 70}, {48, 70} }   // bison 
    };

    // Tamanhos específicos de agachamento para cada personagem
    size downSizes[4][4] = {
        { {72, 66}, {72, 66}, {72, 59}, {72, 59} },   // chun li
        { {61, 61}, {61, 61}, {61, 59}, {61, 59} },   // ken
        { {61, 61}, {61, 61}, {99, 68}, {99, 68} }, // zangief
        { {61, 61}, {61, 61}, {94, 60}, {94, 60} }   // bison 
    };

    size** charSizes = malloc(4 * sizeof(size*)); 

    for (int i = 0; i < 4; i++) {
        charSizes[i] = malloc(16 * sizeof(size)); 

        // Copiar tamanhos comuns para os primeiros 6 estados
        for (int j = 0; j < 6; j++) {
            charSizes[i][j] = commonSizes[i];
        }

        // Copiar tamanhos específicos de salto para os próximos 5 estados
        memcpy(&charSizes[i][6], jumpSizes[i], 5 * sizeof(size)); 

        // Copiar tamanhos específicos de agachamento para os últimos 4 estados
        memcpy(&charSizes[i][11], downSizes[i], 4 * sizeof(size)); 
    }

    return charSizes;
}
// chun li previous tested sizes -> {{43, 87}, {43, 87}, {50, 87}, {50, 87}, {71, 66}, {50, 75}, {50, 75}, {50, 75}, {50, 75}, {71, 66}, {50, 75}, {71, 66}}, 

void freeCharacterSizes(size** charSizes) {
    if (charSizes) {
        for (int i = 0; i < 4; i++) {
            if (charSizes[i]) {
                free(charSizes[i]);
                charSizes[i] = NULL;  // Definir o ponteiro como nulo após liberar a memória
            }
        }
        free(charSizes);
        charSizes = NULL;  // Definir o ponteiro como nulo após liberar a memória
    }
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
    new->isBeingHit = 0;			
    new->stamina = 100;				
    new->isDefending = 0;
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
        p->SW.x = p->x;
        p->SW.y = py;
        p->SE.x = p->x + p->width;
        p->SE.y = py;
        p->NW.x = p->x;
        p->NW.y = py - p->height;
        p->NE.x = p->x + p->width;
        p->NE.y = py - p->height;
    }
}

void recharge_stamina(player *p) {
    if (p->stamina < 100) p->stamina++;
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
    element->control->down_left = 0;
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