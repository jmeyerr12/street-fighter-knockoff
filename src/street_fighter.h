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
#include <allegro5/allegro_image.h>	

#include "character.h"
#include "joystick.h"
#include "menu.h"

#define X_SCREEN 600								
#define Y_SCREEN 300	

#define UP_1 23
#define DOWN_1 19
#define LEFT_1 1
#define RIGHT_1 4

#define UP_2 84
#define DOWN_2 85
#define LEFT_2 82
#define RIGHT_2 83

#define NUM_FRAMES 8  

typedef struct {
    ALLEGRO_BITMAP* frames[NUM_FRAMES];
    int current_frame;
    float frame_duration;  //duração de cada frame em segundos
    double last_frame_update_time;
} background;


typedef struct {
    int up;
    int down;
    int left;
    int right;
} keyState;

void init_animated_background(background* bg, float frame_rate, char *filename);
void update_animated_background(background* bg);
void draw_animated_background(const background* bg);
void destroy_animated_background(background* bg);

#endif
