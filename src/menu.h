#include "street_fighter.h"

#define MENU 0
#define GAME 1
#define EXIT 2

#define MENU_START 0
#define MENU_EXIT 1


void draw_menu(ALLEGRO_FONT* font, int selected_option);
int handle_menu_input(ALLEGRO_EVENT event, int* selected_option);
