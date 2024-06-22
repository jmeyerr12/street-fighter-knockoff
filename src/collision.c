#include "collision.h"

unsigned char collision_players(player *p1, player *p2) {
    setHitbox(p1);
    setHitbox(p2);

    int overlap_x = (p1->SE.x > p2->SW.x) && (p1->SW.x < p2->SE.x);
    int overlap_y = (p1->SE.y > p2->NW.y) && (p1->NW.y < p2->SE.y);

    return overlap_x && overlap_y;
}

unsigned char collision_wall(player *p) {
    return (p->x <= 6) || (p->SE.x >= X_SCREEN - 6);
}

unsigned char collision_2D(player *p1, player *p2) {
    return collision_players(p1, p2) || collision_wall(p1) || collision_wall(p2);
}

void resolve_collision(player *p1, player *p2) {
    if (p1->direction == LEFT) {
        p1->x -= 2;
        p2->x += 2;
    } else {
        p1->x += 2;
        p2->x -= 2;
    }
}

void out_of_bounds(player *p) {
    if (p->x < 6) {
        p->x = 6;  // Define a borda esquerda como limite mínimo
    } else if (p->x > X_SCREEN - 6 - p->width) {
        p->x = X_SCREEN - 6 - p->width;  // Define a borda direita menos a largura do sprite como limite máximo
    }
}