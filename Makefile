# Nome do executável
TARGET = street_fighter

# Diretório das fontes
SRC_DIR = src

# Arquivos fontes
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/character.c $(SRC_DIR)/joystick.c $(SRC_DIR)/street_fighter.c

# Arquivos objeto (gerados a partir dos fontes)
OBJS = $(patsubst $(SRC_DIR)/%.c, %.o, $(SRCS))

# Flags padrão de compilação
CFLAGS = -Wall -Wextra -std=c11

# Flags e bibliotecas do Allegro
ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

# Compilador
CC = gcc

# Regra padrão: compilar tudo
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(ALLEGRO_FLAGS)

# Regra para compilar os arquivos fonte em arquivos objeto
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(ALLEGRO_FLAGS)

# Limpar os arquivos objeto, o executável e outros arquivos não .c ou .h no diretório src
clean:
	rm -f $(OBJS) $(TARGET)
	find $(SRC_DIR) ! -name '*.c' ! -name '*.h' -type f -exec rm -f {} +

# Regra para forçar a recompilação
.PHONY: all clean

