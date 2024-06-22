# Nome do executável
TARGET = street_fighter

# Diretório das fontes
SRC_DIR = src

# Arquivos fontes
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/character.c $(SRC_DIR)/joystick.c $(SRC_DIR)/street_fighter.c \
       $(SRC_DIR)/input.c $(SRC_DIR)/collision.c $(SRC_DIR)/graphics.c $(SRC_DIR)/utils.c \
       $(SRC_DIR)/game.c $(SRC_DIR)/background.c

# Arquivos objeto (gerados a partir dos fontes)
OBJS = $(patsubst $(SRC_DIR)/%.c, %.o, $(SRCS))

# Flags padrão de compilação
CFLAGS = -Wall -Wextra

# Flags e bibliotecas do Allegro
ALLEGRO_FLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)

# Compilador
CC = gcc

# Nome do arquivo de entrega
LOGIN = jmm23
ENTREGA = $(LOGIN)-A4.tar.gz

# Regra padrão: compilar tudo
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(ALLEGRO_FLAGS)

# Regra para compilar os arquivos fonte em arquivos objeto
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(ALLEGRO_FLAGS)

# Limpar os arquivos objeto e o arquivo de entrega
clean:
	rm -f $(OBJS) $(ENTREGA)
	find $(SRC_DIR) ! -name '*.c' ! -name '*.h' -type f -exec rm -f {} +

# Limpar tudo, incluindo o executável
purge: clean
	rm -f $(TARGET)

# Criar o arquivo de entrega
entrega: purge
	tar -czvf $(ENTREGA) Makefile $(SRC_DIR)

# Regra para forçar a recompilação
.PHONY: all clean purge entrega
