#ifndef CHARACTER_H
#define CHARACTER_H

/*
    Biblioteca à funcões ligadas à inicialização e atualização do personagem
*/

#include <allegro5/allegro.h>
#include <stdio.h>
#include "joystick.h"
#include "constants.h"
#include "collision.h"
#include "player_type.h"
#include "graphics.h"

// Funções de criação, inicialização e liberação de memória de jogadores
player* buildPlayer(unsigned int width, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned int height, int direction); // Cria e inicializa um jogador
void resetAttributes(player **p, unsigned int width, unsigned int height, unsigned short x, unsigned short y); // Redefine os atributos de um jogador, como dimensões e posição
void freePlayer(player *p); // Libera a memória alocada para um jogador

// Funções de movimentação e atualização de estado
void movePlayer(player *element, char steps, unsigned char trajectory); // Move o jogador com base na quantidade de passos e trajetória
void updatePlayer(player *element, float time, unsigned short groundLevel); // Atualiza a posição e o estado do jogador com base no tempo e nos limites do jogo
void resetPlayer(player* p); // Redefine o estado do jogador para o estado inicial
void try_move_player(player *p, player *other, int multiplier, int direction); // Tenta mover o jogador e verifica colisões com outro jogador, revertendo a posição se necessário
void update_position(player *player_1, player *player_2, float time); // Atualiza a posição dos jogadores com base no tempo

// Funções de hitbox e range de ataque
void setHitbox(player* p); // Configura a hitbox do jogador para detecção de colisões
bool isInRange(player *attacker, player *defender, int attack); // Verifica se o atacante está na faixa de ataque do defensor

// Funções de dimensões
size** characterSizes(); // Inicializa e retorna um array de tamanhos de personagens
void freeCharacterSizes(size** charSizes); // Libera a memória alocada para os tamanhos de personagens
int countFrames(int movement); // Conta o número de quadros para uma determinada animação de movimento
void updateDimensions(player *p, size** charSizes, int movement); // Atualiza as dimensões do jogador com base no movimento atual
void setDimensions(player* p, unsigned width, unsigned int height); // Define as dimensões (largura e altura) de um jogador

// Funções de estados de movimento
void handle_down(player *p, int mv, int *frame, int maxFrames, int timer_count); // Lida com o movimento de agachar do jogador
void being_hit(player *p, int mv, int *frame, int maxFrames); // Lida com o estado de ser atingido do jogador
void defense(player *p, int mv, int *frame, int maxFrames); // Lida com o estado de defesa do jogador
void handle_jump(player *p, player *opponent, int *movement); // Lida com o movimento de pulo do jogador

// Funções de ataques, dano e recarga de estamina
void check_and_apply_damage(player *p, player *opponent, int damage, int *alreadyDamaged, int attack, int attackType); // Verifica e aplica dano ao oponente se estiver no alcance do ataque
void handle_attack(player *p, player *opponent, int *movement, int *alreadyDamaged); // Lida com os ataques do jogador e verifica o alcance e aplicação de dano
void recharge_stamina(player* p); // Recarrega a stamina do jogador ao longo do tempo

#endif

