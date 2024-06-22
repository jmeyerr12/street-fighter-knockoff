#ifndef UTILS_H
#define UTILS_H

/*
    Biblioteca com arquivos utilitários, usados para debugar o código e verificar informações técnicas do jogo
*/

#include <allegro5/allegro.h>
#include "character.h"

void check_v_key_and_print_position(ALLEGRO_EVENT* ev, player* p1, player* p2); // Verifica se a tecla 'V' foi pressionada e imprime a posição do cursor e as estatísticas dos jogadores
void printPlayerStatistics(player *p, int i); // Imprime as estatísticas do jogador

#endif