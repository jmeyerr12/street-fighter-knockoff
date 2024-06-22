#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <allegro5/allegro.h>
#include "constants.h"
#include <stdio.h>

typedef struct {
    ALLEGRO_BITMAP* frames[NUM_FRAMES];
    int current_frame;
    float frame_duration;  //duração de cada frame em segundos
    double last_frame_update_time;
} background;

void init_animated_background(background* bg, float frame_rate, char *filename);
void update_animated_background(background* bg);
void draw_animated_background(const background* bg);
void destroy_animated_background(background* bg);

#endif
