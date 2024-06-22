//Compilação: gcc main.c character.c joystick.c street_fighter.c -o AS $(pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

#include "street_fighter.h"		

int main() {
    al_init();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();

    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(disp));

    al_register_event_source(queue, al_get_keyboard_event_source());

    player* player_1; 
    player* player_2; 
    
    ALLEGRO_BITMAP* player1_sheet;
    ALLEGRO_BITMAP* player2_sheet;

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Failed to load font.\n");
        return -1;
    }
 
    int state = MENU;
    int selected_option = MENU_START;
    int selected_image = 0;
    char filename[100];
    int p1Wins = 0;
    int p2Wins = 0;
    int roundCounter = 1;
    int sel1 = 0,sel2 = 1;

    srand(time(NULL));
    
    while (state != EXIT) {
        ALLEGRO_EVENT event;
        draw_menu(font, selected_option); 
        al_wait_for_event(queue, &event);

        switch (state) {
            case MENU:            
                draw_menu(font, selected_option); 
               // if (event.type == ALLEGRO_EVENT_KEY_DOWN)
                    state = handle_menu_input(event, &selected_option);
                break;
            case GAME:
                player_1 = buildPlayer(61, 25, Y_SCREEN-112, X_SCREEN, Y_SCREEN, 92, LEFT);
                player_2 = buildPlayer(61, X_SCREEN-75, Y_SCREEN-112, X_SCREEN, Y_SCREEN, 92, RIGHT);
                show_characters_menu(queue,&player1_sheet,&player2_sheet,&sel1,&sel2);
                player_1->sprite = sel1;
                player_2->sprite = sel2;
                selected_image = show_image_menu(font, queue);
                if (selected_image == 0) strcpy(filename,"destroyed_dojo");
                else if (selected_image == 1) strcpy(filename,"dark_dojo");
                while ((p1Wins < 2) && (p2Wins < 2) && (roundCounter <= 3)) { 
                    int resultado = run_round(queue, player_1, player_2, &state, filename, font, player1_sheet, player2_sheet, roundCounter, p1Wins, p2Wins); 
                    if (resultado == 0) { //em caso de empate o vencedor é sorteado
                        resultado = 1 + rand() % 2;
                    }
                    if (resultado == 1) p1Wins++;
                    else if (resultado == 2) p2Wins++;
                    else break; //clicou para sair
                    showWinner(font, resultado);
                    resetAttributes(&player_1, 61, 92, 10, Y_SCREEN/2);
                    resetAttributes(&player_2, 61, 92, X_SCREEN-122, Y_SCREEN/2);
                    roundCounter++;
                }
                al_destroy_bitmap(player1_sheet);
                al_destroy_bitmap(player2_sheet);
                joystick_destroy(player_1->control);
                joystick_destroy(player_2->control);
                free(player_1);
                free(player_2);
                state = ENDGAME;
                break;
            case SINGLE_PLAYER:
                player_1 = buildPlayer(61, 10, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92, LEFT);
                player_2 = buildPlayer(61, X_SCREEN-300, Y_SCREEN/2, X_SCREEN, Y_SCREEN, 92, RIGHT);
                show_characters_menu(queue,&player1_sheet,&player2_sheet,&sel1,&sel2);
                player_1->sprite = sel1;
                player_2->sprite = sel2;
                selected_image = show_image_menu(font, queue);
                if (selected_image == 0) strcpy(filename,"destroyed_dojo");
                else if (selected_image == 1) strcpy(filename,"dark_dojo");
                while ((p1Wins < 2) && (p2Wins < 2) && (roundCounter <= 3)) {
                    int resultado = run_single_player(queue, player_1, player_2, &state, filename, font, player1_sheet, player2_sheet, roundCounter, p1Wins, p2Wins); 
                    if (resultado == 0) resultado = 1 + rand() % 2;  //em caso de empate o vencedor é sorteado
                    if (resultado == 1) p1Wins++;
                    else if (resultado == 2) p2Wins++;
                    else break; //clicou para sair
                    showWinner(font, resultado);
                    resetAttributes(&player_1, 61, 92, 10, Y_SCREEN/2);
                    resetAttributes(&player_2, 61, 92, X_SCREEN-122, Y_SCREEN/2);
                    roundCounter++;
                }
                al_destroy_bitmap(player1_sheet);
                al_destroy_bitmap(player2_sheet);
                joystick_destroy(player_1->control);
                joystick_destroy(player_2->control);
                free(player_1);
                free(player_2);
                state = ENDGAME; 
                break;
            case ENDGAME:
                //mostrar endgame com vencedor e estatisticas
                p1Wins = 0; p2Wins = 0;
                roundCounter = 0;
                state = MENU;
                break;
        }
        
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            state = EXIT;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_event_queue(queue);
    
    return 0;
}

// Arrumar tamanhos (matriz)
// Investigar, com tudo 60 da certo, com os valores setados na mão ele da problema no ataque (chun li(tamanhos variados) vs ken (tamanho tudo 60))
// Arrumar overlap_y no ataque, quando se abaixa nao esta funcionando, toma dano de soco que nao atinge