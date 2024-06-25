#include "graphics.h"

void draw_player(ALLEGRO_BITMAP* p, player* player, int current, int movement, int inverted) {
  al_draw_bitmap_region(p, 
                          112*current,                        // Posição x inicial no sprite sheet
                          112*movement,                       // Posição y inicial no sprite sheet
                          112,                        // Largura do sprite
                          112,                       // Altura do sprite
                          inverted ? player->x + player->width - 112 : player->x,  // Posição x na tela, ajustada para inversão
                          player->y,                         // Posição y na tela
                          inverted ? ALLEGRO_FLIP_HORIZONTAL : 0);  // Flag para inverter o sprite
}

void draw_scoreboard(int score1, int score2, int x, ALLEGRO_FONT *font, int countdown, int round, int points1, int points2, unsigned int stamina1, unsigned int stamina2) {
    score1 = (x < 0) ? 0 : score1;
    score2 = (x < 0) ? 0 : score2;
    int norma_size1=((x/2)-((score1*100)/x)); //pega a porcentagem do tamanho da tela
    int norma_size2=((x/2)+((score2*100)/x));

    char mid_text[100];
    sprintf(mid_text, "%d", countdown);
    int mid_text_x = x / 2 - al_get_text_width(font, mid_text) / 2;
    int mid_text_y = 30; 
    
    char score1_text[100], score2_text[100];
    sprintf(score1_text, "%d", score1 >= 0 ? score1 : 0);
    sprintf(score2_text, "%d", score2 >= 0 ? score2 : 0);

    char round_atual[100];
    sprintf(round_atual, "Round %d", round);

    char points[100];
    sprintf(points, "%d x %d", points1, points2);
    
    al_draw_text(font, al_map_rgb(255, 0, 0), norma_size1 - 50, 15, 0, score1_text);
    al_draw_text(font, al_map_rgb(0, 0, 255), norma_size2 + 20, 15, 0, score2_text);

    al_draw_text(font, al_map_rgb(255, 255, 0), x/2-100000/x, mid_text_y, 0, round_atual);
    al_draw_text(font, al_map_rgb(255, 255, 0), x/2+75000/x, mid_text_y, 0, points);
    
    //vida
    al_draw_filled_rectangle(norma_size1, 10,
                             x/2, 25, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(norma_size2, 10,
                             x/2, 25, al_map_rgb(0, 0, 255));

    //stamina

    al_draw_filled_rectangle(x/2-100000/x, 3,
                              x/2-100000/x + 100, 7, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(x/2+100000/x - 100, 3,
                              x/2+100000/x, 7, al_map_rgb(255, 255, 255));

    al_draw_filled_rectangle(x/2-100000/x, 4,
                              x/2-100000/x + stamina1, 6, al_map_rgb(255, 255, 0));
    al_draw_filled_rectangle(x/2+100000/x - stamina2, 4,
                              x/2+100000/x, 6, al_map_rgb(255, 255, 0));

    al_draw_text(font, al_map_rgb(255, 255, 0), mid_text_x, mid_text_y, 0, mid_text);
}

void draw_menu(ALLEGRO_FONT* font, int selected_option) {
    al_clear_to_color(al_map_rgb(0, 0, 0));  // Limpa a tela com preto

    ALLEGRO_COLOR normal_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR selected_color = al_map_rgb(255, 255, 0);

    float y_start = Y_SCREEN / 3;  // Ponto de partida mais centralizado
    float y_step = 40;  // Espaço menor entre os itens

    al_draw_text(font, selected_option == MENU_START ? selected_color : normal_color,
                 X_SCREEN / 2, y_start, ALLEGRO_ALIGN_CENTER, "Multiplayer");

    al_draw_text(font, selected_option == MENU_SINGLE_PLAYER ? selected_color : normal_color,
                 X_SCREEN / 2, y_start + y_step, ALLEGRO_ALIGN_CENTER, "Single Player");

    al_draw_text(font, selected_option == MENU_CONTROLS ? selected_color : normal_color,
                 X_SCREEN / 2, y_start + 2 * y_step, ALLEGRO_ALIGN_CENTER, "Controls");

    al_draw_text(font, selected_option == MENU_EXIT ? selected_color : normal_color,
                 X_SCREEN / 2, y_start + 3 * y_step, ALLEGRO_ALIGN_CENTER, "Exit Game");

    al_flip_display();
}

int draw_pause(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue) {
    int selected_option = 1; 
    bool done = false;

    al_draw_filled_rectangle(X_SCREEN * 0.4, Y_SCREEN * 0.25, X_SCREEN * 0.6, Y_SCREEN * 0.6, al_map_rgb(0, 0, 0));

    ALLEGRO_COLOR color_unselected = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR color_selected = al_map_rgb(255, 255, 0);

    al_draw_text(font, selected_option == 0 ? color_selected : color_unselected,
                    X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Menu");
    al_draw_text(font, selected_option == 1 ? color_selected : color_unselected,
                    X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Resume");

    al_flip_display();

    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Processa eventos
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_DOWN:
                    selected_option = 1 - selected_option; // Alterna entre as opções
                    break;
                case ALLEGRO_KEY_ENTER:
                    done = true; // Sai do loop ao pressionar ENTER
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done = true; // Sai do loop ao pressionar ESCAPE
                    break;
            }

            al_draw_filled_rectangle(X_SCREEN * 0.4, Y_SCREEN * 0.25, X_SCREEN * 0.6, Y_SCREEN * 0.6, al_map_rgb(0, 0, 0));

            al_draw_text(font, selected_option == 0 ? color_selected : color_unselected,
                         X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Menu");
            al_draw_text(font, selected_option == 1 ? color_selected : color_unselected,
                         X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Resume");

            al_flip_display();
        }
    }

    return selected_option == 0 ? MENU : GAME; // Retorna a opção selecionada
}

void draw_character_selection(ALLEGRO_BITMAP* sprite_sheet, int sprite_index, int x, int y, int sprite_dimension, ALLEGRO_COLOR color) { 
    int border_thickness = 2; 
    al_draw_rectangle(x, y, x + sprite_dimension, y + sprite_dimension, color, border_thickness);
    al_draw_bitmap_region(sprite_sheet, sprite_index * sprite_dimension, 0, sprite_dimension, sprite_dimension, 
                          x, y, 0);
}

void draw_characters_menu(int selected_option1, int selected_option2, ALLEGRO_BITMAP* heads, int done1, int done2) { 
    al_clear_to_color(al_map_rgb(0, 0, 0));
    ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255); //blue
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0); //red
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255); //white

    int start_x = (X_SCREEN - (2 * 32 + 10)) / 2; 
    int start_y = (Y_SCREEN - (2 * 32 + 10)) / 2 - 50; 
    int sprite_dimensions = 32;


    al_draw_bitmap_region(heads, 0, 32 + selected_option1 * 16, 96, 16, 
                        X_SCREEN/4, Y_SCREEN/2, 0);
    al_draw_bitmap_region(heads, selected_option1*112, 96, 112, 112, 
                        X_SCREEN/4, Y_SCREEN-115, 0);

    al_draw_bitmap_region(heads, 0, 32 + selected_option2 * 16, 96, 16, 
                        (X_SCREEN*3)/5, Y_SCREEN/2, 0);
    al_draw_bitmap_region(heads, selected_option2*112, 96, 112, 112, 
                        (X_SCREEN*1)/2+20, Y_SCREEN-115, ALLEGRO_FLIP_HORIZONTAL);

    draw_character_selection(heads, 0, start_x, start_y, sprite_dimensions, selected_option1==0? blue : (selected_option2==0? red : white));
    draw_character_selection(heads, 1, start_x + sprite_dimensions, start_y, sprite_dimensions, selected_option1==1? blue : (selected_option2==1? red : white));
    draw_character_selection(heads, 2, start_x, start_y + sprite_dimensions+3, sprite_dimensions, selected_option1==2? blue : (selected_option2==2? red : white));
    draw_character_selection(heads, 3, start_x + sprite_dimensions, start_y + sprite_dimensions+3, sprite_dimensions, selected_option1==3? blue : (selected_option2==3? red : white));
    
    if (done1 && done2) al_draw_bitmap_region(heads, 0, 272, 112, 112, (X_SCREEN / 2) - (64 / 2), (Y_SCREEN/2) + 64, 0);
    
    al_flip_display();
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

int handle_menu_input(ALLEGRO_EVENT event, int* selected_option) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                *selected_option = (*selected_option == MENU_START) ? MENU_EXIT : (*selected_option - 1);
                break;
            case ALLEGRO_KEY_DOWN:
                *selected_option = (*selected_option == MENU_EXIT) ? MENU_START : (*selected_option + 1);
                break;
            case ALLEGRO_KEY_ENTER:
                if (*selected_option == MENU_START) return GAME;
                if (*selected_option == MENU_SINGLE_PLAYER) return SINGLE_PLAYER;
                if (*selected_option == MENU_CONTROLS) return CONTROLS;
                if (*selected_option == MENU_EXIT) return EXIT;
                break;
            case ALLEGRO_KEY_ESCAPE:
                return EXIT;
        }
    }
    return MENU;
}

int show_image_menu(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue, int *state) {
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

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *state = EXIT;
            break;
        }

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

void show_characters_menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP** player1_sheet, ALLEGRO_BITMAP** player2_sheet, int *selected_option1, int *selected_option2, int *state) {
    *selected_option1 = 0;
    *selected_option2 = 1;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    ALLEGRO_BITMAP* heads = al_load_bitmap("./assets/characters/heads.png");

    bool done1, done2 = false;

    while (!(done1 && done2)) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        draw_characters_menu(*selected_option1, *selected_option2, heads, done1, done2);  
        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *state = EXIT;
            break;
        }

        if (done1 && done2) al_draw_bitmap_region(heads, 0, 276 - 48, 64, 48, (X_SCREEN / 2) - (64 / 2), (Y_SCREEN / 2) - (48 / 2), 0);
        
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if (!done2) {
                        if (*selected_option2 == 3 && *selected_option1 != 1) *selected_option2 = 1;
                        else if (*selected_option2 == 2 && *selected_option1 != 0) *selected_option2 = 0;
                    }
                    break;
                case ALLEGRO_KEY_DOWN:
                    if (!done2) {
                        if (*selected_option2 == 1 && *selected_option1 != 3) *selected_option2 = 3;
                        else if (*selected_option2 == 0 && *selected_option1 != 2) *selected_option2 = 2;
                    }
                    break;
                case ALLEGRO_KEY_LEFT:
                    if (!done2) {
                        if (*selected_option2 == 1 && *selected_option1 != 0) *selected_option2 = 0;
                        else if (*selected_option2 == 3 && *selected_option1 != 2) *selected_option2 = 2;
                    }
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if (!done2) {
                        if (*selected_option2 == 0 && *selected_option1 != 1) *selected_option2 = 1;
                        else if (*selected_option2 == 2 && *selected_option1 != 3) *selected_option2 = 3;
                    }
                    break;
                case ALLEGRO_KEY_W:
                    if (!done1) {
                        if (*selected_option1 == 3 && *selected_option2 != 1) *selected_option1 = 1;
                        else if (*selected_option1 == 2 && *selected_option2 != 0) *selected_option1 = 0;
                    }
                    break;
                case ALLEGRO_KEY_S:
                    if (!done1) {
                        if (*selected_option1 == 1 && *selected_option2 != 3) *selected_option1 = 3;
                        else if (*selected_option1 == 0 && *selected_option2 != 2) *selected_option1 = 2;
                    }
                    break;
                case ALLEGRO_KEY_A:
                    if (!done1) {
                        if (*selected_option1 == 1 && *selected_option2 != 0) *selected_option1 = 0;
                        else if (*selected_option1 == 3 && *selected_option2 != 2) *selected_option1 = 2;
                    }
                    break;
                case ALLEGRO_KEY_D:
                    if (!done1) {
                        if (*selected_option1 == 0 && *selected_option2 != 1) *selected_option1 = 1;
                        else if (*selected_option1 == 2 && *selected_option2 != 3) *selected_option1 = 3;
                    }
                    break;
                case ALLEGRO_KEY_ENTER:
                    done2 = true; 
                    break;
                case ALLEGRO_KEY_SPACE:
                    done1 = true; 
                    break;
            }
        }
    }

    if (*selected_option1 == 0) *player1_sheet = al_load_bitmap("./assets/characters/chun_li.png");
    else if (*selected_option1 == 1) *player1_sheet = al_load_bitmap("./assets/characters/ken.png");
    else if (*selected_option1 == 2) *player1_sheet = al_load_bitmap("./assets/characters/zangief.png");
    else if (*selected_option1 == 3) *player1_sheet = al_load_bitmap("./assets/characters/bison.png");

    if (*selected_option2 == 0) *player2_sheet = al_load_bitmap("./assets/characters/chun_li.png");
    else if (*selected_option2 == 1) *player2_sheet = al_load_bitmap("./assets/characters/ken.png");
    else if (*selected_option2 == 2) *player2_sheet = al_load_bitmap("./assets/characters/zangief.png");
    else if (*selected_option2 == 3) *player2_sheet = al_load_bitmap("./assets/characters/bison.png");

    draw_characters_menu(*selected_option1, *selected_option2, heads, done1, done2);  
    al_rest(1.5); 

    al_destroy_bitmap(heads); 
    al_destroy_timer(timer);   
}


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

void showWinner(ALLEGRO_FONT *font, int winner) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (winner == 0) {
        al_draw_text(font, al_map_rgb(255, 255, 0),
                X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "DRAW");
    } else {
        char text[100];
        sprintf(text, "PLAYER %d WINS THE ROUND", winner);
        al_draw_text(font, al_map_rgb(255, 255, 0),
                X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, text);
    }
    al_flip_display();
    al_rest(1);
}

void showEndgame(ALLEGRO_FONT *font, int winner) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    if (winner == 0) {
        al_draw_text(font, al_map_rgb(255, 255, 0),
                X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "DRAW");
    } else {
        char text[100];
        sprintf(text, "PLAYER %d WINS THE MATCH", winner);
        al_draw_text(font, al_map_rgb(255, 255, 0),
                X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, text);
    }
    al_flip_display();
    al_rest(1);
}

void draw_tutorial(ALLEGRO_FONT* font) {
    al_clear_to_color(al_map_rgb(0, 0, 0));  // Limpa a tela com preto

    ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR player1_color = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR player2_color = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR divider_color = al_map_rgb(255, 255, 0);

    int item_height = al_get_font_line_height(font);
    int controls_start_y = 20;  // Posição inicial y

    // Desenha o título
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "PRESSIONE ENTER PARA VOLTAR AO MENU");

    controls_start_y += item_height * 4;

    // Desenha os controles dos jogadores na mesma altura
    int player_controls_start_y = controls_start_y;

    int player1_x = X_SCREEN / 2 - 5;
    int player2_x = X_SCREEN / 2 + 5;

    // Desenha os controles do jogador 1
    al_draw_text(font, player1_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "PLAYER 1");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "W: PULA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "A: ESQUERDA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "S: ABAIXA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "D: DIREITA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "R: SOCO");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "F: CHUTE");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player1_x, player_controls_start_y, ALLEGRO_ALIGN_RIGHT, "T: ESPECIAL");

    player_controls_start_y = controls_start_y; // Reset y position for player 2

    // Desenha os controles do jogador 2
    al_draw_text(font, player2_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "PLAYER 2");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "SETA PARA CIMA: PULA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "SETA PARA ESQUERDA: ESQUERDA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "SETA PARA BAIXO: ABAIXA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "SETA PARA DIREITA: DIREITA");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "P: SOCO");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "L: CHUTE");
    player_controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, player2_x, player_controls_start_y, ALLEGRO_ALIGN_LEFT, "I: ESPECIAL");

    al_draw_line(X_SCREEN / 2, controls_start_y, X_SCREEN / 2, player_controls_start_y + item_height*1.5, divider_color, 2);
    
    controls_start_y += item_height * 14; // Posição y para controles gerais

    // Desenha os controles gerais
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "CONTROLES GERAIS");
    controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "Esc: Pause");
    controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "Setas: Navegação no Menu");
    controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "Enter: Selecionar Opção");
    controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "Os ataques podem ser realizados pelo ar e agachado");
    controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "Defesa: Se mover para o lado oposto do oponente");
    controls_start_y += item_height * 1.5;
    al_draw_text(font, text_color, X_SCREEN / 2, controls_start_y, ALLEGRO_ALIGN_CENTER, "(pelo ar e agachado)");

    al_flip_display();
}
