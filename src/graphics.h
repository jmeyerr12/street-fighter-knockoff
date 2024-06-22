#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "character.h"
#include "background.h"
#include "constants.h"

// Funções de desenho e renderização
void draw_player(ALLEGRO_BITMAP* p, player* player, int current, int movement, int inverted);
void draw_scoreboard(int score1, int score2, int x, ALLEGRO_FONT *font, int countdown, int round, int points1, int points2, unsigned int stamina1, unsigned int stamina2);
void draw_menu(ALLEGRO_FONT* font, int selected_option);
int draw_pause(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue);
void draw_characters_menu(int selected_option1, int selected_option2, ALLEGRO_BITMAP* heads);
void draw_img_menu(ALLEGRO_FONT* font, int selected_option, background* bg);
void draw_character_selection(ALLEGRO_BITMAP* sprite_sheet, int sprite_index, int x, int y, int sprite_dimension, ALLEGRO_COLOR color);

// Funções de controle de menu
int handle_menu_input(ALLEGRO_EVENT event, int* selected_option);
int show_image_menu(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue);
void show_characters_menu(ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_BITMAP** player1_sheet, ALLEGRO_BITMAP** player2_sheet, int *selected_option1, int *selected_option2);

#endif
