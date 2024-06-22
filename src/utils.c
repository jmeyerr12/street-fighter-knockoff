#include "utils.h"

void check_v_key_and_print_position(ALLEGRO_EVENT *ev, player* p1, player *p2) {
    if (ev->type == ALLEGRO_EVENT_KEY_DOWN) {
        if (ev->keyboard.keycode == ALLEGRO_KEY_V) { // Tecla 'V'
            int x, y;
            al_get_mouse_cursor_position(&x, &y);
            char position[50];
            snprintf(position, sizeof(position), "Cursor position: (%d, %d)", x, y);
            printf("\n\n%s\n", position);
            printPlayerStatistics(p1, 2);
            printPlayerStatistics(p2, 2);
            fflush(stdout);
        }
    }
}

void printPlayerStatistics(player *p, int i) {
    printf("\n\n -- player %d -- \nisJumping: %d\nisDown: %d\nattack: %s\nheight: %d\nwidth: %d\nx: %d y: %d\nSW: (%d,%d) | SE: (%d,%d)\nNW: (%d,%d) | NE: (%d,%d)\npunching range: (%d, %d)\nkicking range: (%d, %d)", 
            i, p->isJumping, p->isDown,
            !p->attack ? "NOT ATTACKING" : (p->attack == ATTACK_KICK ? "KICKING" : 
            (p->attack == ATTACK_PUNCH ? "PUNCHING" : 
            ((p->attack == ATTACK_JUMPING_PUNCH) ? "JMP PUNCH" : 
            ((p->attack == ATTACK_JUMPING_KICK) ? "JMP KICK" : 
            ((p->attack == ATTACK_DOWN_KICK) ? "DWN KICK" : 
            ((p->attack == ATTACK_DOWN_PUNCH) ? "DWN PUNCH" : "ERROR")))))),
            p->height, p->width,
            p->x, p->y, 
            p->SW.x, p->SW.y, p->SE.x, p->SE.y, p->NW.x, p->NW.y, p->NE.x, p->NE.y,
            0,0,0,0
            );
} 
