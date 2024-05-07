#ifndef __SQUARE__ 																												
#define __SQUARE__																																																					

#include "joystick.h"	
#include <stdbool.h>		
#include <stdio.h>	
#include <stdlib.h>																							

#define STEP 10		
#define GRAVITY 75
#define INITIAL_JUMP_SPEED -100.0																					

typedef struct {
    unsigned char side, height, originalHeight;
    unsigned short x, y;
    joystick *control;
    float speed_y, speed_x;         
    bool isJumping;     
    bool isDown;
} player;
																											
player* buildPlayer(unsigned char side, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char height);		
void movePlayer(player *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y);			
void destroyPlayer(player *element);					
void updatePlayer(player *element, float time, unsigned short groundLevel);																	
void resetPlayer(player *element);
void update_position(player *player_1, player *player_2, float time);

#endif																													
