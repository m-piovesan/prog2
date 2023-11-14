CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Lista de todos os arquivos-fonte .c
SRCS = aliens.c audio.c display.c fx.c general.c hud.c keyboard.c obstacles.c ship.c shots.c sprites.c stars.c game.c

# Gera os nomes dos arquivos de objeto (.o)
OBJS = $(SRCS:.c=.o)

# Nome do executável final
TARGET = game

# Regra principal para construir o executável
$(TARGET): $(OBJS)
	$(CC) $^ $(shell pkg-config allegro-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags) -o $@

# Regra para construir cada arquivo de objeto (.o)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Limpar arquivos temporários e o executável
clean:
	rm -f $(OBJS) $(TARGET)
