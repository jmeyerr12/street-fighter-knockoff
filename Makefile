# Nome do executável
TARGET = street_fighter

# Diretório das fontes
SRC_DIR = src

# Diretório dos assets (se aplicável)
ASSETS_DIR = assets

# Nome do usuário para a entrega (substitua por seu nome de usuário)
USER = jmm23

# Nome da entrega
ENTREGA_NAME = $(USER)-A4

# Arquivo de entrega
ENTREGA = $(ENTREGA_NAME).tar.gz

# Arquivos fontes
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/character.c $(SRC_DIR)/joystick.c $(SRC_DIR)/street_fighter.c \
       $(SRC_DIR)/input.c $(SRC_DIR)/collision.c $(SRC_DIR)/graphics.c $(SRC_DIR)/utils.c \
       $(SRC_DIR)/game.c 

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

# Limpar os arquivos objeto
clean:
	rm -f $(OBJS)
	find $(SRC_DIR) ! -name '*.c' ! -name '*.h' -type f -exec rm -f {} +
	rm -f $(ENTREGA)

# Limpar tudo, incluindo o executável
purge: clean
	rm -f $(TARGET)

# Regra para criar o arquivo tar.gz para entrega
entrega: purge
	mkdir -p $(ENTREGA_NAME)
	cp -r Makefile $(SRC_DIR) $(ASSETS_DIR) $(ENTREGA_NAME)/
	tar -czvf $(ENTREGA) $(ENTREGA_NAME)
	rm -rf $(ENTREGA_NAME)

# Regra para forçar a recompilação
.PHONY: all clean purge entrega
