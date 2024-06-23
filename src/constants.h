#ifndef CONSTANTS_H
#define CONSTANTS_H

/* 
    Biblioteca que agrega as constantes utilizadas
*/

// Tamanho da tela
#define X_SCREEN 600								
#define Y_SCREEN 300	

// Outras constantes
#define NUM_FRAMES 8
#define STAMINA_DECREASE 15

// Tamanhos de sprite
#define SPRITE_HEIGHT 112
#define SPRITE_WIDTH 112

// Direções
#define LEFT 0
#define RIGHT 1

// Definições de movimento
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
#define DAMAGED_JMP 13
#define DAMAGED_DOWN 14
#define DEFENDING 15
#define DEFENDING_DOWN 16
#define WINNER 17
#define LOSER 18

// Definições de ataque
#define ATTACK_PUNCH 1
#define ATTACK_KICK 2
#define ATTACK_JUMPING_KICK 3
#define ATTACK_DOWN_PUNCH 4
#define ATTACK_JUMPING_PUNCH 5
#define ATTACK_DOWN_KICK 6

// Física
#define STEP 2	
#define GRAVITY 320
#define INITIAL_JUMP_SPEED -280.0	

// Estados de jogo
#define MENU 0
#define GAME 1
#define SINGLE_PLAYER 2
#define ENDGAME 3
#define EXIT 4

// Estados de Menu
#define MENU_START 0
#define MENU_SINGLE_PLAYER 1
#define MENU_EXIT 2

// Codigo dos sprites de personagens
#define KEN 0
#define CHUN_LI 1
#define BISON 2
#define ZANGIEF 3

#endif
