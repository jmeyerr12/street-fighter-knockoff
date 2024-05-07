#include "street_fighter.h"

void init_animated_background(background* bg, float frame_rate, char *folder) {
    bg->current_frame = 0;
    bg->frame_duration = 1.0 / frame_rate;
    bg->last_frame_update_time = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        char filename[500];
        snprintf(filename, sizeof(filename), "../assets/background/%s/019-l6Xgvsw-%d.png", folder, i);
        bg->frames[i] = al_load_bitmap(filename);
        if (!bg->frames[i]) {
            fprintf(stderr, "Failed to load frame %d. - image %s\n", i, filename);
            exit(-1);
        }
    }
}

void update_animated_background(background* bg) {
    double now = al_get_time();
    if (now - bg->last_frame_update_time >= bg->frame_duration) {
        bg->current_frame = (bg->current_frame + 1) % NUM_FRAMES;
        bg->last_frame_update_time = now;
    }
}

void draw_animated_background(const background* bg) {
    al_draw_bitmap(bg->frames[bg->current_frame], 0, 0, 0);
}

void destroy_animated_background(background* bg) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        al_destroy_bitmap(bg->frames[i]);
    }
}
