#include "street_fighter.h"

void draw_scoreboard(int score1, int score2, int x, int y, ALLEGRO_FONT *font) {
    int norma_size1=((x/2)-((score1*100)/x)); //pega a porcentagem do tamanho da tela
    int norma_size2=((x/2)+((score2*100)/x));
    
    char score1_text[100], score2_text[100];
    sprintf(score1_text, "%d", score1);
    sprintf(score2_text, "%d", score2);
    
    al_draw_text(font, al_map_rgb(255, 0, 0), norma_size1 - 50, 10, 0, score1_text);
    al_draw_text(font, al_map_rgb(0, 0, 255), norma_size2 + 50, 10, 0, score2_text);
    
    al_draw_filled_rectangle(norma_size1, 5,
                             x/2, 20, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(norma_size2, 5,
                             x/2, 20, al_map_rgb(0, 0, 255));
}

void init_animated_background(background* bg, float frame_rate, char *folder) {
    bg->current_frame = 0;
    bg->frame_duration = 1.0 / frame_rate;
    bg->last_frame_update_time = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        char filename[500];
        snprintf(filename, sizeof(filename), "../assets/background/%s/019-l6Xgvsw-%d.png", folder, i);
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
    al_draw_bitmap(bg->frames[bg->current_frame], 0, 0, 0);
}

void destroy_animated_background(background* bg) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        al_destroy_bitmap(bg->frames[i]);
    }
}

int show_image_menu(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue) {
    int selected_option = 0; // 0 para "Destroyed Dojo", 1 para "Dark Dojo"
    bool done = false;
        
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    // Array de backgrounds
    background bg[2];
    init_animated_background(&bg[0], 24,"destroyed_dojo");
    init_animated_background(&bg[1], 24,"dark_dojo");

    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        update_animated_background(&bg[selected_option]);  // Atualiza a animação do background atual
        al_clear_to_color(al_map_rgb(0, 0, 0));
        draw_img_menu(font, selected_option, &bg[selected_option]);  // Desenha o menu com a animação
        
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_DOWN:
                    selected_option = 1 - selected_option; // Toggle entre as opções
                    break;
                case ALLEGRO_KEY_ENTER:
                    done = true; // Saída do loop ao pressionar ENTER
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done = true; // Saída do loop ao pressionar ESCAPE
                    break;
            }
        }
    }

    // Limpeza dos backgrounds
    for (int i = 0; i < 2; i++) {
        destroy_animated_background(&bg[i]);
    }

    return selected_option; // Retorna a opção selecionada
}

void draw_img_menu(ALLEGRO_FONT* font, int selected_option, background* bg) {
    update_animated_background(bg); // Atualiza a animação do background
    al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa a tela
    draw_animated_background(bg); // Desenha o background animado

    ALLEGRO_COLOR color_unselected = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR color_selected = al_map_rgb(255, 255, 0);

    al_draw_text(font, selected_option == 0 ? color_selected : color_unselected,
                 X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Destroyed Dojo");
    al_draw_text(font, selected_option == 1 ? color_selected : color_unselected,
                 X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Dark Dojo");

    al_flip_display();
}


void draw_menu(ALLEGRO_FONT* font, int selected_option) {
    al_clear_to_color(al_map_rgb(0, 0, 0));  // Limpa a tela com preto

    ALLEGRO_COLOR normal_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR selected_color = al_map_rgb(255, 255, 0);

    al_draw_text(font, selected_option == MENU_START ? selected_color : normal_color,
                 X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Start Game");

    al_draw_text(font, selected_option == MENU_EXIT ? selected_color : normal_color,
                 X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Exit Game");

    al_flip_display();
}


int handle_menu_input(ALLEGRO_EVENT event, int* selected_option) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
            case UP_2:
           // printf("UP/n"); fflush(stdout);
                if (*selected_option == MENU_EXIT) {
                    *selected_option = MENU_START;
                } 
                break;
            case DOWN_2:
        //    printf("DoWM/n"); fflush(stdout);
                if (*selected_option == MENU_START) {
                    *selected_option = MENU_EXIT;
                }
                break;
            case ALLEGRO_KEY_ENTER:
                return (*selected_option == MENU_START) ? GAME : EXIT;
            case ALLEGRO_KEY_ESCAPE:
                return EXIT;
        }
    }
    return MENU;  // Continua no menu
}

