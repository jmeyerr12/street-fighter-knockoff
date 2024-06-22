#ifndef __JOYSTICK__ 					
#define __JOYSTICK__			

/*
	Biblioteca de joystick, que controla os movimentos basicos de um player
*/

typedef struct {											
	unsigned char right;									
	unsigned char left;									
	unsigned char up;									
	unsigned char down;					
	unsigned char up_left;									
	unsigned char up_right;	
	unsigned char down_left;
	unsigned char down_right;
} joystick;							

joystick* joystick_create(); // Cria e inicializa um joystick
void joystick_destroy(joystick *element); // Destroi e libera a memória do joystick
void joystick_right(joystick *element); // Move o joystick para a direita
void joystick_left(joystick *element); // Move o joystick para a esquerda
void joystick_up(joystick *element); // Move o joystick para cima
void joystick_up_left(joystick *element); // Move o joystick para cima e para a esquerda
void joystick_up_right(joystick *element); // Move o joystick para cima e para a direita
void joystick_down(joystick *element); // Move o joystick para baixo
void joystick_down_left(joystick *element); // Move o joystick para baixo e para a esquerda
void joystick_down_right(joystick *element); // Move o joystick para baixo e para a direita

#endif
