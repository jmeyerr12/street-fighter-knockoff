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

#define STEP 2	
#define GRAVITY 300
#define INITIAL_JUMP_SPEED -240.0	

#define ATTACK_PUNCH 1
#define ATTACK_KICK 2
#define ATTACK_JUMPING_KICK 3
#define ATTACK_DOWN_PUNCH 4
#define ATTACK_JUMPING_PUNCH 5
#define ATTACK_DOWN_KICK 6

#define IDLE 0
#define WALK 1
#define PUNCH 2
#define KICK 3
#define GET_DOWN 4
#define JUMP 5
#define JUMP_FWD 6
#define JUMP_BCK 7
#define DOWN_PUNCH 8
#define JUMPING_KICK 9
#define JUMPING_PUNCH 10
#define DOWN_KICK 11
#define DAMAGED 12

#define SPRITE_HEIGHT 112
#define SPRITE_WIDTH 112

#define KEN 0
#define CHUN_LI 1
#define BISON 2
#define ZANGIEF 3

#define LEFT 0
#define RIGHT 1

typedef enum {
    STATE_NORMAL,
    STATE_DEFENSIVE,
    STATE_AGGRESSIVE
} player_state;

typedef struct {
    unsigned int x,y;
} coordinates;

typedef struct {
    unsigned int sprite;
    unsigned int width, height, originalHeight;
    unsigned int x, y;
    coordinates SW, SE, NW, NE;
    int health;
    joystick *control;
    float speed_y, speed_x;         
    int isJumping, isDown;  //implementar no codigo mudanca de altura, nao esta feita (isDown)
    int attack;
    int previous_movement;
    unsigned int estamina; //implementar (parece simples)
    int direction;
    int isBeingHit;
} player;

typedef struct {
    unsigned int width, height;
} size;
																											
player* buildPlayer(unsigned int width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned int height, int direction);	
void movePlayer(player *element, char steps, unsigned char trajectory);			
void destroyPlayer(player *element);					
void updatePlayer(player *element, float time, unsigned short groundLevel, unsigned int bounds);																	
void resetPlayer(player *element);
void update_position(player *player_1, player *player_2, float time);
bool isInRange(player *attacker, player *defender, int attack);
void resetAttributes(player **p, unsigned int width, unsigned int height, unsigned short x, unsigned short y);
void printPlayerStatistics(player *p, int i);

size** characterSizes();
void freeCharacterSizes(size** charSizes);
void setDimensions(player *p, unsigned int width, unsigned int height);

void setHitbox(player *p);

#endif																													