#include "background.h"

void init_animated_background(background* bg, float frame_rate, char *folder) {
    bg->current_frame = 0;
    bg->frame_duration = 1.0 / frame_rate;
    bg->last_frame_update_time = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        char filename[500];
        snprintf(filename, sizeof(filename), "./assets/background/%s/019-l6Xgvsw-%d.png", folder, i);
        bg->frames[i] = al_load_bitmap(filename);
        if (!bg->frames[i]) {
            fprintf(stderr, "Failed to load frame %d. - IMAGE %s\n", i, filename);
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
    int original_width = al_get_bitmap_width(bg->frames[bg->current_frame]);
    int original_height = al_get_bitmap_height(bg->frames[bg->current_frame]);

    al_draw_scaled_bitmap(bg->frames[bg->current_frame], 
                          0, 0, 
                          original_width, original_height,
                          0, 0,
                          X_SCREEN, Y_SCREEN, 
                          0); 
}

void destroy_animated_background(background* bg) {
    if (bg && bg->frames) {
        for (int i = 0; i < NUM_FRAMES; i++) {
            if (bg->frames[i]) {
                al_destroy_bitmap(bg->frames[i]);
                bg->frames[i] = NULL;
            }
        }
    }
}