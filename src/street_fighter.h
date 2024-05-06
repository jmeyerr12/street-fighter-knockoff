#ifndef __STREET_FIGHTER__
#define __STREET_FIGHTER__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>	

#include "character.h"
#include "joystick.h"

#define X_SCREEN 320								
#define Y_SCREEN 320	

#define UP_1 23
#define DOWN_1 19
#define LEFT_1 1
#define RIGHT_1 4

#define UP_2 84
#define DOWN_2 85
#define LEFT_2 82
#define RIGHT_2 83

typedef struct {
    int up;
    int down;
    int left;
    int right;
} keyState;

#endif
