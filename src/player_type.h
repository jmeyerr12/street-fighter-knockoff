#ifndef PLAYER_TYPE_H
#define PLAYER_TYPE_H

/*
    Biblioteca com o tipo "player", os tipos utilizados por "player" e o tipo player_state, referente aos estados do bot no single player 
*/

// Definição dos estados do bot
typedef enum {
    STATE_NORMAL,
    STATE_DEFENSIVE,
    STATE_AGGRESSIVE
} player_state;

// Definição da estrutura de coordenadas
typedef struct {
    unsigned int x, y;
} coordinates;

// Definição da estrutura de caixa de ataque
typedef struct {
    coordinates SW, SE, NW, NE;
} attack_box;

// Definição da estrutura de tamanho
typedef struct {
    unsigned int width, height;
} size;

// Definição da estrutura do jogador
typedef struct {
    unsigned int sprite;
    unsigned int width, height, originalHeight;
    unsigned int x, y;
    coordinates SW, SE, NW, NE;
    int health;
    joystick *control;
    float speed_y, speed_x;         
    int isJumping, isDown, isDefending;  
    int attack;
    int previous_movement;
    int stamina; 
    int direction;
    int isBeingHit;
    attack_box punch_box;
    attack_box kick_box;
} player;

#endif
