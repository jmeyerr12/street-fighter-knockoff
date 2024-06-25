#ifndef GRAPHICS_H
#define GRAPHICS_H

/*
    Biblioteca de funções responsáveis por "desenhar" o jogo na tela.
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "character.h"
#include "constants.h"

typedef struct {
    ALLEGRO_BITMAP* frames[NUM_FRAMES];
    int current_frame;
    float frame_duration;  //duração de cada frame em segundos
    double last_frame_update_time;
} background;

// Funções de desenho e renderização
void draw_player(ALLEGRO_BITMAP* p, player* player, int current, int movement, int inverted); // Desenha o jogador na tela
void draw_scoreboard(int score1, int score2, int x, ALLEGRO_FONT *font, int countdown, int round, int points1, int points2, unsigned int stamina1, unsigned int stamina2); // Desenha o placar do jogo na tela
void draw_menu(ALLEGRO_FONT* font, int selected_option); // Desenha o menu principal na tela
int draw_pause(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue); // Desenha o menu de pausa e lida com eventos
void draw_characters_menu(int selected_option1, int selected_option2, ALLEGRO_BITMAP* heads, int done1, int done2); // Desenha o menu de seleção de personagens
void draw_img_menu(ALLEGRO_FONT* font, int selected_option, background* bg); // Desenha o menu de seleção de imagens de fundo
void draw_character_selection(ALLEGRO_BITMAP* sprite_sheet, int sprite_index, int x, int y, int sprite_dimension, ALLEGRO_COLOR color); // Desenha a seleção de personagens no menu
void showWinner(ALLEGRO_FONT *font, int winner);
void showEndgame(ALLEGRO_FONT *font, int winner);

// Funções de controle de menu
int handle_menu_input(ALLEGRO_EVENT event, int* selected_option); // Lida com a entrada do usuário no menu principal
int show_image_menu(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue, int *state); // Mostra o menu de seleção de imagens de fundo
void show_characters_menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP** player1_sheet, ALLEGRO_BITMAP** player2_sheet, int *selected_option1, int *selected_option2, int *state); // Mostra o menu de seleção de personagens

// Funções da tela de fundo
void init_animated_background(background* bg, float frame_rate, char *filename); // Inicializa o fundo animado com a taxa de quadros e o arquivo especificado
void update_animated_background(background* bg); // Atualiza o estado do fundo animado para o próximo quadro
void draw_animated_background(const background* bg); // Desenha o fundo animado na tela
void destroy_animated_background(background* bg); // Libera a memória alocada para o fundo animado
void draw_tutorial(ALLEGRO_FONT * font);

#endif
