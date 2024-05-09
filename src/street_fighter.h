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

#define MENU 0
#define GAME 1
#define EXIT 2

#define MENU_START 0
#define MENU_EXIT 1

#define NUM_SPRITES 10

typedef struct {
    ALLEGRO_BITMAP* frames[NUM_SPRITES];
    int current_frame, max_frames;
    float frame_width, frame_height;
} sprite;

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

void draw_scoreboard(int score1, int score2, int x, int y, ALLEGRO_FONT *font);
void draw_menu(ALLEGRO_FONT* font, int selected_option);
int handle_menu_input(ALLEGRO_EVENT event, int* selected_option);
int show_image_menu(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue);
void draw_img_menu(ALLEGRO_FONT* font, int selected_option, background* bg);
void init_animated_background(background* bg, float frame_rate, char *filename);
void update_animated_background(background* bg);
void draw_animated_background(const background* bg);
void destroy_animated_background(background* bg);

#endif
