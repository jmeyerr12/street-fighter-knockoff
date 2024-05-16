#ifndef __CHARACTER__ 																												
#define __CHARACTER__																																																					

#include "joystick.h"	
#include <stdbool.h>		
#include <stdio.h>	
#include <stdlib.h>				

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>			

#define ATTACK_PUNCH 1
#define ATTACK_KICK 2

#define STEP 2	
#define GRAVITY 300
#define INITIAL_JUMP_SPEED -240.0	

#define IDLE 0
#define WALK 1
#define PUNCH 2
#define KICK 3
#define GET_DOWN 4
#define JUMP 5
#define JUMP_FWD 6
#define JUMP_BCK 7

#define SPRITE_HEIGHT 112
#define SPRITE_WIDTH 112

#define KEN 0
#define CHUN_LI 1
#define BISON 2
#define ZANGIEF 3

typedef struct {
    unsigned int width, height, originalHeight;
    unsigned int x, y;
    int health;
    joystick *control;
    float speed_y, speed_x;         
    int isJumping, isDown;  //implementar no codigo mudanca de altura, nao esta feita (isDown)
    int attack;
    unsigned int estamina; //implementar (parece simples)
} player;

typedef struct {
    unsigned int height, width;
} size;
																											
player* buildPlayer(unsigned int width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned int height);	
void movePlayer(player *element, char steps, unsigned char trajectory);			
void destroyPlayer(player *element);					
void updatePlayer(player *element, float time, unsigned short groundLevel, unsigned int bounds);																	
void resetPlayer(player *element);
void update_position(player *player_1, player *player_2, float time);
bool isInRange(player *attacker, player *defender, int attack);
void resetAttributes(player **p, unsigned int width, unsigned int height, unsigned short x, unsigned short y);

#endif																													