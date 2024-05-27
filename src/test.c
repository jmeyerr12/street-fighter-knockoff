#include "character.h"

void handle_down(player *p, int mv, int *frame, int maxFrames, int timer_count) {
    if (mv == GET_DOWN) {
        if (((*frame) < maxFrames) && timer_count % 10 == 0) (*frame)++;  
    } else {     
        if (timer_count % 10 == 0) (*frame)++;  
        if (p->attack != 0 && (*frame) >= maxFrames) p->attack = 0;
        if ((*frame) > maxFrames) (*frame) = 0;
    }
}

void handle_jump(player *p, player *opponent, int *movement) {
    if (p->isJumping == 1) (*movement) = JUMP;
    else if (p->isJumping == 2 && (p->x > opponent->x)) (*movement) = JUMP_FWD;
    else if (p->isJumping == 3 && (p->x > opponent->x)) (*movement) = JUMP_BCK;
    else if (p->isJumping == 2) (*movement) = JUMP_BCK;
    else if (p->isJumping == 3) (*movement) = JUMP_FWD;
}

void handle_attack(player *p, player *opponent, int *movement, int *alreadyDamaged) {
    if (p->attack == ATTACK_KICK) {
        *movement = KICK;
        if (isInRange(p, opponent, KICK) && !(*alreadyDamaged)) {
            (*alreadyDamaged) = 1;
            opponent->health -= 30; //implementar sprite de sofrendo ataque!!
        }
    } else if (p->attack == ATTACK_PUNCH) {
        *movement = PUNCH;
        if (isInRange(p, opponent, PUNCH) && !(*alreadyDamaged)) {
            (*alreadyDamaged) = 1;
            opponent->health -= 30; //implementar sprite de sofrendo ataque!!
        }
    } else (*alreadyDamaged) = 0;
}