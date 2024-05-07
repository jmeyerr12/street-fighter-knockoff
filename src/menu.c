#include "menu.h"

void draw_img_menu(ALLEGRO_FONT* font, int selected_option) {
    al_clear_to_color(al_map_rgb(0, 0, 0));  // Limpa a tela com preto

    char filename[1024];

    ALLEGRO_COLOR normal_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR selected_color = al_map_rgb(255, 255, 0);
    
    /*if (selected_option != -1) {
        if (selected_option == 0) strcpy(filename,"destroyed_dojo");
        
        background bg;
        init_animated_background(&bg, 24.0, filename); 
    }*/
    
    al_draw_text(font, selected_option == MENU_START ? selected_color : normal_color,
                 X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Destroyed dojo");

    al_draw_text(font, selected_option == MENU_EXIT ? selected_color : normal_color,
                 X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Not Implemented");

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
                return (*selected_option == MENU_START) ? IMG_MENU : EXIT;
            case ALLEGRO_KEY_ESCAPE:
                return EXIT;
        }
    }
    return MENU;  // Continua no menu
}

